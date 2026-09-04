// Guild.cpp: implementation of the CGuild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Guild.h"
#include "CharacterManager.h"
#include "GuildManager.h"
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

void CGuild::GDGuildCreateRecv(SDHP_GUILD_CREATE_RECV* lpMsg,int aIndex) // OK
{
	SDHP_GUILD_CREATE_SEND pMsg;

	pMsg.header.set(0xA0,0x00,sizeof(pMsg));

	pMsg.Index = 0;

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Master,lpMsg->Master,sizeof(pMsg.Master));

	memcpy(pMsg.Mark,lpMsg->Mark,sizeof(pMsg.Mark));

	if((pMsg.result=gGuildManager.CreateGuild(lpMsg->Name,lpMsg->Master,lpMsg->Mark)) != 1)
	{
		gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
		return;
	}

	GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->Name);

	if(lpGuild != 0)
	{
		pMsg.Index = lpGuild->Index;

		this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));

		this->MemberOnlineOffline(lpMsg->Master,lpMsg->Server);
	}
}

void CGuild::GDGuildDeleteRecv(SDHP_GUILD_DELETE_RECV* lpMsg,int aIndex) // OK
{
	SDHP_GUILD_DELETE_SEND pMsg;

	pMsg.header.set(0xA0,0x01,sizeof(pMsg));

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,lpMsg->Member,sizeof(pMsg.Member));

	if((pMsg.result=gGuildManager.RemoveGuild(lpMsg->Name)) != 1)
	{
		gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
		return;
	}

	this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
}

void CGuild::GDGuildMemberAddRecv(SDHP_GUILD_MEMBER_ADD_RECV* lpMsg,int aIndex) // OK
{
	SDHP_GUILD_MEMBER_ADD_SEND pMsg;

	pMsg.header.set(0xA0,0x02,sizeof(pMsg));

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,lpMsg->Member,sizeof(pMsg.Member));

	pMsg.Server = lpMsg->Server;

	if((pMsg.result=gGuildManager.AddGuildMember(lpMsg->Name,lpMsg->Member,lpMsg->Server)) != 1)
	{
		gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
		return;
	}

	this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
}

void CGuild::GDGuildMemberDelRecv(SDHP_GUILD_MEMBER_DEL_RECV* lpMsg,int aIndex) // OK
{
	SDHP_GUILD_MEMBER_DEL_SEND pMsg;

	pMsg.header.set(0xA0,0x03,sizeof(pMsg));

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,lpMsg->Member,sizeof(pMsg.Member));

	if((pMsg.result=gGuildManager.DelGuildMember(lpMsg->Member)) != 1)
	{
		gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
		return;
	}

	this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
}

void CGuild::GDGuildNoticeRecv(SDHP_GUILD_NOTICE_RECV* lpMsg) // OK
{
	if(gGuildManager.SetGuildNotice(lpMsg->Name,lpMsg->Notice) != 0)
	{
		SDHP_GUILD_NOTICE_SEND pMsg;

		pMsg.header.set(0xA0,0x04,sizeof(pMsg));

		memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

		memcpy(pMsg.Notice,lpMsg->Notice,sizeof(pMsg.Notice));

		this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
	}
}

void CGuild::GDGuildPostRecv(SDHP_GUILD_POST_RECV* lpMsg) // OK
{
	SDHP_GUILD_POST_SEND pMsg;

	pMsg.header.set(0xA0,0x05,sizeof(pMsg));

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,lpMsg->Member,sizeof(pMsg.Member));

	memcpy(pMsg.Message,lpMsg->Message,sizeof(pMsg.Message));

	this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
}

void CGuild::GDGuildScoreRecv(SDHP_GUILD_SCORE_RECV* lpMsg) // OK
{
	if(gGuildManager.SetGuildScore(lpMsg->Name,lpMsg->Score) != 0)
	{
		SDHP_GUILD_SCORE_SEND pMsg;

		pMsg.header.set(0xA0,0x06,sizeof(pMsg));

		memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

		pMsg.Score = lpMsg->Score;

		this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
	}
}

void CGuild::GuildInfoSend(char* name) // OK
{
	GUILD_INFO* lpGuild = gGuildManager.GetGuild(name);

	if(lpGuild != 0)
	{
		BYTE send[4096];

		SDHP_GUILD_INFO_SEND pMsg;

		pMsg.header.set(0xA0,0x07,0);

		int size = sizeof(pMsg);

		pMsg.Index = lpGuild->Index;

		memcpy(pMsg.Name,lpGuild->Name,sizeof(pMsg.Name));

		memcpy(pMsg.Master,lpGuild->Master,sizeof(pMsg.Master));

		memcpy(pMsg.Mark,lpGuild->Mark,sizeof(pMsg.Mark));

		pMsg.Score = lpGuild->Score;

		memcpy(pMsg.Notice,lpGuild->Notice,sizeof(pMsg.Notice));

		pMsg.count = 0;

		SDHP_GUILD_INFO info;

		for(int n=0;n < MAX_GUILD_MEMBER;n++)
		{
			if(lpGuild->Member[n].IsEmpty() == 0)
			{
				memcpy(info.Name,lpGuild->Member[n].Name,sizeof(info.Name));

				info.Server = lpGuild->Member[n].Server;

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
}

void CGuild::MemberOnlineOffline(char* name,int server) // OK
{
	GUILD_MEMBER_INFO* lpMember = gGuildManager.GetMemberInfo(name);

	if(lpMember != 0)
	{
		lpMember->Server = server;
	}

	GUILD_INFO* lpGuild = gGuildManager.GetGuildMember(name);

	if(lpGuild != 0)
	{
		if(server != -1)
		{
			this->GuildInfoSend(lpGuild->Name);
		}

		SDHP_GUILD_MEMBER_UPDATE_SEND pMsg;

		pMsg.header.set(0xA0,0x08,sizeof(pMsg));

		memcpy(pMsg.Name,lpGuild->Name,sizeof(pMsg.Name));

		memcpy(pMsg.Member,name,sizeof(pMsg.Member));

		pMsg.Server = server;

		this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
	}
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