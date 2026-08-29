// PeriodicItem.h: interface for the CPeriodicItem class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DataServerProtocol.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_PERIODIC_ITEM_RECV
{
	PSWMSG_HEAD header; // C2:2A:00
	WORD index;
	char account[11];
	BYTE count;
};

struct SDHP_PERIODIC_ITEM1
{
	BYTE slot;
	DWORD serial;
};

struct SDHP_PERIODIC_ITEM_SAVE_RECV
{
	PSWMSG_HEAD header; // C2:2A:30
	WORD index;
	char account[11];
	BYTE count;
};

struct SDHP_PERIODIC_ITEM_SAVE
{
	DWORD serial;
	DWORD time;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_PERIODIC_ITEM_SEND
{
	PSWMSG_HEAD header; // C2:2A:00
	WORD index;
	char account[11];
	BYTE count;
};

struct SDHP_PERIODIC_ITEM2
{
	BYTE slot;
	DWORD serial;
	DWORD time;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CPeriodicItem
{
public:
	CPeriodicItem();
	virtual ~CPeriodicItem();
	void GDPeriodicItemRecv(SDHP_PERIODIC_ITEM_RECV* lpMsg,int index);
	void GDPeriodicItemSaveRecv(SDHP_PERIODIC_ITEM_SAVE_RECV* lpMsg);
};

extern CPeriodicItem gPeriodicItem;
