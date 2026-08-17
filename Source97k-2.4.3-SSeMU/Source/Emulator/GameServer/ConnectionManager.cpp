// ConnectionManager.cpp: implementation of the CConnectionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConnectionManager.h"
#include "BlackList.h"
#include "Log.h"
#include "ServerInfo.h"
#include "Util.h"

CConnectionManager gConnectionManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConnectionManager::CConnectionManager() // OK
{
	this->m_ConnectionTimeInfo.clear();
}

bool CConnectionManager::CheckMaxConnection(int aIndex) // OK
{
	if(gServerInfo.m_MaxConnectionPerIP != 0 || gServerInfo.m_MaxConnectionPerHID != 0)
	{
		int ipCount = 0;

		int hwCount = 0;

		for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
		{
			if(gObj[n].Connected != OBJECT_OFFLINE)
			{
				if(strcmp(gObj[n].IpAddr,gObj[aIndex].IpAddr) == 0)
				{
					ipCount++;
				}

				if(strcmp(gObj[n].HardwareId,gObj[aIndex].HardwareId) == 0)
				{
					hwCount++;
				}
			}
		}

		if(gServerInfo.m_MaxConnectionPerIP > 0 && ipCount > gServerInfo.m_MaxConnectionPerIP)
		{
			gLog.Output(LOG_CONNECT,"[ObjectManager][%d] CloseClient [%s] Reason: IP Limit Exceed [%d][%d]",aIndex,gObj[aIndex].IpAddr,ipCount,gServerInfo.m_MaxConnectionPerIP);
			return 0;
		}

		if(gServerInfo.m_MaxConnectionPerHID > 0 && hwCount > gServerInfo.m_MaxConnectionPerHID)
		{
			gLog.Output(LOG_CONNECT,"[ObjectManager][%d] CloseClient [%s] Reason: HID Limit Exceed [%d][%d]",aIndex,gObj[aIndex].HardwareId,hwCount,gServerInfo.m_MaxConnectionPerHID);
			return 0;
		}
	}

	return 1;
}

void CConnectionManager::CGHardwareIdRecv(PMSG_HARDWARE_ID_INFO_RECV* lpMsg,int aIndex) // OK
{
	char HardwareId[45] = {0};

	memcpy(HardwareId,lpMsg->HardwareId,sizeof(lpMsg->HardwareId));

	if(HardwareId[8] != '-' && HardwareId[17] != '-' && HardwareId[26] != '-' && HardwareId[35] != '-')
	{
		CloseClient(aIndex);
		return;
	}

	if(strlen(HardwareId) != 44)
	{
		CloseClient(aIndex);
		return;
	}
	
	if(gBlackList.CheckHardwareId(HardwareId) == 0)
	{
		CloseClient(aIndex);
		return;
	}

	gObj[aIndex].ClientVerify = 1;

	strcpy_s(gObj[aIndex].HardwareId,HardwareId);

	gLog.Output(LOG_CONNECT,"[ObjectManager][%d] Verified client [%s][%s]",aIndex,gObj[aIndex].IpAddr,gObj[aIndex].HardwareId);
}