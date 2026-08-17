// AntiFlood.cpp: implementation of the CAntiFlood class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AntiFlood.h"
#include "ServerInfo.h"
#include "Log.h"
#include "Util.h"

CAntiFlood gAntiFlood;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool CAntiFlood::ConnectionCheck(char* IpAddress) // OK
{
	if(gServerInfo.m_AntiFloodSwitch != 0)
	{
		DWORD currentTime = GetTickCount();

		std::map<std::string,ANTI_FLOOD_INFO>::iterator it = this->m_AntiFloodInfo.find(IpAddress);

		if(it == this->m_AntiFloodInfo.end())
		{
			ANTI_FLOOD_INFO info;

			info.Time = currentTime;
		
			info.Count = 1;
		
			info.Blocked = false;
		
			info.BlockTime = 0;

			this->m_AntiFloodInfo.insert(std::pair<std::string,ANTI_FLOOD_INFO>(IpAddress,info));
		}
		else
		{
			DWORD elapsedTime = currentTime - it->second.Time;

			if(it->second.Blocked == true)
			{
				if(elapsedTime >= (DWORD)gServerInfo.m_AntiFloodBlockTime)
				{
					it->second.Blocked = false;

					gLog.Output(LOG_HACK,"[AntiFlood] IP address %s has been unblocked.",IpAddress);
				}
				else
				{
					return false;
				}
			}
			else
			{
				if(elapsedTime <= (DWORD)gServerInfo.m_AntiFloodMaxTime)
				{
					it->second.Count++;

					if(it->second.Count > gServerInfo.m_AntiFloodMaxCount)
					{
						it->second.Blocked = true;
						it->second.BlockTime = currentTime;
						gLog.Output(LOG_HACK,"[AntiFlood] IP address %s was blocked due to too many attempts.",IpAddress);
						return false;
					}
				}
				else
				{
					it->second.Time = currentTime;
					it->second.Count = 1;
				}
			}
		}
	}

	return true;
}