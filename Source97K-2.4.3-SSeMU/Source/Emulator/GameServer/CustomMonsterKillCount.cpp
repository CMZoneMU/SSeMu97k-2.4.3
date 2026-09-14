// Update 93 2.5.1 -> 97K - Contador de mortes de monstros / missões de caça
// CustomMonsterKillCount.cpp: implementation of the CCustomMonsterKillCount class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomMonsterKillCount.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "ItemManager.h"
#include "MonsterManager.h"
#include "MemScript.h"
#include "Message.h"
#include "Notice.h"
#include "Util.h"

CCustomMonsterKillCount gCustomMonsterKillCount;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomMonsterKillCount::CCustomMonsterKillCount()
{
	this->m_CustomMonsterKillCountSwitch = 0;
	this->m_MonsterKillCountInfo.clear();
}

CCustomMonsterKillCount::~CCustomMonsterKillCount()
{

}

void CCustomMonsterKillCount::ReadCustomMonsterKillCountInfo(char* section,char* path)
{
	this->m_CustomMonsterKillCountSwitch = GetPrivateProfileInt(section,"CustomMonsterKillCountSwitch",0,path);
}

void CCustomMonsterKillCount::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->m_MonsterKillCountInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					MONSTER_KILL_COUNT_INFO info;

					info.Index = lpMemScript->GetNumber();

					info.MonsterClass = lpMemScript->GetAsNumber();

					info.Enable[0] = lpMemScript->GetAsNumber();

					info.Enable[1] = lpMemScript->GetAsNumber();

					info.Enable[2] = lpMemScript->GetAsNumber();

					info.Enable[3] = lpMemScript->GetAsNumber();

					info.Repeatable = lpMemScript->GetAsNumber();

					info.KillCount = lpMemScript->GetAsNumber();

					info.Money = lpMemScript->GetAsNumber();

					lpMemScript->GetAsNumber(); // Coin1 (fallback)
					
					lpMemScript->GetAsNumber(); // Coin2 (fallback)

					strcpy_s(info.Message,lpMemScript->GetAsString());

					this->SetInfo(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					MONSTER_KILL_ITEM_LIST info;

					info.Index = lpMemScript->GetNumber();

					int ItemCat = lpMemScript->GetAsNumber();

					info.ItemIndex = SafeGetItem(GET_ITEM(ItemCat,lpMemScript->GetAsNumber()));

					info.Level = lpMemScript->GetAsNumber();

					info.Dur = lpMemScript->GetAsNumber();

					info.Option1 = lpMemScript->GetAsNumber();

					info.Option2 = lpMemScript->GetAsNumber();

					info.Option3 = lpMemScript->GetAsNumber();

					info.NewOption = lpMemScript->GetAsNumber();

					info.SetOption = lpMemScript->GetAsNumber();

					info.Duration = lpMemScript->GetAsNumber();

					this->SetItemInfo(info);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomMonsterKillCount::SetInfo(MONSTER_KILL_COUNT_INFO info)
{
	std::map<int,MONSTER_KILL_COUNT_INFO>::iterator it = this->m_MonsterKillCountInfo.find(info.Index);

	if(it != this->m_MonsterKillCountInfo.end())
	{
		return;
	}

	this->m_MonsterKillCountInfo.insert(std::pair<int,MONSTER_KILL_COUNT_INFO>(info.Index,info));
}

void CCustomMonsterKillCount::SetItemInfo(MONSTER_KILL_ITEM_LIST info)
{
	std::map<int,MONSTER_KILL_COUNT_INFO>::iterator it = this->m_MonsterKillCountInfo.find(info.Index);

	if(it == this->m_MonsterKillCountInfo.end())
	{
		return;
	}

	it->second.ItemList.push_back(info);
}

void CCustomMonsterKillCount::CheckMonsterKill(LPOBJ lpObj,LPOBJ lpTarget)
{
	if(this->m_CustomMonsterKillCountSwitch == 0)
	{
		return;
	}

	for(std::map<int,MONSTER_KILL_COUNT_INFO>::iterator it=this->m_MonsterKillCountInfo.begin();it != this->m_MonsterKillCountInfo.end();it++)
	{
		if(it->second.MonsterClass != lpObj->Class)
		{
			continue;
		}

		if(it->second.Enable[lpTarget->AccountLevel] == 0)
		{
			continue;
		}

		SDHP_MONSTER_KILL_COUNT_SEND pMsg;

		pMsg.header.set(0x50,sizeof(pMsg));

		pMsg.Index = lpTarget->Index;

		memcpy(pMsg.Account,lpTarget->Account,sizeof(pMsg.Account));

		memcpy(pMsg.Name,lpTarget->Name,sizeof(pMsg.Name));

		pMsg.MonsterClass = lpObj->Class;

		gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);

		return;
	}
}

void CCustomMonsterKillCount::DGMonsterKillCountRecv(SDHP_MONSTER_KILL_COUNT_RECV* lpMsg)
{
	if(this->m_CustomMonsterKillCountSwitch == 0)
	{
		return;
	}

	if(gObjIsAccountValid(lpMsg->Index,lpMsg->Account) == 0)
	{
		LogAdd(LOG_RED,"[DGMonsterKillCountRecv] Invalid Account [%d](%s)",lpMsg->Index,lpMsg->Account);
		CloseClient(lpMsg->Index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->Index];

	for(std::map<int,MONSTER_KILL_COUNT_INFO>::iterator it=this->m_MonsterKillCountInfo.begin();it != this->m_MonsterKillCountInfo.end();it++)
	{
		if(it->second.MonsterClass != lpMsg->MonsterClass)
		{
			continue;
		}

		int KillCount = it->second.KillCount;

		if(lpMsg->Count > KillCount)
		{
			if(it->second.Repeatable == 0)
			{
				continue;
			}

			int multiplicador = (lpMsg->Count+KillCount-1)/KillCount;

			KillCount *= multiplicador;
		}

		if(lpMsg->Count < KillCount)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,"%s %d/%d",gMonsterManager.GetName(lpMsg->MonsterClass),lpMsg->Count,KillCount);
			continue;
		}

		if(it->second.Money > 0)
		{
			if(gObjCheckMaxMoney(lpObj->Index,it->second.Money) == 0)
			{
				lpObj->Money = MAX_MONEY;
			}
			else
			{
				lpObj->Money += it->second.Money;
			}

			GCMoneySend(lpObj->Index,lpObj->Money);
		}

		int px = lpObj->X;
		int py = lpObj->Y;

		for(std::vector<MONSTER_KILL_ITEM_LIST>::iterator ir = it->second.ItemList.begin(); ir != it->second.ItemList.end(); ir++)
		{
			if(gObjGetRandomFreeLocation(lpObj->Map,&px,&py,2,2,50) == 0)
			{
				px = lpObj->X;
				py = lpObj->Y;
			}

			GDCreateItemSend(lpObj->Index,lpObj->Map,(BYTE)px,(BYTE)py,ir->ItemIndex,(BYTE)ir->Level,(BYTE)ir->Dur,(BYTE)ir->Option1,(BYTE)ir->Option2,(BYTE)ir->Option3,lpObj->Index,(BYTE)ir->NewOption,(BYTE)ir->SetOption,((ir->Duration>0)?((DWORD)time(0)+ir->Duration):0));
		}

		GCServerCommandSend(lpObj->Index,0,lpObj->X,lpObj->Y,1);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,it->second.Message,KillCount);

		return;
	}
}
