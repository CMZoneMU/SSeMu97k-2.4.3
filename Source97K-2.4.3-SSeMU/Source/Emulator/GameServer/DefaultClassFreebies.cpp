// Update 93 2.5.1 -> 97K - Presentes iniciais para novos personagens
// DefaultClassFreebies.cpp: implementation of the CDefaultClassFreebies class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DefaultClassFreebies.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "ItemManager.h"
#include "Protocol.h"
#include "MemScript.h"
#include "Util.h"

CDefaultClassFreebies gDefaultClassFreebies;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDefaultClassFreebies::CDefaultClassFreebies()
{
	for(int n = 0; n < MAX_CLASS; n++)
	{
		this->m_DefaultClassFreebiesInfo[n].Class = n;
		this->m_DefaultClassFreebiesInfo[n].Money = 0;
		this->m_DefaultClassFreebiesInfo[n].LevelUpPoint = 0;
		this->m_DefaultClassFreebiesInfo[n].ItemList.clear();
		this->m_DefaultClassFreebiesInfo[n].BuffList.clear();
	}
}

CDefaultClassFreebies::~CDefaultClassFreebies()
{

}

void CDefaultClassFreebies::Load(char* path)
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

	for(int n = 0; n < MAX_CLASS; n++)
	{
		this->m_DefaultClassFreebiesInfo[n].Class = n;
		this->m_DefaultClassFreebiesInfo[n].Money = 0;
		this->m_DefaultClassFreebiesInfo[n].LevelUpPoint = 0;
		this->m_DefaultClassFreebiesInfo[n].ItemList.clear();
		this->m_DefaultClassFreebiesInfo[n].BuffList.clear();
	}

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

					DEFAULT_CLASS_FREEBIES_INFO info;
					info.ItemList.clear();
					info.BuffList.clear();

					info.Class = lpMemScript->GetNumber();

					info.Money = lpMemScript->GetAsNumber();

					info.LevelUpPoint = lpMemScript->GetAsNumber();

					this->SetInfo(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					DEFAULT_CLASS_FREEBIES_ITEM_INFO info;

					info.Class = lpMemScript->GetNumber();

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
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					DEFAULT_CLASS_FREEBIES_BUFF_INFO info;

					info.Class = lpMemScript->GetNumber();

					info.Effect = lpMemScript->GetAsNumber();

					info.Type = lpMemScript->GetAsNumber();

					info.Power[0] = lpMemScript->GetAsNumber();

					info.Power[1] = lpMemScript->GetAsNumber();

					info.Power[2] = lpMemScript->GetAsNumber();

					info.Power[3] = lpMemScript->GetAsNumber();

					info.Duration = lpMemScript->GetAsNumber();

					this->SetBuffInfo(info);
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

void CDefaultClassFreebies::SetInfo(DEFAULT_CLASS_FREEBIES_INFO info)
{
	if(CHECK_RANGE(info.Class,MAX_CLASS) == 0)
	{
		return;
	}

	this->m_DefaultClassFreebiesInfo[info.Class].Class = info.Class;
	this->m_DefaultClassFreebiesInfo[info.Class].Money = info.Money;
	this->m_DefaultClassFreebiesInfo[info.Class].LevelUpPoint = info.LevelUpPoint;
}

void CDefaultClassFreebies::SetItemInfo(DEFAULT_CLASS_FREEBIES_ITEM_INFO info)
{
	if(CHECK_RANGE(info.Class,MAX_CLASS) == 0)
	{
		return;
	}

	this->m_DefaultClassFreebiesInfo[info.Class].ItemList.push_back(info);
}

void CDefaultClassFreebies::SetBuffInfo(DEFAULT_CLASS_FREEBIES_BUFF_INFO info)
{
	if(CHECK_RANGE(info.Class,MAX_CLASS) == 0)
	{
		return;
	}

	this->m_DefaultClassFreebiesInfo[info.Class].BuffList.push_back(info);
}

void CDefaultClassFreebies::GetCharacterFreebies(LPOBJ lpObj,int status)
{
	if(status == 0)
	{
		return;
	}

	if(CHECK_RANGE(lpObj->Class,MAX_CLASS) == 0)
	{
		return;
	}

	DEFAULT_CLASS_FREEBIES_INFO lpInfo = this->m_DefaultClassFreebiesInfo[lpObj->Class];

	if(lpInfo.Money > 0)
	{
		if(gObjCheckMaxMoney(lpObj->Index,lpInfo.Money) == 0)
		{
			lpObj->Money = MAX_MONEY;
		}
		else
		{
			lpObj->Money += lpInfo.Money;
		}

		GCMoneySend(lpObj->Index,lpObj->Money);
	}

	if(lpInfo.LevelUpPoint > 0)
	{
		lpObj->LevelUpPoint += lpInfo.LevelUpPoint;
		GCNewCharacterInfoSend(lpObj);
		GDCharacterInfoSaveSend(lpObj->Index);
	}

	for(std::vector<DEFAULT_CLASS_FREEBIES_ITEM_INFO>::iterator it = lpInfo.ItemList.begin(); it != lpInfo.ItemList.end(); it++)
	{
		GDCreateItemSend(lpObj->Index,0xEB,0,0,it->ItemIndex,it->Level,it->Dur,it->Option1,it->Option2,it->Option3,-1,it->NewOption,it->SetOption,((it->Duration>0) ? ((DWORD)time(0)+it->Duration) : 0));
	}

	for(std::vector<DEFAULT_CLASS_FREEBIES_BUFF_INFO>::iterator it = lpInfo.BuffList.begin(); it != lpInfo.BuffList.end(); it++)
	{
		if(gEffectManager.GetInfo(it->Effect) == 0)
		{
			continue;
		}

		gEffectManager.AddEffect(lpObj,(it->Type == 0) ? 0 : 1,it->Effect,(it->Type == 0) ? it->Duration : (int)(time(0)+it->Duration),it->Power[0],it->Power[1],it->Power[2],it->Power[3]);
	}
}
