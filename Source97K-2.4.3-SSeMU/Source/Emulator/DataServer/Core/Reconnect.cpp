// Update 89 2.4.7 -> 97K - Sistema de Reconexão (Fase 1: Persistência no DataServer)
// Reconnect.cpp: implementation of the CReconnect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Reconnect.h"
#include "QueryManager.h"
#include "SocketManager.h"

CReconnect gReconnect;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReconnect::CReconnect()
{

}

CReconnect::~CReconnect()
{

}

void CReconnect::GDReconnectInfoInsertRecv(SDHP_RECONNECT_INFO_INSERT_RECV* lpMsg)
{
	if(gQueryManager.ExecQuery("SELECT Name FROM ReconnectData WHERE Name='%s'",lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.BindParameterAsBinary(1,lpMsg->data,sizeof(lpMsg->data));
		gQueryManager.ExecQuery("INSERT INTO ReconnectData (Name,ServerCode,Data,Time) VALUES ('%s','%d',?,'%d')",lpMsg->name,lpMsg->ServerCode,lpMsg->ReconnectTime);
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
		gQueryManager.BindParameterAsBinary(1,lpMsg->data,sizeof(lpMsg->data));
		gQueryManager.ExecQuery("UPDATE ReconnectData SET Data=? WHERE Name='%s'",lpMsg->name);
		gQueryManager.Close();
	}
}

void CReconnect::GDReconnectInfoDeleteRecv(SDHP_RECONNECT_INFO_DELETE_RECV* lpMsg)
{
	gQueryManager.ExecQuery("DELETE FROM ReconnectData WHERE Name='%s' AND ServerCode='%d'",lpMsg->name,lpMsg->ServerCode);

	gQueryManager.Close();
}

void CReconnect::ReconnectInfoListSend(int ServerCode,int index)
{
	if(gQueryManager.ExecQuery("SELECT * FROM ReconnectData WHERE ServerCode='%d'",ServerCode) != 0)
	{
		while(gQueryManager.Fetch() != SQL_NO_DATA)
		{
			SDHP_RECONNECT_INFO_INSERT_SEND pMsg;

			pMsg.header.set(0xC0,0x00,sizeof(pMsg));

			gQueryManager.GetAsString("Name",pMsg.name,sizeof(pMsg.name));

			pMsg.ServerCode = ServerCode;

			pMsg.ReconnectTime = gQueryManager.GetAsInteger("Time");

			gQueryManager.GetAsBinary("Data",pMsg.data,sizeof(pMsg.data));

			gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
		}
	}

	gQueryManager.Close();
}
