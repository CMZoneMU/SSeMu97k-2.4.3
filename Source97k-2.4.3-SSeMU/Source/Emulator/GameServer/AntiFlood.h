// AntiFlood.h: interface for the CAntiFlood class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

struct ANTI_FLOOD_INFO
{
	DWORD Time;
	int Count;
	bool Blocked;
	DWORD BlockTime;
};

class CAntiFlood
{
public:
	bool ConnectionCheck(char* IpAddress);
public:
	std::map<std::string,ANTI_FLOOD_INFO> m_AntiFloodInfo;
};

extern CAntiFlood gAntiFlood;