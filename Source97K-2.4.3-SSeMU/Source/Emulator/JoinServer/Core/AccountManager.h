// AccountManager.h: interface for the CAllowableIpList class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "CriticalSection.h"

#define MAX_ACCOUNT 10000

struct ACCOUNT_INFO
{
	char Account[11];
	char Password[11];
	char IpAddress[16];
	char HardwareId[45];
	WORD UserIndex;
	WORD GameServerCode;
};

class CAccountManager
{
public:
	CAccountManager();
	virtual ~CAccountManager();
	void ClearServerAccountInfo(WORD ServerCode);
	bool GetAccountInfo(ACCOUNT_INFO* lpAccountInfo,char* account);
	void InsertAccountInfo(ACCOUNT_INFO AccountInfo);
	void RemoveAccountInfo(ACCOUNT_INFO AccountInfo);
	long GetAccountCount();
	bool CheckConnectionLimit(char* IpAddress,char* HardwareId);
private:
	CCriticalSection m_critical;
	std::map<std::string,ACCOUNT_INFO> m_AccountInfo;
};

extern CAccountManager gAccountManager;

static const int gJoinServerMaxAccount[5] = { ((12 << 2) + 2) * 2,((10 << 7) - 250 - 30) - 500 - 300,((10 << 7) - 200 - 80) / 2,((10 << 7) - 180 - 100) * 10,((10 << 7) - 80 - 200) * 10 };