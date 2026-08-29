// IpManager.cpp: implementation of the CIpManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IpManager.h"

CIpManager gIpManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIpManager::CIpManager() // OK
{
	this->m_IpAddressInfo.clear();
}

bool CIpManager::CheckIpAddress(char* IpAddress) // OK
{
	std::map<std::string,IP_ADDRESS_INFO>::iterator lpIpAddress = this->m_IpAddressInfo.find(IpAddress);

	if(lpIpAddress != this->m_IpAddressInfo.end())
	{
		if((GetTickCount()-lpIpAddress->second.TickCount) < 1000)
		{
			return 0;
		}

		return ((lpIpAddress->second.count>=MaxConnectionPerIP)?0:1);
	}

	return 1;
}

void CIpManager::InsertIpAddress(char* IpAddress) // OK
{
	IP_ADDRESS_INFO info;

	strcpy_s(info.IpAddress,IpAddress);

	info.TickCount = GetTickCount();

	info.count = 1;

	std::map<std::string,IP_ADDRESS_INFO>::iterator it = this->m_IpAddressInfo.find(std::string(IpAddress));

	if(it == this->m_IpAddressInfo.end())
	{
		this->m_IpAddressInfo.insert(std::pair<std::string,IP_ADDRESS_INFO>(std::string(IpAddress),info));
	}
	else
	{
		it->second.count++;
	}
}

void CIpManager::RemoveIpAddress(char* IpAddress) // OK
{
	std::map<std::string,IP_ADDRESS_INFO>::iterator it = this->m_IpAddressInfo.find(std::string(IpAddress));

	if(it != this->m_IpAddressInfo.end())
	{
		if((--it->second.count) == 0)
		{
			this->m_IpAddressInfo.erase(it);
		}
	}
}
