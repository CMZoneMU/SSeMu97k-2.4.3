#pragma once

#include "Protocol.h"

struct PMSG_PERIODIC_ITEM_RECV
{
	PSBMSG_HEAD header; // C1:D5:01
	WORD sourceslot;
	WORD targetslot;
	DWORD time;
};

struct PMSG_PERIODIC_ITEM_DELETE_RECV
{
	PSBMSG_HEAD header; // C1:D5:02
	BYTE slot;
};

void RenderPeriodicTipText();
void GCPeriodicItemInit();
void GCPeriodicItemRecv(PMSG_PERIODIC_ITEM_RECV* lpMsg);
void GCPeriodicItemDeleteRecv(PMSG_PERIODIC_ITEM_DELETE_RECV* lpMsg);