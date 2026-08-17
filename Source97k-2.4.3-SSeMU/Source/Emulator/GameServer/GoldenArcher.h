// GoldenArcher.h: interface for the CGoldenArcher class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_GOLDEN_ARCHER_COUNT_RECV
{
	PBMSG_HEAD header; // C1:95
	BYTE type;
	BYTE slot;
};

struct PMSG_GOLDEN_ARCHER_REWARD_RECV
{
	PBMSG_HEAD header; // C1:96
	BYTE type;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_GOLDEN_ARCHER_COUNT_SEND
{
	PBMSG_HEAD header; // C1:94
	BYTE type;
	DWORD count;
	WORD code[3];
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_GOLDEN_ARCHER_COUNT_RECV
{
	PSBMSG_HEAD header; // C1:94:00
	WORD index;
	char account[11];
	DWORD count;
};

struct SDHP_GOLDEN_ARCHER_REWARD_RECV
{
	PSBMSG_HEAD header; // C1:94:01
	WORD index;
	char account[11];
	BYTE type;
	BYTE result;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_GOLDEN_ARCHER_COUNT_SEND
{
	PSBMSG_HEAD header; // C1:94:00
	WORD index;
	char account[11];
};

struct SDHP_GOLDEN_ARCHER_REWARD_SEND
{
	PSBMSG_HEAD header; // C1:94:01
	WORD index;
	char account[11];
	BYTE type;
	int count;
};

struct SDHP_GOLDEN_ARCHER_ADD_COUNT_SAVE_SEND
{
	PSBMSG_HEAD header; // C1:94:30
	WORD index;
	char account[11];
	DWORD count;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct GOLDEN_ARCHER_ITEM_INFO
{
	int ItemIndex;
	int Group;
	int Option0;
	int Option1;
	int Option2;
	int Option3;
	int Option4;
	int Option5;
	int Duration;
	int DropRate;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CGoldenArcher
{
public:
	CGoldenArcher();
	virtual ~CGoldenArcher();
	void Load(char* path);
	void CGGoldenArcherCountRecv(int aIndex);
	void CGGoldenArcherRegisterRecv(PMSG_GOLDEN_ARCHER_COUNT_RECV* lpMsg,int aIndex);
	void CGGoldenArcherRewardRecv(PMSG_GOLDEN_ARCHER_REWARD_RECV* lpMsg,int aIndex);
	void CGGoldenArcherCloseRecv(int aIndex);
	void DGGoldenArcherCountRecv(SDHP_GOLDEN_ARCHER_COUNT_RECV* lpMsg);
	void DGGoldenArcherRewardRecv(SDHP_GOLDEN_ARCHER_REWARD_RECV* lpMsg);
	void GDGoldenArcherAddCountSaveSend(int aIndex, DWORD count);
private:
	std::vector<GOLDEN_ARCHER_ITEM_INFO> m_GoldenArcherItemInfo;
};

static const BYTE m_GoldenArcherAmount[5] = {10,20,30,50,100};

extern CGoldenArcher gGoldenArcher;