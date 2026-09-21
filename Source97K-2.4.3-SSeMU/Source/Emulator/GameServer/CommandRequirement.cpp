// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 96 (2.5.4) - Command requirement system implementation
#include "stdafx.h"
#include "CommandRequirement.h"
#include "EffectManager.h"
#include "Guild.h"
#include "GuildManager.h"
#include "ItemManager.h"
#include "MemScript.h"
#include "Message.h"
#include "Notice.h"
#include "Quest.h"
#include "Util.h"

CCommandRequirement gCommandRequirement;

CCommandRequirement::CCommandRequirement()
{
	this->m_CommandRequirementInfo.clear();
}

CCommandRequirement::~CCommandRequirement()
{

}

void CCommandRequirement::Load(char* path)
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

	this->m_CommandRequirementInfo.clear();

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

			COMMAND_REQUIREMENT_INFO info;

			memset(&info,0,sizeof(info));

			info.Index = lpMemScript->GetNumber();

			info.AccountLevel = lpMemScript->GetAsNumber();

			info.MinLevel = lpMemScript->GetAsNumber();

			info.MaxLevel = lpMemScript->GetAsNumber();

			info.MinReset = lpMemScript->GetAsNumber();

			info.MaxReset = lpMemScript->GetAsNumber();

			info.CheckItem = lpMemScript->GetAsNumber();

			info.ItemIndex = lpMemScript->GetAsNumber();

			info.ItemIndex = (info.ItemIndex != -1) ? SafeGetItem(GET_ITEM(info.ItemIndex,lpMemScript->GetAsNumber())) : info.ItemIndex;

			info.ItemCount = lpMemScript->GetAsNumber();

			info.ItemLevel = lpMemScript->GetAsNumber();

			info.EffectState = lpMemScript->GetAsNumber();

			info.QuestIndex = lpMemScript->GetAsNumber();

			info.QuestState = lpMemScript->GetAsNumber();

			info.PKLevel = lpMemScript->GetAsNumber();

			info.PKCount = lpMemScript->GetAsNumber();

			info.GuildRank = lpMemScript->GetAsNumber();

			info.MapIndex = lpMemScript->GetAsNumber();

			info.X = lpMemScript->GetAsNumber();

			info.Y = lpMemScript->GetAsNumber();

			info.TX = lpMemScript->GetAsNumber();

			info.TY = lpMemScript->GetAsNumber();

			info.DayOfWeek = lpMemScript->GetAsNumber();

			info.MinHour = lpMemScript->GetAsNumber();

			info.MaxHour = lpMemScript->GetAsNumber();

			info.FailMessage = lpMemScript->GetAsNumber();

			for(int n=0; n < MAX_CLASS; n++)
			{
				info.RequireClass[n] = lpMemScript->GetAsNumber();
			}

			this->m_CommandRequirementInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CCommandRequirement::CommandCheck(LPOBJ lpObj,int index)
{
	SYSTEMTIME SystemTime;

	GetSystemTime(&SystemTime);

	for(std::vector<COMMAND_REQUIREMENT_INFO>::iterator it = this->m_CommandRequirementInfo.begin(); it != this->m_CommandRequirementInfo.end(); it++)
	{
		if(it->Index != index)
		{
			continue;
		}

		if(it->AccountLevel != -1 && it->AccountLevel > lpObj->AccountLevel)
		{
			continue;
		}

		if(it->MinLevel != -1 && it->MinLevel > lpObj->Level)
		{
			continue;
		}

		if(it->MaxLevel != -1 && it->MaxLevel < lpObj->Level)
		{
			continue;
		}

		if(it->MinReset != -1 && it->MinReset > lpObj->Reset)
		{
			continue;
		}

		if(it->MaxReset != -1 && it->MaxReset < lpObj->Reset)
		{
			continue;
		}

		if(it->RequireClass[lpObj->Class] == 0 || it->RequireClass[lpObj->Class] > (lpObj->ChangeUp+1))
		{
			continue;
		}

		if(it->CheckItem != -1)
		{
			for(int n = 0; n < INVENTORY_WEAR_SIZE; n++)
			{
				if(lpObj->Inventory[n].IsItem() != 0)
				{
					if(it->FailMessage != -1){gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(it->FailMessage));}
					return 1;
				}
			}
		}

		if(it->ItemIndex != -1 && gItemManager.GetInventoryItemCount(lpObj,it->ItemIndex,it->ItemLevel) < it->ItemCount)
		{
			if(it->FailMessage != -1){gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(it->FailMessage));}
			return 1;
		}

		if(it->EffectState != -1 && gEffectManager.CheckEffect(lpObj,it->EffectState) == 0)
		{
			if(it->FailMessage != -1){gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(it->FailMessage));}
			return 1;
		}

		if(it->QuestIndex != -1 && gQuest.CheckQuestListState(lpObj,it->QuestIndex,it->QuestState) == 0)
		{
			if(it->FailMessage != -1){gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(it->FailMessage));}
			return 1;
		}

		if(it->PKLevel != -1 && it->PKLevel != lpObj->PKLevel)
		{
			if(it->FailMessage != -1){gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(it->FailMessage));}
			return 1;
		}

		if(it->PKCount != -1 && it->PKCount > lpObj->PKCount)
		{
			if(it->FailMessage != -1){gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(it->FailMessage));}
			return 1;
		}

		if(it->GuildRank != -1)
		{
			int guildRank = GUILD_RANK_NONE;

			if(lpObj->Guild != 0)
			{
				guildRank = (_stricmp(lpObj->Guild->Master,lpObj->Name) == 0) ? GUILD_RANK_MASTER : GUILD_RANK_NORMAL;
			}

			if(it->GuildRank != guildRank)
			{
				if(it->FailMessage != -1){gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(it->FailMessage));}
				return 1;
			}
		}

		if(it->MapIndex != -1 && it->MapIndex != lpObj->Map)
		{
			if(it->FailMessage != -1){gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(it->FailMessage));}
			return 1;
		}

		if(it->X != -1 && it->Y != -1 && ((it->X > lpObj->X || it->TX < lpObj->X) || (it->Y > lpObj->Y || it->TY < lpObj->Y)))
		{
			if(it->FailMessage != -1){gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(it->FailMessage));}
			return 1;
		}

		if(it->DayOfWeek != -1 && it->DayOfWeek != (SystemTime.wDayOfWeek+1))
		{
			if(it->FailMessage != -1){gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(it->FailMessage));}
			return 1;
		}

		if(it->MinHour != -1 && it->MinHour > SystemTime.wHour)
		{
			if(it->FailMessage != -1){gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(it->FailMessage));}
			return 1;
		}

		if(it->MaxHour != -1 && it->MaxHour < SystemTime.wHour)
		{
			if(it->FailMessage != -1){gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(it->FailMessage));}
			return 1;
		}
	}

	return 0;
}

void CCommandRequirement::CommandDone(LPOBJ lpObj,int index)
{
	for(std::vector<COMMAND_REQUIREMENT_INFO>::iterator it = this->m_CommandRequirementInfo.begin(); it != this->m_CommandRequirementInfo.end(); it++)
	{
		if(it->Index != index)
		{
			continue;
		}

		if(it->AccountLevel != -1 && it->AccountLevel > lpObj->AccountLevel)
		{
			continue;
		}

		if(it->MinLevel != -1 && it->MinLevel > lpObj->Level)
		{
			continue;
		}

		if(it->MaxLevel != -1 && it->MaxLevel < lpObj->Level)
		{
			continue;
		}

		if(it->MinReset != -1 && it->MinReset > lpObj->Reset)
		{
			continue;
		}

		if(it->MaxReset != -1 && it->MaxReset < lpObj->Reset)
		{
			continue;
		}

		if(it->RequireClass[lpObj->Class] == 0 || it->RequireClass[lpObj->Class] > (lpObj->ChangeUp+1))
		{
			continue;
		}

		if(it->ItemIndex != -1)
		{
			gItemManager.DeleteInventoryItemCount(lpObj,it->ItemIndex,it->ItemLevel,it->ItemCount);
		}
	}
}
