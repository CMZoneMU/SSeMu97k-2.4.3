// Reconnect.cpp: implementation of the CReconnect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Reconnect.h"
#include "CustomAttack.h"
#include "Party.h"
#include "ServerInfo.h"

CReconnect gReconnect;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReconnect::CReconnect() // OK
{

}

CReconnect::~CReconnect() // OK
{

}

void CReconnect::MainProc() // OK
{
	for(std::map<std::string,RECONNECT_INFO>::iterator it=this->m_ReconnectInfo.begin();it != this->m_ReconnectInfo.end();)
	{
		if((GetTickCount()-it->second.ReconnectTime) > (((DWORD)gServerInfo.m_PartyReconnectTime)*1000))
		{
			it = this->m_ReconnectInfo.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void CReconnect::SetReconnectInfo(LPOBJ lpObj) // OK
{
	RECONNECT_INFO info;

	memcpy(info.Name,lpObj->Name,sizeof(info.Name));

	info.ReconnectTime = GetTickCount();

	info.PartyNumber = lpObj->PartyNumber;

	info.AutoAddPointCount = lpObj->AutoAddPointCount;

	memcpy(info.AutoAddPointStats,lpObj->AutoAddPointStats,sizeof(info.AutoAddPointStats));

	info.AutoResetEnable = lpObj->AutoResetEnable;

	memcpy(info.AutoResetStats,lpObj->AutoResetStats,sizeof(info.AutoResetStats));

	info.RequestOption = lpObj->Option;

	memcpy(info.AutoPartyPassword,lpObj->AutoPartyPassword,sizeof(info.AutoPartyPassword));

	info.Attack.Started = lpObj->Attack.Started;
	
	info.Attack.AutoBuff = lpObj->Attack.AutoBuff;
	
	info.Attack.SkillIndex = lpObj->Attack.SkillIndex;
	
	info.Attack.StartMap = lpObj->Attack.StartMap;
	
	info.Attack.StartX = lpObj->Attack.StartX;
	
	info.Attack.StartY = lpObj->Attack.StartY;
	
	info.Attack.AttackTime = lpObj->Attack.AttackTime;
	
	info.Attack.PotionTime = lpObj->Attack.PotionTime;
	
	info.Attack.OnlineTime = lpObj->Attack.OnlineTime;

	info.Pick.Started = lpObj->Pick.Started;
	
	info.Pick.ItemCount = lpObj->Pick.ItemCount;

	memcpy(info.Pick.ItemList,lpObj->Pick.ItemList,sizeof(info.Pick.ItemList));

	info.Pick.PickJewel = lpObj->Pick.PickJewel;

	info.Pick.PickSet = lpObj->Pick.PickSet;

	info.Pick.PickExc = lpObj->Pick.PickExc;

	info.Pick.PickMoney = lpObj->Pick.PickMoney;

	info.Pick.OnlineTime = lpObj->Pick.OnlineTime;

	std::map<std::string,RECONNECT_INFO>::iterator it = this->m_ReconnectInfo.find(std::string(lpObj->Name));

	if(it == this->m_ReconnectInfo.end())
	{
		this->m_ReconnectInfo.insert(std::pair<std::string,RECONNECT_INFO>(std::string(lpObj->Name),info));
	}
	else
	{
		it->second = info;
	}
}

void CReconnect::GetReconnectInfo(LPOBJ lpObj) // OK
{
	std::map<std::string,RECONNECT_INFO>::iterator it = this->m_ReconnectInfo.find(std::string(lpObj->Name));

	if(it != this->m_ReconnectInfo.end())
	{
		this->ResumeParty(lpObj,&it->second);

		this->ResumeCommand(lpObj,&it->second);

		this->ResumeAttack(lpObj,&it->second);
		
		this->ResumePick(lpObj,&it->second);

		this->m_ReconnectInfo.erase(it);
	}
}

bool CReconnect::CheckResumeParty(RECONNECT_INFO* lpInfo) // OK
{
	int count = 0;

	if(OBJECT_RANGE(lpInfo->PartyNumber) != 0)
	{
		for(std::map<std::string,RECONNECT_INFO>::iterator it=this->m_ReconnectInfo.begin();it != this->m_ReconnectInfo.end();it++)
		{
			if(it->second.PartyNumber == lpInfo->PartyNumber)
			{
				count++;
			}
		}
	}

	return ((count>1)?1:gParty.IsParty(lpInfo->PartyNumber));
}

void CReconnect::ResumeParty(LPOBJ lpObj,RECONNECT_INFO* lpInfo) // OK
{
	if(this->CheckResumeParty(lpInfo) != 0)
	{
		if(gParty.IsParty(lpInfo->PartyNumber) == 0)
		{
			lpObj->PartyNumber = lpInfo->PartyNumber;
			gParty.m_PartyInfo[lpInfo->PartyNumber].Count = 1;
			gParty.m_PartyInfo[lpInfo->PartyNumber].Index[0] = lpObj->Index;
			gParty.m_PartyInfo[lpInfo->PartyNumber].Index[1] = -1;
			gParty.m_PartyInfo[lpInfo->PartyNumber].Index[2] = -1;
			gParty.m_PartyInfo[lpInfo->PartyNumber].Index[3] = -1;
			gParty.m_PartyInfo[lpInfo->PartyNumber].Index[4] = -1;
			gParty.GCPartyListSend(lpInfo->PartyNumber);
		}
		else
		{
			gParty.AddMember(lpInfo->PartyNumber,lpObj->Index);
		}
	}
}

void CReconnect::ResumeCommand(LPOBJ lpObj,RECONNECT_INFO* lpInfo) // OK
{
	lpObj->AutoAddPointCount = lpInfo->AutoAddPointCount;

	memcpy(lpObj->AutoAddPointStats,lpInfo->AutoAddPointStats,sizeof(lpObj->AutoAddPointStats));

	lpObj->AutoResetEnable = lpInfo->AutoResetEnable;

	memcpy(lpObj->AutoResetStats,lpInfo->AutoResetStats,sizeof(lpObj->AutoResetStats));

	lpObj->Option = lpInfo->RequestOption;

	memcpy(lpObj->AutoPartyPassword,lpInfo->AutoPartyPassword,sizeof(lpObj->AutoPartyPassword));
}

void CReconnect::ResumeAttack(LPOBJ lpObj,RECONNECT_INFO* lpInfo) // OK
{
	lpObj->Attack.Started = lpInfo->Attack.Started;
	
	lpObj->Attack.AutoBuff = lpInfo->Attack.AutoBuff;
	
	lpObj->Attack.SkillIndex = lpInfo->Attack.SkillIndex;
	
	lpObj->Attack.StartMap = lpInfo->Attack.StartMap;
	
	lpObj->Attack.StartX = lpInfo->Attack.StartX;
	
	lpObj->Attack.StartY = lpInfo->Attack.StartY;
	
	lpObj->Attack.AttackTime = lpInfo->Attack.AttackTime;
	
	lpObj->Attack.PotionTime = lpInfo->Attack.PotionTime;
	
	lpObj->Attack.OnlineTime = lpInfo->Attack.OnlineTime;

	gCustomAttack.CustomAttackSend(lpObj->Index);
}

void CReconnect::ResumePick(LPOBJ lpObj,RECONNECT_INFO* lpInfo) // OK
{
	lpObj->Pick.Started = lpInfo->Pick.Started;
	
	lpObj->Pick.ItemCount = lpInfo->Pick.ItemCount;

	memcpy(lpObj->Pick.ItemList,lpInfo->Pick.ItemList,sizeof(lpObj->Pick.ItemList));

	lpObj->Pick.PickJewel = lpInfo->Pick.PickJewel;

	lpObj->Pick.PickSet = lpInfo->Pick.PickSet;

	lpObj->Pick.PickExc = lpInfo->Pick.PickExc;

	lpObj->Pick.PickMoney = lpInfo->Pick.PickMoney;

	lpObj->Pick.OnlineTime = lpInfo->Pick.OnlineTime;
}