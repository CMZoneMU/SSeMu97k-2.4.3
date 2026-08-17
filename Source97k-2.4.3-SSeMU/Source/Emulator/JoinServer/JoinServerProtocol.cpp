#include "stdafx.h"
#include "JoinServerProtocol.h"
#include "MD5.h"
#include "AccountManager.h"
#include "Log.h"
#include "QueryManager.h"
#include "ServerManager.h"
#include "SocketManager.h"
#include "Util.h"

void JoinServerProtocolCore(int index,BYTE head,BYTE* lpMsg,int size) // OK
{
	switch(head)
	{
		case 0x00:
			GJServerInfoRecv((SDHP_SERVER_INFO_RECV*)lpMsg,index);
			break;
		case 0x01:
			GJConnectAccountRecv((SDHP_CONNECT_ACCOUNT_RECV*)lpMsg,index);
			break;
		case 0x02:
			GJDisconnectAccountRecv((SDHP_DISCONNECT_ACCOUNT_RECV*)lpMsg,index);
			break;
		case 0x05:
			GJAccountLevelRecv((SDHP_ACCOUNT_LEVEL_RECV*)lpMsg,index);
			break;
		case 0x11:
			GJAccountLevelSaveRecv((SDHP_ACCOUNT_LEVEL_SAVE_RECV*)lpMsg,index);
			break;
		case 0x20:
			GJServerUserInfoRecv((SDHP_SERVER_USER_INFO_RECV*)lpMsg,index);
			break;
		case 0x30:
			GJExternalDisconnectAccountRecv((SDHP_EXTERNAL_DISCONNECT_ACCOUNT_RECV*)lpMsg,index);
			break;
	}
}

void GJServerInfoRecv(SDHP_SERVER_INFO_RECV* lpMsg,int index) // OK
{
	gServerManager[index].SetServerInfo(lpMsg->ServerName,lpMsg->ServerPort,lpMsg->ServerCode);
}

void GJConnectAccountRecv(SDHP_CONNECT_ACCOUNT_RECV* lpMsg,int index) // OK
{
	SDHP_CONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.set(0x01,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	pMsg.result = 1;

	if(gAccountManager.CheckConnectionLimit(lpMsg->IpAddress,lpMsg->HardwareId) == 0)
	{
		pMsg.result = 7;
		gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(CheckTextSyntax(lpMsg->account,sizeof(lpMsg->account)) == 0)
	{
		pMsg.result = 2;
		gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	ACCOUNT_INFO AccountInfo;

	if(gAccountManager.GetAccountInfo(&AccountInfo,lpMsg->account) != 0)
	{
		pMsg.result = 3;
		gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
		
		if(strcmp(AccountInfo.Password,lpMsg->password) == 0)
		{
			JGAccountAlreadyConnectedSend(AccountInfo.GameServerCode,AccountInfo.UserIndex,AccountInfo.Account);
		}

		return;
	}

	if(gAccountManager.GetAccountCount() >= gJoinServerMaxAccount[4])
	{
		pMsg.result = 4;
		gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(MD5Encryption == 0)
	{
		if(gQueryManager.ExecQuery("SELECT memb___id,memb__pwd FROM MEMB_INFO WHERE memb___id='%s'",lpMsg->account) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();
			pMsg.result = 2;
			gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
			return;
		}

		char account[11],password[11] = {0};

		gQueryManager.GetAsString("memb___id", account, sizeof(account));

		if (strcmp(lpMsg->account, account) != 0)
		{
			gQueryManager.Close();
			pMsg.result = 2;
			gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
			return;
		}

		gQueryManager.GetAsString("memb__pwd",password,sizeof(password));

		if(strcmp(lpMsg->password,password) != 0)
		{
			gQueryManager.Close();
			pMsg.result = 0;
			gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
			return;
		}

		gQueryManager.Close();
	}
	else
	{
		if(gQueryManager.ExecQuery("SELECT memb___id,memb__pwd FROM MEMB_INFO WHERE memb___id='%s'",lpMsg->account) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();
			pMsg.result = 2;
			gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
			return;
		}

		char account[11] = { 0 };

		gQueryManager.GetAsString("memb___id", account, sizeof(account));

		if(strcmp(lpMsg->account, account) != 0)
		{
			gQueryManager.Close();
			pMsg.result = 2;
			gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
			return;
		}

		BYTE password[16] = {0};

		gQueryManager.GetAsBinary("memb__pwd",password,sizeof(password));

		MD5 MD5Hash;

		if(MD5Hash.MD5_CheckValue(lpMsg->password,(char*)password,MakeAccountKey(lpMsg->account)) == 0)
		{
			gQueryManager.Close();
			pMsg.result = 0;
			gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
			return;
		}

		gQueryManager.Close();
	}

	if(gQueryManager.ExecQuery("SELECT sno__numb,bloc_code FROM MEMB_INFO WHERE memb___id='%s'",lpMsg->account) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		pMsg.result = 2;
		gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	gQueryManager.GetAsString("sno__numb",pMsg.PersonalCode,sizeof(pMsg.PersonalCode));

	pMsg.BlockCode = (BYTE)gQueryManager.GetAsInteger("bloc_code");

	gQueryManager.Close();

	if(gQueryManager.ExecQuery("EXEC WZ_GetAccountLevel '%s'",lpMsg->account) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		pMsg.result = 2;
		gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	pMsg.AccountLevel = gQueryManager.GetAsInteger("AccountLevel");

	gQueryManager.GetAsString("AccountExpireDate",pMsg.AccountExpireDate,sizeof(pMsg.AccountExpireDate));

	gQueryManager.Close();

	gQueryManager.ExecQuery("EXEC WZ_CONNECT_MEMB '%s','%s','%s'",lpMsg->account,gServerManager[index].m_ServerName,lpMsg->IpAddress);

	gQueryManager.Close();

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);

	strcpy_s(AccountInfo.Account,lpMsg->account);

	strcpy_s(AccountInfo.Password,lpMsg->password);

	strcpy_s(AccountInfo.IpAddress,lpMsg->IpAddress);

	strcpy_s(AccountInfo.HardwareId,lpMsg->HardwareId);

	AccountInfo.UserIndex = lpMsg->index;
	AccountInfo.GameServerCode = gServerManager[index].m_ServerCode;

	gAccountManager.InsertAccountInfo(AccountInfo);

	gLog.Output(LOG_ACCOUNT,"[AccountInfo] Account connected (Account: %s, IpAddress: %s, GameServerCode: %d)",AccountInfo.Account,AccountInfo.IpAddress,AccountInfo.GameServerCode);
}

void GJDisconnectAccountRecv(SDHP_DISCONNECT_ACCOUNT_RECV* lpMsg,int index) // OK
{
	SDHP_DISCONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.set(0x02,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	pMsg.result = 1;

	ACCOUNT_INFO AccountInfo;

	if(gAccountManager.GetAccountInfo(&AccountInfo,lpMsg->account) == 0)
	{
		pMsg.result = 0;
		gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(AccountInfo.UserIndex != lpMsg->index)
	{
		pMsg.result = 0;
		gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(AccountInfo.GameServerCode != gServerManager[index].m_ServerCode)
	{
		pMsg.result = 0;
		gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	gQueryManager.ExecQuery("EXEC WZ_DISCONNECT_MEMB '%s'",lpMsg->account);

	gQueryManager.Close();

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);

	gAccountManager.RemoveAccountInfo(AccountInfo);

	gLog.Output(LOG_ACCOUNT,"[AccountInfo] Account disconnected (Account: %s, IpAddress: %s, GameServerCode: %d)",AccountInfo.Account,AccountInfo.IpAddress,AccountInfo.GameServerCode);
}

void GJAccountLevelRecv(SDHP_ACCOUNT_LEVEL_RECV* lpMsg,int index) // OK
{
	ACCOUNT_INFO AccountInfo;

	if(gAccountManager.GetAccountInfo(&AccountInfo,lpMsg->account) == 0)
	{
		return;
	}

	SDHP_ACCOUNT_LEVEL_SEND pMsg;

	pMsg.header.set(0x05,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	if(gQueryManager.ExecQuery("EXEC WZ_GetAccountLevel '%s'",lpMsg->account) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.AccountLevel = 0;
	}
	else
	{
		pMsg.AccountLevel = gQueryManager.GetAsInteger("AccountLevel");

		gQueryManager.GetAsString("AccountExpireDate",pMsg.AccountExpireDate,sizeof(pMsg.AccountExpireDate));

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
}

void GJAccountLevelSaveRecv(SDHP_ACCOUNT_LEVEL_SAVE_RECV* lpMsg,int index) // OK
{
	gQueryManager.ExecQuery("EXEC WZ_SetAccountLevel '%s','%d','%d'",lpMsg->account,lpMsg->AccountLevel,lpMsg->AccountExpireTime);
	gQueryManager.Fetch();
	gQueryManager.Close();
}

void GJServerUserInfoRecv(SDHP_SERVER_USER_INFO_RECV* lpMsg,int index) // OK
{
	gServerManager[index].m_CurUserCount = lpMsg->CurUserCount;

	gServerManager[index].m_MaxUserCount = lpMsg->MaxUserCount;
}

void GJExternalDisconnectAccountRecv(SDHP_EXTERNAL_DISCONNECT_ACCOUNT_RECV* lpMsg,int index) // OK
{
	ACCOUNT_INFO AccountInfo;

	if(gAccountManager.GetAccountInfo(&AccountInfo,lpMsg->account) == 0)
	{
		return;
	}

	JGExternalDisconnectAccountSend(AccountInfo.GameServerCode,AccountInfo.UserIndex,AccountInfo.Account);
}

void JGExternalDisconnectAccountSend(int GameServerCode,int UserIndex,char* account) // OK
{
	CServerManager* lpServerManager = FindServerByCode(GameServerCode);

	if(lpServerManager == 0)
	{
		return;
	}

	SDHP_DISCONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.set(0x02,sizeof(pMsg));

	pMsg.index = UserIndex;

	memcpy(pMsg.account,account,sizeof(pMsg.account));

	pMsg.result = 0;

	gSocketManager.DataSend(lpServerManager->m_index,(BYTE*)&pMsg,pMsg.header.size);
}

void JGAccountAlreadyConnectedSend(int GameServerCode,int UserIndex,char* account) // OK
{
	CServerManager* lpServerManager = FindServerByCode(GameServerCode);

	if(lpServerManager == 0)
	{
		return;
	}

	SDHP_ACCOUNT_ALREADY_CONNECTED_SEND pMsg;

	pMsg.header.set(0x30,sizeof(pMsg));

	pMsg.index = UserIndex;

	memcpy(pMsg.account,account,sizeof(pMsg.account));

	gSocketManager.DataSend(lpServerManager->m_index,(BYTE*)&pMsg,pMsg.header.size);
}
