// Guild.h: interface for the CGuild class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DataServerProtocol.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_GUILD_CREATE_RECV
{
	PSBMSG_HEAD header; // C1:A0:00
	char Name[9];
	BYTE Mark[32];
	char Master[11];
};

struct SDHP_GUILD_REMOVE_RECV
{
	PSBMSG_HEAD header; // C1:A0:01
	char Name[9];
	char Member[11];
};

struct SDHP_GUILD_MEMBER_ADD_RECV
{
	PSBMSG_HEAD header; // C1:A0:02
	char Name[9];
	char Member[11];
};

struct SDHP_GUILD_MEMBER_DEL_RECV
{
	PSBMSG_HEAD header; // C1:A0:03
	char Name[9];
	char Member[11];
};

struct SDHP_GUILD_MEMBER_STATUS_RECV
{
	PSBMSG_HEAD header; // C1:A0:04
	char Member[11];
};

struct SDHP_GUILD_SCORE_UPDATE_RECV
{
	PSBMSG_HEAD header; // C1:A0:05
	char Name[9];
	DWORD Score;
};

struct SDHP_GUILD_GLOBAL_CHAT_RECV
{
	PSBMSG_HEAD header; // C1:A0:06
	char Name[9];
	char Member[11];
	char message[60];
};

struct SDHP_GUILD_NOTICE_UPDATE_RECV
{
	PSBMSG_HEAD header; // C1:A0:07
	char Name[9];
	char Member[11];
	char notice[60];
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_GUILD_CREATE_SEND
{
	PSBMSG_HEAD header; // C1:A0:00
	DWORD Index;
	char Name[9];
	BYTE Mark[32];
	char Master[11];
	BYTE Result;
};

struct SDHP_GUILD_REMOVE_SEND
{
	PSBMSG_HEAD header; // C1:A0:01
	char Name[9];
	char Member[11];
	BYTE Result;
};

struct SDHP_GUILD_MEMBER_ADD_SEND
{
	PSBMSG_HEAD header; // C1:A0:02
	char Name[9];
	char Member[11];
	BYTE Result;
};

struct SDHP_GUILD_MEMBER_DEL_SEND
{
	PSBMSG_HEAD header; // C1:A0:03
	char Name[9];
	char Member[11];
	BYTE Result;
};

struct SDHP_GUILD_MEMBER_STATUS_SEND
{
	PSBMSG_HEAD header; // C1:A0:04
	char Name[9];
	char Member[11];
	WORD Server;
};

struct SDHP_GUILD_SCORE_UPDATE_SEND
{
	PSBMSG_HEAD header; // C1:A0:05
	char Name[9];
	DWORD Score;
};

struct SDHP_GUILD_GLOBAL_CHAT_SEND
{
	PSBMSG_HEAD header; // C1:A0:06
	char Name[9];
	char Member[11];
	char message[60];
};

struct SDHP_GUILD_NOTICE_UPDATE_SEND
{
	PSBMSG_HEAD header; // C1:A0:07
	char Name[9];
	char Member[11];
	char notice[60];
};

struct SDHP_GUILD_INFO_SEND
{
	PSWMSG_HEAD header; // C1:A0:30
	DWORD Index;
	char Name[9];
	BYTE Mark[32];
	char Master[11];
	DWORD Score;
	BYTE count;
};

struct SDHP_GUILD_INFO
{
	char Name[11];
	WORD Server;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CGuild
{
public:
	CGuild();
	virtual ~CGuild();
	void GDGuildCreateRequestRecv(SDHP_GUILD_CREATE_RECV* lpMsg,int aIndex);
	void GDGuildRemoveRequestRecv(SDHP_GUILD_REMOVE_RECV* lpMsg,int aIndex);
	void GDGuildMemberAddRequestRecv(SDHP_GUILD_MEMBER_ADD_RECV* lpMsg,int aIndex);
	void GDGuildMemberDelRequestRecv(SDHP_GUILD_MEMBER_DEL_RECV* lpMsg,int aIndex);
	void GDGuildMemberStatusRecv(SDHP_GUILD_MEMBER_STATUS_RECV* lpMsg,int aIndex);
	void GDGuildScoreUpdateRecv(SDHP_GUILD_SCORE_UPDATE_RECV* lpMsg);
	void GDGuildGlobalChatRecv(SDHP_GUILD_GLOBAL_CHAT_RECV* lpMsg);
	void GDGuildNoticeUpdateRecv(SDHP_GUILD_NOTICE_UPDATE_RECV* lpMsg);
	void DGGuildInfoSend(char* name);
	void DGGuildMemberInfoSend(char* name,char* member,int server);
	void DataSendToAll(BYTE* lpMsg,int size);
};

extern CGuild gGuild;