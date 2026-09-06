// Update 89 2.4.7 -> 97K - Sistema de Reconexão (Fase 1: Persistência no DataServer)
// Reconnect.h: interface for the CReconnect class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DataServerProtocol.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_RECONNECT_INFO_INSERT_RECV
{
	PSWMSG_HEAD header; // C1:C0:00
	char name[11];
	WORD ServerCode;
	DWORD ReconnectTime;
	BYTE data[400];
};

struct SDHP_RECONNECT_INFO_DELETE_RECV
{
	PSBMSG_HEAD header; // C1:C0:01
	char name[11];
	WORD ServerCode;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_RECONNECT_INFO_INSERT_SEND
{
	PSWMSG_HEAD header; // C1:C0:00
	char name[11];
	WORD ServerCode;
	DWORD ReconnectTime;
	BYTE data[400];
};

//**********************************************//
//**********************************************//
//**********************************************//

class CReconnect
{
public:
	CReconnect();
	virtual ~CReconnect();
	void GDReconnectInfoInsertRecv(SDHP_RECONNECT_INFO_INSERT_RECV* lpMsg);
	void GDReconnectInfoDeleteRecv(SDHP_RECONNECT_INFO_DELETE_RECV* lpMsg);
	void ReconnectInfoListSend(int ServerCode,int index);
};

extern CReconnect gReconnect;
