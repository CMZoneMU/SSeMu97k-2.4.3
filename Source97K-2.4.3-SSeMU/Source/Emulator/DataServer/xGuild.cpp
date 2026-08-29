// Guild.cpp: implementation of the CGuild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Guild.h"
#include "CharacterManager.h"
#include "GuildManager.h"
#include "QueryManager.h"
#include "ServerManager.h"
#include "SocketManager.h"
#include "Util.h"

CGuild gGuild;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuild::CGuild() // OK
{

}

CGuild::~CGuild() // OK
{

}

void CGuild::GDGuildCreateRequestRecv(SDHP_GUILD_CREATE_RECV* lpMsg,int aIndex) // OK
{
	SDHP_GUILD_CREATE_SEND pMsg;

	pMsg.header.set(0xA0,0x00,sizeof(pMsg));

	pMsg.Index = 0;

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Mark,lpMsg->Mark,sizeof(pMsg.Mark));

	memcpy(pMsg.Master,lpMsg->Master,sizeof(pMsg.Master));

	if((pMsg.Result=gGuildManager.CreateRequest(lpMsg->Name,lpMsg->Master,lpMsg->Mark)) != 1)
	{
		gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
		return;
	}

	GUILD_INFO* pGuild = gGuildManager.GetGuild(lpMsg->Name);

	if(pGuild != 0)
	{
		pMsg.Index = pGuild->Index;

		this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));

		gGuildManager.SetMemberServer(lpMsg->Name,lpMsg->Master,GetServerCodeByName(lpMsg->Master));

		this->DGGuildInfoSend(lpMsg->Name);

		this->DGGuildMemberInfoSend(lpMsg->Name,lpMsg->Master,GetServerCodeByName(lpMsg->Master));
	}
}

void CGuild::GDGuildRemoveRequestRecv(SDHP_GUILD_REMOVE_RECV* lpMsg,int aIndex) // OK
{
	SDHP_GUILD_REMOVE_SEND pMsg;

	pMsg.header.set(0xA0,0x01,sizeof(pMsg));

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,lpMsg->Member,sizeof(pMsg.Member));

	if((pMsg.Result=gGuildManager.RemoveRequest(lpMsg->Name)) != 1)
	{
		gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
		return;
	}

	this->DataSendToAll((BYTE*)&pMsg, sizeof(pMsg));
}

void CGuild::GDGuildMemberAddRequestRecv(SDHP_GUILD_MEMBER_ADD_RECV* lpMsg,int aIndex) // OK
{
	SDHP_GUILD_MEMBER_ADD_SEND pMsg;

	pMsg.header.set(0xA0,0x02,sizeof(pMsg));

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,lpMsg->Member,sizeof(pMsg.Member));

	if((pMsg.Result =gGuildManager.AddMember(lpMsg->Name,lpMsg->Member,GetServerCodeByName(lpMsg->Member))) != 1)
	{
		gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
		return;
	}

	this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
}

void CGuild::GDGuildMemberDelRequestRecv(SDHP_GUILD_MEMBER_DEL_RECV* lpMsg,int aIndex) // OK
{
	SDHP_GUILD_MEMBER_DEL_SEND pMsg;

	pMsg.header.set(0xA0,0x03,sizeof(pMsg));

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,lpMsg->Member,sizeof(pMsg.Member));

	if((pMsg.Result=gGuildManager.DelMember(lpMsg->Name,lpMsg->Member)) != 1)
	{
		gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
		return;
	}

	this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
}

void CGuild::GDGuildScoreUpdateRecv(SDHP_GUILD_SCORE_UPDATE_RECV* lpMsg) // OK
{
	if(gGuildManager.SetGuildScore(lpMsg->Name,lpMsg->Score) != 0)
	{
		SDHP_GUILD_SCORE_UPDATE_SEND pMsg;

		pMsg.header.set(0xA0,0x05,sizeof(pMsg));

		memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

		pMsg.Score = lpMsg->Score;

		this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
	}
}

void CGuild::GDGuildMemberStatusRecv(SDHP_GUILD_MEMBER_STATUS_RECV* lpMsg,int aIndex) // OK
{
	GUILD_INFO* pGuild = gGuildManager.FindGuild(lpMsg->Member);

	if(pGuild != 0)
	{
		gGuildManager.SetMemberServer(pGuild->Name,lpMsg->Member,GetServerCodeByName(lpMsg->Member));

		this->DGGuildInfoSend(pGuild->Name);

		this->DGGuildMemberInfoSend(pGuild->Name,lpMsg->Member,GetServerCodeByName(lpMsg->Member));
	}
}

void CGuild::GDGuildGlobalChatRecv(SDHP_GUILD_GLOBAL_CHAT_RECV* lpMsg) // OK
{
	GUILD_INFO* pGuild = gGuildManager.GetGuild(lpMsg->Name);

	if(pGuild == 0)
	{
		return;
	}

	SDHP_GUILD_GLOBAL_CHAT_SEND pMsg;

	pMsg.header.set(0xA0,0x06,sizeof(pMsg));

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,lpMsg->Member,sizeof(pMsg.Member));

	memcpy(pMsg.message,lpMsg->message,sizeof(pMsg.message));

	this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
}

void CGuild::GDGuildNoticeUpdateRecv(SDHP_GUILD_NOTICE_UPDATE_RECV* lpMsg) // OK
{
	GUILD_INFO* pGuild = gGuildManager.GetGuild(lpMsg->Name);

	if(pGuild == 0)
	{
		return;
	}

	gQueryManager.BindParameterAsString(1,lpMsg->notice,sizeof(lpMsg->notice));

	if(gQueryManager.ExecQuery("UPDATE Guild SET G_Notice=? WHERE G_Name='%s'",lpMsg->Name) != 0)
	{
		memcpy(pGuild->Notice,lpMsg->notice,sizeof(pGuild->Notice));

		SDHP_GUILD_NOTICE_UPDATE_SEND pMsg;

		pMsg.header.set(0xA0,0x07,sizeof(pMsg));

		memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

		memcpy(pMsg.Member,lpMsg->Member,sizeof(pMsg.Member));

		memcpy(pMsg.notice,lpMsg->notice,sizeof(pMsg.notice));

		this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
	}

	gQueryManager.Close();
}

void CGuild::DGGuildInfoSend(char* name) // OK
{
	GUILD_INFO* pGuild = gGuildManager.GetGuild(name);

	if(pGuild == 0)
	{
		return;
	}

	BYTE send[2048];

	SDHP_GUILD_INFO_SEND pMsg;

	pMsg.header.set(0xA0,0x30,0);

	int size = sizeof(pMsg);

	pMsg.Index = pGuild->Index;

	memcpy(pMsg.Name,pGuild->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Mark,pGuild->Mark,sizeof(pMsg.Mark));

	memcpy(pMsg.Master,pGuild->Master,sizeof(pMsg.Master));

	pMsg.Score = pGuild->Score;

	pMsg.count = 0;

	SDHP_GUILD_INFO info;

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(pGuild->GuildMember[n].IsEmpty() == 0)
		{
			memcpy(info.Name,pGuild->GuildMember[n].Name,sizeof(info.Name));

			info.Server = pGuild->GuildMember[n].Server;

			memcpy(&send[size],&info,sizeof(info));

			size += sizeof(info);

			pMsg.count++;
		}
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));

	this->DataSendToAll(send,size);
}

void CGuild::DGGuildMemberInfoSend(char* name,char* member,int server) // OK
{
	SDHP_GUILD_MEMBER_STATUS_SEND pMsg;

	pMsg.header.set(0xA0,0x04,sizeof(pMsg));

	memcpy(pMsg.Name,name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,member,sizeof(pMsg.Member));

	pMsg.Server = server;

	this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
}

void CGuild::DataSendToAll(BYTE* lpMsg,int size) // OK
{
	for(int n=0;n < MAX_SERVER;n++)
	{
		if(gServerManager[n].CheckState() != 0)
		{
			gSocketManager.DataSend(n,lpMsg,size);
		}
	}
}