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
	char Master[11];
	BYTE Mark[32];
	int Server;
};

struct SDHP_GUILD_DELETE_RECV
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
	int Server;
};

struct SDHP_GUILD_MEMBER_DEL_RECV
{
	PSBMSG_HEAD header; // C1:A0:03
	char Name[9];
	char Member[11];
};

struct SDHP_GUILD_NOTICE_RECV
{
	PSBMSG_HEAD header; // C1:A0:04
	char Name[9];
	char Notice[60];
};

struct SDHP_GUILD_POST_RECV
{
	PSBMSG_HEAD header; // C1:A0:05
	char Name[9];
	char Member[11];
	char Message[60];
};

struct SDHP_GUILD_SCORE_RECV
{
	PSBMSG_HEAD header; // C1:A0:06
	char Name[9];
	DWORD Score;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_GUILD_CREATE_SEND
{
	PSBMSG_HEAD header; // C1:A0:00
	DWORD Index;
	char Name[9];
	char Master[11];
	BYTE Mark[32];
	BYTE result;
};

struct SDHP_GUILD_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:A0:01
	char Name[9];
	char Member[11];
	BYTE result;
};

struct SDHP_GUILD_MEMBER_ADD_SEND
{
	PSBMSG_HEAD header; // C1:A0:02
	char Name[9];
	char Member[11];
	int Server;
	BYTE result;
};

struct SDHP_GUILD_MEMBER_DEL_SEND
{
	PSBMSG_HEAD header; // C1:A0:03
	char Name[9];
	char Member[11];
	BYTE result;
};

struct SDHP_GUILD_NOTICE_SEND
{
	PSBMSG_HEAD header; // C1:A0:04
	char Name[9];
	char Notice[60];
};

struct SDHP_GUILD_POST_SEND
{
	PSBMSG_HEAD header; // C1:A0:05
	char Name[9];
	char Member[11];
	char Message[60];
};

struct SDHP_GUILD_SCORE_SEND
{
	PSBMSG_HEAD header; // C1:A0:06
	char Name[9];
	DWORD Score;
};

struct SDHP_GUILD_INFO_SEND
{
	PSWMSG_HEAD header; // C2:A0:07
	DWORD Index;
	char Name[9];
	char Master[11];
	BYTE Mark[32];
	DWORD Score;
	char Notice[60];
	BYTE count;
};

struct SDHP_GUILD_INFO
{
	char Name[11];
	int Server;
};

struct SDHP_GUILD_MEMBER_UPDATE_SEND
{
	PSBMSG_HEAD header; // C1:A0:08
	char Name[9];
	char Member[11];
	int Server;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CGuild
{
public:
	CGuild();
	virtual ~CGuild();
	void GDGuildCreateRecv(SDHP_GUILD_CREATE_RECV* lpMsg,int aIndex);
	void GDGuildDeleteRecv(SDHP_GUILD_DELETE_RECV* lpMsg,int aIndex);
	void GDGuildMemberAddRecv(SDHP_GUILD_MEMBER_ADD_RECV* lpMsg,int aIndex);
	void GDGuildMemberDelRecv(SDHP_GUILD_MEMBER_DEL_RECV* lpMsg,int aIndex);
	void GDGuildNoticeRecv(SDHP_GUILD_NOTICE_RECV* lpMsg);
	void GDGuildPostRecv(SDHP_GUILD_POST_RECV* lpMsg);
	void GDGuildScoreRecv(SDHP_GUILD_SCORE_RECV* lpMsg);
	void GuildInfoSend(char* name);
	void MemberOnlineOffline(char* name, int server);
	void DataSendToAll(BYTE* lpMsg,int size);
};

extern CGuild gGuild;