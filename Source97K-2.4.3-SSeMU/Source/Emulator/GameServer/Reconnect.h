// Update 89 2.4.7 -> 97K - Sistema de Reconexão (Fase 1: Persistência no DataServer)
// Reconnect.h: interface for the CReconnect class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"
#include "User.h"

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_RECONNECT_INFO_INSERT_RECV
{
	PSWMSG_HEAD header; // C1:C0:00
	char name[11];
	WORD ServerCode;
	DWORD ReconnectTime;
	BYTE data[400];
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_RECONNECT_INFO_INSERT_SEND
{
	PSWMSG_HEAD header; // C1:C0:00
	char name[11];
	WORD ServerCode;
	DWORD ReconnectTime;
	BYTE data[400];
};

struct SDHP_RECONNECT_INFO_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:C0:01
	char name[11];
	WORD ServerCode;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct RECONNECT_INFO
{
	char Account[11];
	// Update 92 2.5.0 -> 97K - Armazenamento de senha para reconexao offline
	char Password[11];
	char Name[11];
	char IpAddr[16];
	char HardwareId[45];
	int ReconnectTime;
	int PartyNumber;
	int AutoAddPointCount;
	int AutoAddPointStats[5];
	int AutoResetEnable;
	int AutoResetStats[5];
	int RequestOption;
	char AutoPartyPassword[11];
	ATTACK_STRUCT Attack;
	PICK_STRUCT Pick;
};

// Update 90 2.4.8 -> 97K - Rastreamento e restauracao de eventos locais (DS e BC)
struct RECONNECT_LOCAL_INFO
{
	char Name[11];
	int EventTime;
	BYTE EventFlag;
	BYTE EventLevel;
};

class CReconnect
{
public:
	CReconnect();
	virtual ~CReconnect();
	void MainProc();
	void SetReconnectInfo(LPOBJ lpObj);
	void GetReconnectInfo(LPOBJ lpObj);
	bool CheckResumeParty(RECONNECT_INFO* lpInfo);
	void ResumeParty(LPOBJ lpObj,RECONNECT_INFO* lpInfo);
	void ResumeCommand(LPOBJ lpObj,RECONNECT_INFO* lpInfo);
	void ResumeAttack(LPOBJ lpObj,RECONNECT_INFO* lpInfo);
	void ResumePick(LPOBJ lpObj,RECONNECT_INFO* lpInfo);
	void ResumeLocalEvent(LPOBJ lpObj,RECONNECT_LOCAL_INFO* lpInfo);
	// Update 92 2.5.0 -> 97K - Restauracao e reconexao automatica de contas em modo offline
	void ResumeOffline(RECONNECT_INFO* lpInfo);
	void DGReconnectInfoInsertRecv(SDHP_RECONNECT_INFO_INSERT_RECV* lpMsg);
	void GDReconnectInfoInsertSend(RECONNECT_INFO* lpInfo);
	void GDReconnectInfoRemoveSend(char* Name);
private:
	std::map<std::string,RECONNECT_INFO> m_ReconnectInfo;
	std::map<std::string,RECONNECT_LOCAL_INFO> m_ReconnectLocalInfo;
};

extern CReconnect gReconnect;
