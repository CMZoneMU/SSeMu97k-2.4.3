// BonusManager.cpp: implementation of the CBonusManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BonusManager.h"
#include "DSProtocol.h"
#include "ItemDrop.h"
#include "ItemManager.h"
#include "ItemOptionRate.h"
#include "MemScript.h"
#include "Message.h"
#include "Monster.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "ScheduleManager.h"
#include "ServerInfo.h"
#include "Util.h"

CBonusManager gBonusManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBonusManager::CBonusManager() // OK
{
	for(int n=0;n < MAX_BONUS;n++)
	{
		BONUS_INFO* lpInfo = &this->m_BonusInfo[n];

		lpInfo->Index = n;
		lpInfo->State = BONUS_STATE_BLANK;
		lpInfo->RemainTime = 0;
		lpInfo->TargetTime = 0;
		lpInfo->TickCount = GetTickCount();

		lpInfo->StartTime.clear();

		lpInfo->ValueInfo.clear();
	}
}

CBonusManager::~CBonusManager() // OK
{

}

void CBonusManager::Init() // OK
{
	for(int n=0;n < MAX_BONUS;n++)
	{
		if(gServerInfo.m_BonusManagerSwitch == 0)
		{
			this->SetState(&this->m_BonusInfo[n],BONUS_STATE_BLANK);
		}
		else
		{
			this->SetState(&this->m_BonusInfo[n],BONUS_STATE_EMPTY);
		}
	}
}

void CBonusManager::Load(char* path) // OK
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

	for(int n=0;n < MAX_BONUS;n++)
	{
		this->m_BonusInfo[n].StartMessage = -1;
		this->m_BonusInfo[n].FinalMessage = -1;
		this->m_BonusInfo[n].BonusTime = 0;
		this->m_BonusInfo[n].StartTime.clear();
		this->m_BonusInfo[n].ValueInfo.clear();
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

					BONUS_START_TIME info;

					int index = lpMemScript->GetNumber();

					info.Year = lpMemScript->GetAsNumber();

					info.Month = lpMemScript->GetAsNumber();

					info.Day = lpMemScript->GetAsNumber();

					info.DayOfWeek = lpMemScript->GetAsNumber();

					info.Hour = lpMemScript->GetAsNumber();

					info.Minute = lpMemScript->GetAsNumber();

					info.Second = lpMemScript->GetAsNumber();

					this->m_BonusInfo[index].StartTime.push_back(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int index = lpMemScript->GetNumber();

					this->m_BonusInfo[index].StartMessage = lpMemScript->GetAsNumber();

					this->m_BonusInfo[index].FinalMessage = lpMemScript->GetAsNumber();

					this->m_BonusInfo[index].BonusTime = lpMemScript->GetAsNumber();

					strcpy_s(this->m_BonusInfo[index].Name,lpMemScript->GetAsString());
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					BONUS_VALUE_INFO info;

					int index = lpMemScript->GetNumber();

					info.BonusIndex = lpMemScript->GetAsNumber();

					info.BonusValue[0] = lpMemScript->GetAsNumber();

					info.BonusValue[1] = lpMemScript->GetAsNumber();

					info.BonusValue[2] = lpMemScript->GetAsNumber();

					info.BonusValue[3] = lpMemScript->GetAsNumber();

					info.MapNumber = lpMemScript->GetAsNumber();

					this->m_BonusInfo[index].ValueInfo.push_back(info);
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

void CBonusManager::MainProc() // OK
{
	if(gServerInfo.m_BonusManagerSwitch == 0)
	{
		return;
	}

	for(int n=0;n < MAX_BONUS;n++)
	{
		BONUS_INFO* lpLevel = &this->m_BonusInfo[n];

		if((GetTickCount()-lpLevel->TickCount) >= 1000)
		{
			lpLevel->TickCount = GetTickCount();

			lpLevel->RemainTime = (int)difftime(lpLevel->TargetTime,time(0));

			switch(lpLevel->State)
			{
				case BONUS_STATE_BLANK:
					this->ProcState_BLANK(lpLevel);
					break;
				case BONUS_STATE_EMPTY:
					this->ProcState_EMPTY(lpLevel);
					break;
				case BONUS_STATE_START:
					this->ProcState_START(lpLevel);
					break;
			}
		}
	}
}

void CBonusManager::ProcState_BLANK(BONUS_INFO* lpInfo) // OK
{

}

void CBonusManager::ProcState_EMPTY(BONUS_INFO* lpInfo) // OK
{
	if(lpInfo->RemainTime <= 0)
	{
		LogAdd(LOG_BLACK,"[Bonus Manager] (%d) SetState START",lpInfo->Index);
		if(lpInfo->StartMessage != -1){gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"%s",gMessage.GetMessage(lpInfo->StartMessage)); }
		this->SetState(lpInfo,BONUS_STATE_START);
	}
}

void CBonusManager::ProcState_START(BONUS_INFO* lpInfo) // OK
{
	if(lpInfo->RemainTime <= 0)
	{
		LogAdd(LOG_BLACK,"[Bonus Manager] (%d) SetState EMPTY",lpInfo->Index);
		if(lpInfo->FinalMessage != -1){gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"%s",gMessage.GetMessage(lpInfo->FinalMessage));}
		this->SetState(lpInfo,BONUS_STATE_EMPTY);
	}
}

void CBonusManager::SetState(BONUS_INFO* lpInfo,int state) // OK
{
	if(gServerInfo.m_BonusManagerSwitch == 0)
	{
		return;
	}

	lpInfo->State = state;

	switch(lpInfo->State)
	{
		case BONUS_STATE_BLANK:
			this->SetState_BLANK(lpInfo);
			break;
		case BONUS_STATE_EMPTY:
			this->SetState_EMPTY(lpInfo);
			break;
		case BONUS_STATE_START:
			this->SetState_START(lpInfo);
			break;
	}
}

void CBonusManager::SetState_BLANK(BONUS_INFO* lpInfo) // OK
{

}

void CBonusManager::SetState_EMPTY(BONUS_INFO* lpInfo) // OK
{
	this->CheckSync(lpInfo);
}

void CBonusManager::SetState_START(BONUS_INFO* lpInfo) // OK
{
	lpInfo->RemainTime = lpInfo->BonusTime;

	lpInfo->TargetTime = (int)(time(0)+lpInfo->RemainTime);
}

void CBonusManager::CheckSync(BONUS_INFO* lpInfo) // OK
{
	if(lpInfo->StartTime.empty() != 0)
	{
		this->SetState(lpInfo,BONUS_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<BONUS_START_TIME>::iterator it=lpInfo->StartTime.begin();it != lpInfo->StartTime.end();it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpInfo,BONUS_STATE_BLANK);
		return;
	}

	lpInfo->RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	lpInfo->TargetTime = (int)ScheduleTime.GetTime();

	int sec = lpInfo->RemainTime;

	int day = sec/86400;
	sec %= 86400;

	int hr = sec/3600;
	sec %= 3600;

	int min = sec/60;

	if(day > 0)
	{
		LogAdd(LOG_BLACK,"[Bonus Manager] (%d) Sync Start Time. [%d] day [%d] hour [%d] min remain",lpInfo->Index,day,hr,min);
	}
	else if(hr > 0)
	{
		LogAdd(LOG_BLACK,"[Bonus Manager] (%d) Sync Start Time. [%d] hour [%d] min remain",lpInfo->Index,hr,min);
	}
	else
	{
		LogAdd(LOG_BLACK,"[Bonus Manager] (%d) Sync Start Time. [%d] min remain",lpInfo->Index,min);
	}
}

int CBonusManager::GetState(int index) // OK
{
	if(index < 0 || index >= MAX_BONUS)
	{
		return BONUS_STATE_BLANK;
	}

	return this->m_BonusInfo[index].State;
}

int CBonusManager::GetBonusValue(LPOBJ lpObj,int BonusIndex,int BonusValue) // OK
{
	for(int n=0;n < MAX_BONUS;n++)
	{
		if(this->m_BonusInfo[n].State == BONUS_STATE_START)
		{
			for(std::vector<BONUS_VALUE_INFO>::iterator it=this->m_BonusInfo[n].ValueInfo.begin();it != this->m_BonusInfo[n].ValueInfo.end();it++)
			{
				if(it->BonusIndex == BonusIndex && (it->MapNumber == -1 || it->MapNumber == lpObj->Map))
				{
					switch(it->BonusIndex)
					{
						case BONUS_INDEX_EXPERIENCE_RATE:
							BonusValue += (BonusValue*it->BonusValue[lpObj->AccountLevel])/100;
							break;
						case BONUS_INDEX_ITEM_DROP_RATE:
							BonusValue += (BonusValue*it->BonusValue[lpObj->AccountLevel])/100;
							break;
						case BONUS_INDEX_EXC_ITEM_DROP_RATE:
							BonusValue += ((__int64)(1000000-BonusValue)*it->BonusValue[lpObj->AccountLevel])/1000000;
							break;
						case BONUS_INDEX_SET_ITEM_DROP_RATE:
							BonusValue += ((__int64)(1000000-BonusValue)*it->BonusValue[lpObj->AccountLevel])/1000000;
							break;
						case BONUS_INDEX_CHAOS_MIX_RATE:
							BonusValue += it->BonusValue[lpObj->AccountLevel];
							break;
					}
				}
			}
		}
	}

	return BonusValue;
}

void CBonusManager::ForceStart(int index) // OK
{
	if(gServerInfo.m_BonusManagerSwitch == 0)
	{
		return;
	}

	if(index < 0 || index >= MAX_BONUS)
	{
		return;
	}

	BONUS_INFO* lpInfo = &this->m_BonusInfo[index];

	if(this->GetState(index) == BONUS_STATE_BLANK)
	{
		return;
	}

	SYSTEMTIME SystemTime;

	GetLocalTime(&SystemTime);

	BONUS_START_TIME info;

	info.Year = SystemTime.wYear;

	info.Month = SystemTime.wMonth;

	info.Day = SystemTime.wDay;

	info.DayOfWeek = -1;

	info.Hour = SystemTime.wHour;

	info.Minute = SystemTime.wMinute;

	info.Second = SystemTime.wSecond;

	lpInfo->StartTime.push_back(info);

	this->SetState(lpInfo,BONUS_STATE_EMPTY);
}