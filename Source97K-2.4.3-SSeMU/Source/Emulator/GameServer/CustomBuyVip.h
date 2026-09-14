// Update 93 2.5.1 -> 97K - Sistema de compra de VIP por comando /buyvip
// CustomBuyVip.h: interface for the CCustomBuyVip class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

struct CUSTOM_BUY_VIP_INFO
{
	int AccountLevel;
	int NewAccountLevel;
	int Days;
	DWORD Money;
	DWORD Coin1;
	DWORD Coin2;
	char Code[32];
};

class CCustomBuyVip
{
public:
	CCustomBuyVip();
	virtual ~CCustomBuyVip();
	void ReadCustomBuyVipInfo(char* section,char* path);
	void Load(char* path);
	void CommandBuyVip(LPOBJ lpObj,char* arg);
private:
	int m_CustomBuyVipSwitch;
	std::map<std::string,CUSTOM_BUY_VIP_INFO> m_CustomBuyVipInfo;
};

extern CCustomBuyVip gCustomBuyVip;
