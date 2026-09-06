// Update 89 2.4.7 -> 97K - Sistema de Reconexão (Fase 1: Persistência no DataServer)
// Reconnect.cpp: implementation of the CReconnect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Reconnect.h"
#include "BloodCastle.h"
#include "CustomAttack.h"
#include "DevilSquare.h"
#include "DSProtocol.h"
#include "EventEntryLevel.h"
#include "GameMain.h"
#include "JSProtocol.h"
#include "Map.h"
#include "Party.h"
#include "ServerInfo.h"
#include "Log.h"
#include "Util.h"

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
		if((--it->second.ReconnectTime) < 0)
		{
			this->GDReconnectInfoRemoveSend(it->second.Name);

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
	// Update 92 2.5.0 -> 97K - Salvamento de dados para contas offline fora de SafeZone
	if(gServerInfo.m_ReconnectOfflineSwitch != 0)
	{
		if(lpObj->Attack.Offline != 0)
		{
			if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
			{
				return;
			}

			GDCharacterInfoSaveSend(lpObj->Index);
		}
	}

	RECONNECT_INFO info;
	memset(&info,0,sizeof(info));

	// Update 90 2.4.8 -> 97K - Aprimoramento no salvamento de dados do Reconnect
	memcpy(info.Account,lpObj->Account,sizeof(info.Account));

	// Update 92 2.5.0 -> 97K - Armazenamento de senha para reconexao offline
	memcpy(info.Password,lpObj->Password,sizeof(info.Password));

	memcpy(info.Name,lpObj->Name,sizeof(info.Name));

	memcpy(info.IpAddr,lpObj->IpAddr,sizeof(info.IpAddr));

	memcpy(info.HardwareId,lpObj->HardwareId,sizeof(info.HardwareId));

	info.ReconnectTime = gServerInfo.m_ReconnectTime;

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

	this->GDReconnectInfoInsertSend(&info);

	// Update 90 2.4.8 -> 97K - Rastreamento e salvamento de eventos locais (Devil Square e Blood Castle)
	RECONNECT_LOCAL_INFO lpInfo;

	memset(&lpInfo,0,sizeof(lpInfo));

	memcpy(lpInfo.Name,lpObj->Name,sizeof(lpInfo.Name));

	if((DS_MAP_RANGE(lpObj->Map) != 0 || BC_MAP_RANGE(lpObj->Map) != 0) && lpObj->EventFlag != 0)
	{
		lpInfo.EventTime = ((int)time(0));

		lpInfo.EventFlag = lpObj->EventFlag;

		lpInfo.EventLevel = lpObj->EventLevel;
	}

	std::map<std::string,RECONNECT_LOCAL_INFO>::iterator itl = this->m_ReconnectLocalInfo.find(std::string(lpInfo.Name));

	if(itl == this->m_ReconnectLocalInfo.end())
	{
		this->m_ReconnectLocalInfo.insert(std::pair<std::string,RECONNECT_LOCAL_INFO>(std::string(lpInfo.Name),lpInfo));
	}
	else
	{
		itl->second = lpInfo;
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

		this->GDReconnectInfoRemoveSend(it->second.Name);

		this->m_ReconnectInfo.erase(it);
	}

	// Update 90 2.4.8 -> 97K - Restauracao de evento local (Devil Square e Blood Castle)
	std::map<std::string,RECONNECT_LOCAL_INFO>::iterator lpInfo = this->m_ReconnectLocalInfo.find(std::string(lpObj->Name));

	if(lpInfo != this->m_ReconnectLocalInfo.end())
	{
		this->ResumeLocalEvent(lpObj,&lpInfo->second);

		this->m_ReconnectLocalInfo.erase(lpInfo);
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
	// Update 90 2.4.8 -> 97K - Verificacao de switch
	if(gServerInfo.m_ReconnectPartySwitch == 0)
	{
		return;
	}

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
	// Update 90 2.4.8 -> 97K - Verificacao de switch
	if(gServerInfo.m_ReconnectCommandSwitch == 0)
	{
		return;
	}

	lpObj->AutoAddPointCount = lpInfo->AutoAddPointCount;

	memcpy(lpObj->AutoAddPointStats,lpInfo->AutoAddPointStats,sizeof(lpObj->AutoAddPointStats));

	lpObj->AutoResetEnable = lpInfo->AutoResetEnable;

	memcpy(lpObj->AutoResetStats,lpInfo->AutoResetStats,sizeof(lpObj->AutoResetStats));

	lpObj->Option = lpInfo->RequestOption;

	memcpy(lpObj->AutoPartyPassword,lpInfo->AutoPartyPassword,sizeof(lpObj->AutoPartyPassword));
}

void CReconnect::ResumeAttack(LPOBJ lpObj,RECONNECT_INFO* lpInfo) // OK
{
	// Update 90 2.4.8 -> 97K - Verificacao de switch
	if(gServerInfo.m_ReconnectAttackSwitch == 0)
	{
		return;
	}

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
	// Update 90 2.4.8 -> 97K - Verificacao de switch
	if(gServerInfo.m_ReconnectPickSwitch == 0)
	{
		return;
	}

	lpObj->Pick.Started = lpInfo->Pick.Started;
	
	lpObj->Pick.ItemCount = lpInfo->Pick.ItemCount;

	memcpy(lpObj->Pick.ItemList,lpInfo->Pick.ItemList,sizeof(lpObj->Pick.ItemList));

	lpObj->Pick.PickJewel = lpInfo->Pick.PickJewel;

	lpObj->Pick.PickSet = lpInfo->Pick.PickSet;

	lpObj->Pick.PickExc = lpInfo->Pick.PickExc;

	lpObj->Pick.PickMoney = lpInfo->Pick.PickMoney;

	lpObj->Pick.OnlineTime = lpInfo->Pick.OnlineTime;
}

// Update 90 2.4.8 -> 97K - Restauracao de eventos locais (Devil Square e Blood Castle)
void CReconnect::ResumeLocalEvent(LPOBJ lpObj,RECONNECT_LOCAL_INFO* lpInfo)
{
	if(gServerInfo.m_ReconnectEventSwitch == 0)
	{
		return;
	}

	if(((int)time(0)) >= lpInfo->EventTime+gServerInfo.m_ReconnectEventMaxTime)
	{
		return;
	}

	if(lpInfo->EventFlag == 1) // Devil Square
	{
		if(DS_LEVEL_RANGE(lpInfo->EventLevel) == 0)
		{
			return;
		}

		if(lpInfo->EventLevel != gEventEntryLevel.GetDSLevel(lpObj))
		{
			return;
		}

		if(gDevilSquare.GetState(lpInfo->EventLevel) >= DS_STATE_READY && gDevilSquare.GetState(lpInfo->EventLevel) <= DS_STATE_START)
		{
			DEVIL_SQUARE_LEVEL* lpLevel = &gDevilSquare.m_DevilSquareLevel[lpInfo->EventLevel];

			if(gDevilSquare.GetUserCount(lpLevel) >= gServerInfo.m_DevilSquareMaxUser)
			{
				return;
			}

			if(gDevilSquare.AddUser(lpLevel,lpObj->Index) == 0)
			{
				return;
			}

			switch(lpInfo->EventLevel)
			{
				case 0:
					gObjMoveGate(lpObj->Index,58);
					break;
				case 1:
					gObjMoveGate(lpObj->Index,59);
					break;
				case 2:
					gObjMoveGate(lpObj->Index,60);
					break;
				case 3:
					gObjMoveGate(lpObj->Index,61);
					break;
			}
		}
	}
	else if(lpInfo->EventFlag == 2) // Blood Castle
	{
		if(BC_LEVEL_RANGE(lpInfo->EventLevel) == 0)
		{
			return;
		}

		if(lpInfo->EventLevel != gEventEntryLevel.GetBCLevel(lpObj))
		{
			return;
		}

		if(gBloodCastle.GetState(lpInfo->EventLevel) >= BC_STATE_READY && gBloodCastle.GetState(lpInfo->EventLevel) <= BC_STATE_START)
		{
			BLOOD_CASTLE_LEVEL* lpLevel = &gBloodCastle.m_BloodCastleLevel[lpInfo->EventLevel];

			if(gBloodCastle.GetUserCount(lpLevel) >= gServerInfo.m_BloodCastleMaxUser)
			{
				return;
			}

			if(gBloodCastle.AddUser(lpLevel,lpObj->Index) == 0)
			{
				return;
			}

			switch(lpInfo->EventLevel)
			{
				case 0:
					gObjMoveGate(lpObj->Index,66);
					break;
				case 1:
					gObjMoveGate(lpObj->Index,67);
					break;
				case 2:
					gObjMoveGate(lpObj->Index,68);
					break;
				case 3:
					gObjMoveGate(lpObj->Index,69);
					break;
				case 4:
					gObjMoveGate(lpObj->Index,70);
					break;
				case 5:
					gObjMoveGate(lpObj->Index,71);
					break;
			}
		}
	}
}

void CReconnect::DGReconnectInfoInsertRecv(SDHP_RECONNECT_INFO_INSERT_RECV* lpMsg)
{
	RECONNECT_INFO info = *(RECONNECT_INFO*)lpMsg->data;

	std::map<std::string,RECONNECT_INFO>::iterator it = this->m_ReconnectInfo.find(std::string(lpMsg->name));

	if(it == this->m_ReconnectInfo.end())
	{
		this->m_ReconnectInfo.insert(std::pair<std::string,RECONNECT_INFO>(std::string(lpMsg->name),info));
	}
	else
	{
		it->second = info;
	}

	// Update 92 2.5.0 -> 97K - Retomada de conexao offline ao receber informacoes de reconexao
	this->ResumeOffline(&info);
}

// Update 92 2.5.0 -> 97K - Restauracao e reconexao automatica de contas em modo offline
void CReconnect::ResumeOffline(RECONNECT_INFO* lpInfo)
{
	if(gServerInfo.m_ReconnectOfflineSwitch == 0)
	{
		return;
	}

	if(gCustomAttack.m_CustomAttackOfflineSwitch == 0)
	{
		return;
	}

	if(lpInfo->Attack.Offline == 0)
	{
		return;
	}

	if(strlen(lpInfo->Account) == 0 || strlen(lpInfo->Password) == 0 || strlen(lpInfo->Name) == 0 || strlen(lpInfo->IpAddr) == 0 || strlen(lpInfo->HardwareId) == 0)
	{
		return;
	}

	int index = gObjAddSearch(INVALID_SOCKET,lpInfo->IpAddr);

	if(OBJECT_RANGE(index) == 0)
	{
		return;
	}

	if(gObjAdd(INVALID_SOCKET,lpInfo->IpAddr,index) == -1)
	{
		return;
	}

	gObj[index].LoginMessageSend++;
	gObj[index].LoginMessageCount++;
	gObj[index].ConnectTickCount = GetTickCount();
	gObj[index].ClientTickCount = GetTickCount();
	gObj[index].ServerTickCount = GetTickCount();
	gObj[index].ClientVerify = 1;

	memcpy(gObj[index].Account,lpInfo->Account,sizeof(gObj[index].Account));
	memcpy(gObj[index].Password,lpInfo->Password,sizeof(gObj[index].Password));
	memcpy(gObj[index].HardwareId,lpInfo->HardwareId,sizeof(gObj[index].HardwareId));

	GJConnectAccountSend(index,lpInfo->Account,lpInfo->Password,lpInfo->IpAddr,lpInfo->HardwareId);

	GDCharacterInfoSend(index,lpInfo->Name);

	gLog.Output(LOG_CONNECT,"[ObjectManager][%d] Offline Reconnect [%s][%s] [%s][%s]",index,lpInfo->Account,lpInfo->Name,lpInfo->IpAddr,lpInfo->HardwareId);
	LogAdd(LOG_BLUE,"[ObjectManager][%d] Offline Reconnect [%s][%s] [%s][%s]",index,lpInfo->Account,lpInfo->Name,lpInfo->IpAddr,lpInfo->HardwareId);
}

void CReconnect::GDReconnectInfoInsertSend(RECONNECT_INFO* lpInfo)
{
	SDHP_RECONNECT_INFO_INSERT_SEND pMsg;

	pMsg.header.set(0xC0,0x00,sizeof(pMsg));

	memcpy(pMsg.name,lpInfo->Name,sizeof(pMsg.name));

	pMsg.ServerCode = gServerInfo.m_ServerCode;

	pMsg.ReconnectTime = lpInfo->ReconnectTime;

	memcpy(pMsg.data,lpInfo,sizeof(pMsg.data));

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg));
}

void CReconnect::GDReconnectInfoRemoveSend(char* Name)
{
	SDHP_RECONNECT_INFO_DELETE_SEND pMsg;

	pMsg.header.set(0xC0,0x01,sizeof(pMsg));

	memcpy(pMsg.name,Name,sizeof(pMsg.name));

	pMsg.ServerCode = gServerInfo.m_ServerCode;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}