// Update 88 2.4.6 -> 97K - Sistema de recompensas diárias
// CustomDailyReward.h: interface for the CCustomDailyReward class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DataServerProtocol.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_DAILY_REWARD_INFO_RECV
{
	PBMSG_HEAD header; // C1:16
	WORD aIndex;
	char account[11];
	char name[11];
	int index;
	int MinOnlineTime;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_DAILY_REWARD_INFO_SEND
{
	PBMSG_HEAD header; // C1:16
	WORD aIndex;
	char account[11];
	char name[11];
	int index;
	BYTE result;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CCustomDailyReward
{
public:
	CCustomDailyReward();
	virtual ~CCustomDailyReward();
	void GDDailyRewardCheckRecv(SDHP_DAILY_REWARD_INFO_RECV* lpMsg,int index);
};

extern CCustomDailyReward gCustomDailyReward;
