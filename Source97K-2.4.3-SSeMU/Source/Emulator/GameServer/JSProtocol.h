#pragma once

#include "Protocol.h"

//**********************************************//
//********** JoinServer -> GameServer **********//
//**********************************************//

struct SDHP_CONNECT_ACCOUNT_RECV
{
	PBMSG_HEAD header; // C1:01
	WORD index;
	char account[11];
	char PersonalCode[14];
	BYTE result;
	BYTE BlockCode;
	WORD AccountLevel;
	char AccountExpireDate[20];
};

struct SDHP_DISCONNECT_ACCOUNT_RECV
{
	PBMSG_HEAD header; // C1:02
	WORD index;
	char account[11];
	BYTE result;
};

struct SDHP_ACCOUNT_LEVEL_RECV
{
	PBMSG_HEAD header; // C1:05
	WORD index;
	char account[11];
	WORD AccountLevel;
	char AccountExpireDate[20];
};

struct SDHP_ACCOUNT_ALREADY_CONNECTED_RECV
{
	PBMSG_HEAD header; // C1:30
	WORD index;
	char account[11];
};

//**********************************************//
//********** GameServer -> JoinServer **********//
//**********************************************//

struct SDHP_JOIN_SERVER_INFO_SEND
{
	PBMSG_HEAD header; // C1:00
	BYTE type;
	WORD ServerPort;
	char ServerName[50];
	WORD ServerCode;
};

struct SDHP_CONNECT_ACCOUNT_SEND
{
	PBMSG_HEAD header; // C1:01
	WORD index;
	char account[11];
	char password[11];
	char IpAddress[16];
	char HardwareId[45];
};

struct SDHP_DISCONNECT_ACCOUNT_SEND
{
	PBMSG_HEAD header; // C1:02
	WORD index;
	char account[11];
	char IpAddress[16];
	char HardwareId[45];
};

struct SDHP_ACCOUNT_LEVEL_SEND
{
	PBMSG_HEAD header; // C1:05
	WORD index;
	char account[11];
};

struct SDHP_ACCOUNT_LEVEL_SAVE_SEND
{
	PBMSG_HEAD header; // C1:11
	WORD index;
	char account[11];
	WORD AccountLevel;
	DWORD AccountExpireTime;
};

struct SDHP_SERVER_USER_INFO_SEND
{
	PBMSG_HEAD header; // C1:20
	WORD CurUserCount;
	WORD MaxUserCount;
};

//**********************************************//
//**********************************************//
//**********************************************//

void JoinServerProtocolCore(BYTE head,BYTE* lpMsg,int size);
void JGConnectAccountRecv(SDHP_CONNECT_ACCOUNT_RECV* lpMsg);
void JGDisconnectAccountRecv(SDHP_DISCONNECT_ACCOUNT_RECV* lpMsg);
void JGAccountLevelRecv(SDHP_ACCOUNT_LEVEL_RECV* lpMsg);
void JGAccountAlreadyConnectedRecv(SDHP_ACCOUNT_ALREADY_CONNECTED_RECV* lpMsg);
void GJServerInfoSend();
void GJConnectAccountSend(int aIndex,char* account,char* password,char* IpAddress,char* HardwareId);
void GJDisconnectAccountSend(int aIndex,char* account,char* IpAddress,char* HardwareId);
void GJAccountLevelSend(int aIndex);
void GJAccountLevelSaveSend(int aIndex,int AccountLevel,int AccountExpireTime);
void GJServerUserInfoSend();
