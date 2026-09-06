// Update 88 2.4.6 -> 97K - Sistema de recompensas diárias
// CustomDailyReward.cpp: implementation of the CCustomDailyReward class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomDailyReward.h"
#include "QueryManager.h"
#include "SocketManager.h"

CCustomDailyReward gCustomDailyReward;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomDailyReward::CCustomDailyReward()
{

}

CCustomDailyReward::~CCustomDailyReward()
{

}

void CCustomDailyReward::GDDailyRewardCheckRecv(SDHP_DAILY_REWARD_INFO_RECV* lpMsg,int index)
{
	SDHP_DAILY_REWARD_INFO_SEND pMsg;

	pMsg.header.set(0x16,sizeof(pMsg));

	pMsg.aIndex = lpMsg->aIndex;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	pMsg.index = lpMsg->index;

	pMsg.result = 0;

	if(gQueryManager.ExecQuery("SELECT * FROM DailyRewardData WHERE Name='%s' AND [index]=%d",lpMsg->name,lpMsg->index) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		gQueryManager.ExecQuery("INSERT INTO DailyRewardData (Name,[index],result) VALUES ('%s',%d,0)",lpMsg->name,lpMsg->index);

		gQueryManager.Close();
	}
	else
	{
		pMsg.result = (BYTE)gQueryManager.GetAsInteger("result");

		gQueryManager.Close();
	}

	if(pMsg.result == 0)
	{
		if(lpMsg->MinOnlineTime == 0)
		{
			pMsg.result = 1;

			gQueryManager.ExecQuery("UPDATE DailyRewardData SET result=1 WHERE Name='%s' AND [index]=%d",lpMsg->name,lpMsg->index);

			gQueryManager.Close();
		}
		else
		{
			if(gQueryManager.ExecQuery("SELECT OnlineHours FROM MEMB_STAT WHERE memb___id='%s' AND OnlineHours >= %d",lpMsg->account,lpMsg->MinOnlineTime) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
			{
				pMsg.result = 0;

				gQueryManager.Close();
			}
			else
			{
				pMsg.result = 1;

				gQueryManager.Close();

				gQueryManager.ExecQuery("UPDATE DailyRewardData SET result=1 WHERE Name='%s' AND [index]=%d",lpMsg->name,lpMsg->index);

				gQueryManager.Close();
			}
		}
	}
	else
	{
		pMsg.result = 0;
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
}
