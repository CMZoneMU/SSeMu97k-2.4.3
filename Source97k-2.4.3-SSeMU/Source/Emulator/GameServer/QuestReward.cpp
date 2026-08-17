// QuestReward.cpp: implementation of the CQuestReward class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestReward.h"
#include "EffectManager.h"
#include "DSProtocol.h"
#include "MemScript.h"
#include "ObjectManager.h"
#include "Quest.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "Util.h"

CQuestReward gQuestReward;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestReward::CQuestReward() // OK
{
	this->m_QuestRewardInfo.clear();
}

CQuestReward::~CQuestReward() // OK
{

}

void CQuestReward::Load(char* path) // OK
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

	this->m_QuestRewardInfo.clear();

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

			QUEST_REWARD_INFO info;

			info.Sort = lpMemScript->GetNumber();

			info.Type = lpMemScript->GetAsNumber();

			info.Index = lpMemScript->GetAsNumber();

			info.Quantity = lpMemScript->GetAsNumber();

			info.Level = lpMemScript->GetAsNumber();

			info.Option1 = lpMemScript->GetAsNumber();

			info.Option2 = lpMemScript->GetAsNumber();

			info.Option3 = lpMemScript->GetAsNumber();

			info.NewOption = lpMemScript->GetAsNumber();

			info.RequireIndex = lpMemScript->GetAsNumber();

			info.RequireState = lpMemScript->GetAsNumber();

			for(int n=0;n < MAX_CLASS;n++){info.RequireClass[n] = lpMemScript->GetAsNumber();}

			this->m_QuestRewardInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CQuestReward::CheckQuestRewardRequisite(LPOBJ lpObj,QUEST_REWARD_INFO* lpInfo) // OK
{
	if(lpInfo->RequireIndex != -1 && gQuest.CheckQuestListState(lpObj,lpInfo->RequireIndex,lpInfo->RequireState) == 0)
	{
		return 0;
	}

	if(lpInfo->RequireClass[lpObj->Class] == 0 || lpInfo->RequireClass[lpObj->Class] > (lpObj->ChangeUp+1))
	{
		return 0;
	}

	return 1;
}

void CQuestReward::InsertQuestReward(LPOBJ lpObj,int QuestIndex) // OK
{
	for each(QUEST_REWARD_INFO lpInfo in this->m_QuestRewardInfo)
	{
		if(lpInfo.RequireIndex != QuestIndex)
		{
			continue;
		}

		if(this->CheckQuestRewardRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		if(lpInfo.Type == QUEST_REWARD_POINT)
		{
			lpObj->LevelUpPoint += lpInfo.Quantity;
			gQuest.GCQuestRewardSend(lpObj->Index,lpInfo.Index,lpInfo.Quantity);
			continue;
		}

		if(lpInfo.Type == QUEST_REWARD_CHANGE1)
		{
			if((lpObj->DBClass%16) < 1)
			{
				lpObj->DBClass = ((lpObj->DBClass/16)*16)+1;
				lpObj->ChangeUp = lpObj->DBClass%16;
			}

			gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

			BYTE Class = (lpObj->ChangeUp*16);
			Class -= (Class/32);
			Class += (lpObj->Class*32);

			gQuest.GCQuestRewardSend(lpObj->Index,lpInfo.Index,Class);
			continue;
		}

		if(lpInfo.Type == QUEST_REWARD_HERO)
		{
			int AddPoint = ((lpObj->Level>gServerInfo.m_PlusStatMinLevel)?(lpObj->Level- gServerInfo.m_PlusStatMinLevel):0)*gServerInfo.m_PlusStatPoint;

			lpObj->LevelUpPoint += AddPoint;

			gQuest.GCQuestRewardSend(lpObj->Index,lpInfo.Index,AddPoint);
			continue;
		}

		if(lpInfo.Type == QUEST_REWARD_COMBO)
		{
			gQuest.GCQuestRewardSend(lpObj->Index,lpInfo.Index,lpInfo.Quantity);
			continue;
		}
	}
}

long CQuestReward::GetQuestRewardPoint(LPOBJ lpObj,int QuestIndex) // OK
{
	long point = 0;

	for each(QUEST_REWARD_INFO lpInfo in this->m_QuestRewardInfo)
	{
		if(lpInfo.RequireIndex != QuestIndex)
		{
			continue;
		}

		if(lpInfo.Type == QUEST_REWARD_POINT)
		{
			point += lpInfo.Quantity;
			continue;
		}
	}

	return point;
}
