// PeriodicItem.h: interface for the CPeriodicItem class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"

//**********************************************//
//********** DataServer -> GameServer **********//
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
	DWORD time;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_PERIODIC_ITEM_SEND
{
	PSWMSG_HEAD header; // C2:2A:00
	WORD index;
	char account[11];
	BYTE count;
};

struct SDHP_PERIODIC_ITEM
{
	BYTE slot;
	DWORD serial;
};

struct SDHP_PERIODIC_ITEM_SAVE_SEND
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
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_PERIODIC_ITEM_SEND
{
	PSBMSG_HEAD header; // C1:D5:01
	WORD sourceslot;
	WORD targetslot;
	DWORD time;
};

struct PMSG_PERIODIC_ITEM_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:D5:02
	BYTE slot;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CPeriodicItem
{
public:
	CPeriodicItem();
	virtual ~CPeriodicItem();
	void MainProc();
	void DGPeriodicItemRecv(SDHP_PERIODIC_ITEM_RECV* lpMsg);
	void GDPeriodicItemListSend(int aIndex);
	void GDPeriodicItemSaveSend(int aIndex);
	void GCPeriodicItemInit(int aIndex);
	void GCPeriodicItemSend(int aIndex,int sourceslot,int targetslot,int time);
	void GCPeriodicItemDeleteSend(int aIndex,int slot);
};

extern CPeriodicItem gPeriodicItem;