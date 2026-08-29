// GoldenArcher.cpp: implementation of the CGoldenArcher class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GoldenArcher.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "ItemManager.h"
#include "ItemOptionRate.h"
#include "MemScript.h"
#include "RandomManager.h"
#include "Util.h"

CGoldenArcher gGoldenArcher;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGoldenArcher::CGoldenArcher() // OK
{
	this->m_GoldenArcherItemInfo.clear();
}

CGoldenArcher::~CGoldenArcher() // OK
{

}

void CGoldenArcher::Load(char* path) // OK
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

	this->m_GoldenArcherItemInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			GOLDEN_ARCHER_ITEM_INFO info;

			info.ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetNumber(),lpMemScript->GetAsNumber()));

			info.Group = lpMemScript->GetAsNumber();

			info.Option0 = lpMemScript->GetAsNumber();

			info.Option1 = lpMemScript->GetAsNumber();

			info.Option2 = lpMemScript->GetAsNumber();

			info.Option3 = lpMemScript->GetAsNumber();

			info.Option4 = lpMemScript->GetAsNumber();

			info.Duration = lpMemScript->GetAsNumber();

			info.DropRate = lpMemScript->GetAsNumber();

			this->m_GoldenArcherItemInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CGoldenArcher::CGGoldenArcherCountRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(((lpObj->GoldenArcherTransaction[0] == 0)?(lpObj->GoldenArcherTransaction[0]++):lpObj->GoldenArcherTransaction[0]) != 0)
	{
		return;
	}

	SDHP_GOLDEN_ARCHER_COUNT_SEND pMsg;

	pMsg.header.set(0x94,0x00,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CGoldenArcher::CGGoldenArcherRegisterRecv(PMSG_GOLDEN_ARCHER_COUNT_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(gItemManager.GetInventoryItemCount(lpObj,GET_ITEM(14,21),0) != 0)
	{
		gItemManager.DeleteInventoryItemCount(lpObj,GET_ITEM(14,21),0,1);

		this->GDGoldenArcherAddCountSaveSend(aIndex,1);

		this->CGGoldenArcherCountRecv(aIndex);
	}
}

void CGoldenArcher::CGGoldenArcherRewardRecv(PMSG_GOLDEN_ARCHER_REWARD_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(((lpObj->GoldenArcherTransaction[1] == 0)?(lpObj->GoldenArcherTransaction[1]++):lpObj->GoldenArcherTransaction[1]) != 0)
	{
		return;
	}

	SDHP_GOLDEN_ARCHER_REWARD_SEND pMsg;

	pMsg.header.set(0x94,0x01,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	pMsg.type = lpMsg->type;

	pMsg.count = m_GoldenArcherAmount[pMsg.type];

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CGoldenArcher::CGGoldenArcherCloseRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Interface.use == 0 || lpObj->Interface.type == INTERFACE_NONE || lpObj->Interface.type != INTERFACE_GOLDEN_ARCHER)
	{
		return;
	}

	lpObj->Interface.use = 0;
	lpObj->Interface.type = INTERFACE_NONE;
	lpObj->Interface.state = 0;
}

void CGoldenArcher::DGGoldenArcherCountRecv(SDHP_GOLDEN_ARCHER_COUNT_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGGoldenArcherCountRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->GoldenArcherTransaction[0] = 0;

	PMSG_GOLDEN_ARCHER_COUNT_SEND pMsg;

	pMsg.header.set(0x94,sizeof(pMsg));

	pMsg.type = 0;

	pMsg.count = lpMsg->count;

	memset(pMsg.code,0,sizeof(pMsg.code));

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
}

void CGoldenArcher::DGGoldenArcherRewardRecv(SDHP_GOLDEN_ARCHER_REWARD_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGGoldenArcherRewardRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->GoldenArcherTransaction[1] = 0;

	if(lpMsg->result != 0)
	{
		CRandomManager RandomManager;

		for(std::vector<GOLDEN_ARCHER_ITEM_INFO>::iterator it=this->m_GoldenArcherItemInfo.begin();it != this->m_GoldenArcherItemInfo.end();it++)
		{
			if(it->Group == lpMsg->type)
			{
				RandomManager.AddElement((int)(&(*it)),it->DropRate);
			}
		}

		GOLDEN_ARCHER_ITEM_INFO* lpGoldenArcherItemInfo = 0;

		if(RandomManager.GetRandomElement((int*)&lpGoldenArcherItemInfo) != 0)
		{
			WORD ItemIndex = lpGoldenArcherItemInfo->ItemIndex;
			BYTE ItemLevel = 0;
			BYTE ItemOption1 = 0;
			BYTE ItemOption2 = 0;
			BYTE ItemOption3 = 0;
			BYTE ItemNewOption = 0;
			BYTE ItemSetOption = 0;

			gItemOptionRate.GetItemOption0(lpGoldenArcherItemInfo->Option0,&ItemLevel);

			gItemOptionRate.GetItemOption1(lpGoldenArcherItemInfo->Option1,&ItemOption1);

			gItemOptionRate.GetItemOption2(lpGoldenArcherItemInfo->Option2,&ItemOption2);

			gItemOptionRate.GetItemOption3(lpGoldenArcherItemInfo->Option3,&ItemOption3);

			gItemOptionRate.GetItemOption4(lpGoldenArcherItemInfo->Option4,&ItemNewOption);

			gItemOptionRate.GetItemOption5(lpGoldenArcherItemInfo->Option5,&ItemSetOption);

			gItemOptionRate.MakeNewOption(ItemIndex,1,ItemNewOption,&ItemNewOption);

			gItemOptionRate.MakeSetOption(ItemIndex,ItemSetOption,&ItemSetOption);

			if(gItemManager.CheckItemInventorySpace(lpObj,ItemIndex) != 0)
			{
				GDCreateItemSend(lpObj->Index,0xEB,0,0,ItemIndex,ItemLevel,0,ItemOption1,ItemOption2,ItemOption3,-1,ItemNewOption,ItemSetOption,((lpGoldenArcherItemInfo->Duration>0)?((DWORD)time(0)+lpGoldenArcherItemInfo->Duration):0));
			}
			else
			{
				GDCreateItemSend(lpObj->Index,lpObj->Map,(BYTE)lpObj->X,(BYTE)lpObj->Y,ItemIndex,ItemLevel,0,ItemOption1,ItemOption2,ItemOption3,lpObj->Index,ItemNewOption,ItemSetOption,((lpGoldenArcherItemInfo->Duration>0)?((DWORD)time(0)+lpGoldenArcherItemInfo->Duration):0));
			}

			GCServerCommandSend(lpObj->Index,0,lpObj->X,lpObj->Y,1);
		}

		this->CGGoldenArcherCountRecv(lpMsg->index);
	}
}

void CGoldenArcher::GDGoldenArcherAddCountSaveSend(int aIndex, DWORD count) // OK
{
	SDHP_GOLDEN_ARCHER_ADD_COUNT_SAVE_SEND pMsg;

	pMsg.header.set(0x94,0x30,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	pMsg.count = count;

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));
}