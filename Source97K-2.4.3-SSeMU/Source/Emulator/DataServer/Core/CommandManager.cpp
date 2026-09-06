// CommandManager.cpp: implementation of the CCommandManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandManager.h"
#include "BadSyntax.h"
#include "QueryManager.h"
#include "SocketManager.h"
#include "Util.h"

CCommandManager gCommandManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandManager::CCommandManager() // OK
{

}

CCommandManager::~CCommandManager() // OK
{

}

void CCommandManager::GDCommandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg,int index) // OK
{
	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x0F,0x00,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	gQueryManager.ExecQuery("EXEC WZ_GetResetInfo '%s','%s'",lpMsg->account,lpMsg->name);

	gQueryManager.Fetch();

	pMsg.ResetDay = gQueryManager.GetAsInteger("ResetDay");

	pMsg.ResetWek = gQueryManager.GetAsInteger("ResetWek");

	pMsg.ResetMon = gQueryManager.GetAsInteger("ResetMon");

	gQueryManager.Close();

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
}

void CCommandManager::GDCommandMasterResetRecv(SDHP_COMMAND_MASTER_RESET_RECV* lpMsg,int index) // OK
{
	SDHP_COMMAND_MASTER_RESET_SEND pMsg;

	pMsg.header.set(0x0F,0x01,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	gQueryManager.ExecQuery("EXEC WZ_GetMasterResetInfo '%s','%s'",lpMsg->account,lpMsg->name);

	gQueryManager.Fetch();

	pMsg.MasterResetDay = gQueryManager.GetAsInteger("MasterResetDay");

	pMsg.MasterResetWek = gQueryManager.GetAsInteger("MasterResetWek");

	pMsg.MasterResetMon = gQueryManager.GetAsInteger("MasterResetMon");

	gQueryManager.Close();

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
}

void CCommandManager::GDCommandDivorceRecv(SDHP_COMMAND_DIVORCE_RECV* lpMsg, int index) // OK
{
	gQueryManager.ExecQuery("UPDATE Character SET Married=0, MarryCouple=NULL WHERE Name='%s'", lpMsg->name);

	gQueryManager.Close();
}

void CCommandManager::GDCommandRenameRecv(SDHP_COMMAND_RENAME_RECV* lpMsg, int index) // OK
{
	SDHP_COMMAND_RENAME_SEND pMsg;

	pMsg.header.set(0x0F,0x04,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	pMsg.result = 0;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	memcpy(pMsg.newname,lpMsg->newname,sizeof(pMsg.newname));

	if(CheckTextSyntax(lpMsg->newname,sizeof(lpMsg->newname)) == 0)
	{
		pMsg.result = 2;
	}

	if(pMsg.result == 0)
	{
		gQueryManager.BindParameterAsString(1,lpMsg->newname,sizeof(lpMsg->newname));

		if(gQueryManager.ExecQuery("EXEC WZ_RenameCharacter '%s','%s',?",lpMsg->account,lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();
		}
		else
		{
			pMsg.result = gQueryManager.GetResult(0);

			gQueryManager.Close();
		}
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
}

void CCommandManager::GDCommandBanAccountRecv(SDHP_COMMAND_BAN_ACCOUNT_RECV* lpMsg, int index) // OK
{
	SDHP_COMMAND_BAN_ACCOUNT_SEND pMsg;

	pMsg.header.set(0x0F,0x05,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	pMsg.BlocCode = lpMsg->BlocCode;

	if(gQueryManager.ExecQuery("EXEC WZ_BanAccount '%s','%d'",lpMsg->account,lpMsg->BlocCode) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		pMsg.result = 0;

		gQueryManager.Close();
	}
	else
	{
		pMsg.result = gQueryManager.GetResult(0);

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
}

void CCommandManager::GDCommandBanCharacterRecv(SDHP_COMMAND_BAN_CHARACTER_RECV* lpMsg, int index) // OK
{
	SDHP_COMMAND_BAN_CHARACTER_SEND pMsg;

	pMsg.header.set(0x0F,0x06,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	pMsg.CtlCode = lpMsg->CtlCode;

	if(gQueryManager.ExecQuery("EXEC WZ_BanCharacter '%s','%d'",lpMsg->name,lpMsg->CtlCode) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		pMsg.result = 0;

		gQueryManager.Close();
	}
	else
	{
		pMsg.result = gQueryManager.GetResult(0);

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
}