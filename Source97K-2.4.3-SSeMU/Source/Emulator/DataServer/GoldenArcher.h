// GoldenArcher.h: interface for the CGoldenArcher class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DataServerProtocol.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_GOLDEN_ARCHER_COUNT_RECV
{
	PSBMSG_HEAD header; // C1:94:00
	WORD index;
	char account[11];
};

struct SDHP_GOLDEN_ARCHER_REWARD_RECV
{
	PSBMSG_HEAD header; // C1:94:01
	WORD index;
	char account[11];
	BYTE type;
	int count;
};

struct SDHP_GOLDEN_ARCHER_ADD_COUNT_SAVE_RECV
{
	PSBMSG_HEAD header; // C1:94:30
	WORD index;
	char account[11];
	DWORD Count;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_GOLDEN_ARCHER_COUNT_SEND
{
	PSBMSG_HEAD header; // C1:94:00
	WORD index;
	char account[11];
	DWORD count;
};

struct SDHP_GOLDEN_ARCHER_REWARD_SEND
{
	PSBMSG_HEAD header; // C1:94:01
	WORD index;
	char account[11];
	BYTE type;
	BYTE result;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CGoldenArcher
{
public:
	CGoldenArcher();
	virtual ~CGoldenArcher();
	void GDGoldenArcherCountRecv(SDHP_GOLDEN_ARCHER_COUNT_RECV* lpMsg, int index);
	void GDGoldenArcherRewardRecv(SDHP_GOLDEN_ARCHER_REWARD_RECV* lpMsg, int index);
	void GDGoldenArcherAddCountSaveRecv(SDHP_GOLDEN_ARCHER_ADD_COUNT_SAVE_RECV* lpMsg);
};

extern CGoldenArcher gGoldenArcher;