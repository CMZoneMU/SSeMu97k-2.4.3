// PeriodicItem.cpp: implementation of the CPeriodicItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PeriodicItem.h"
#include "QueryManager.h"
#include "SocketManager.h"

CPeriodicItem gPeriodicItem;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPeriodicItem::CPeriodicItem() // OK
{

}

CPeriodicItem::~CPeriodicItem() // OK
{

}

void CPeriodicItem::GDPeriodicItemRecv(SDHP_PERIODIC_ITEM_RECV* lpMsg,int index) // OK
{
	BYTE send[4096];

	SDHP_PERIODIC_ITEM_SEND pMsg;

	pMsg.header.set(0x2A,0x00,0);

	int size = sizeof(pMsg);

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	pMsg.count = 0;

	SDHP_PERIODIC_ITEM2 info;

	for(int n=0;n < lpMsg->count;n++)
	{
		SDHP_PERIODIC_ITEM1* lpInfo = (SDHP_PERIODIC_ITEM1*)(((BYTE*)lpMsg)+sizeof(SDHP_PERIODIC_ITEM_RECV)+(sizeof(SDHP_PERIODIC_ITEM1)*n));

		if(gQueryManager.ExecQuery("SELECT Time FROM PeriodicItem WHERE ItemSerial=%d",lpInfo->serial) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();
			gQueryManager.ExecQuery("INSERT INTO PeriodicItem (ItemSerial,Time) VALUES (%d,%d)",lpInfo->serial,0);
			gQueryManager.Close();

			info.slot = lpInfo->slot;
			info.serial = lpInfo->serial;
			info.time = 0;
		}
		else
		{
			info.slot = lpInfo->slot;
			info.serial = lpInfo->serial;
			info.time = gQueryManager.GetAsInteger("Time");

			gQueryManager.Close();
		}

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));

	gSocketManager.DataSend(index,send,size);
}

void CPeriodicItem::GDPeriodicItemSaveRecv(SDHP_PERIODIC_ITEM_SAVE_RECV* lpMsg) // OK
{
	for(int n=0;n < lpMsg->count;n++)
	{
		SDHP_PERIODIC_ITEM_SAVE* lpInfo = (SDHP_PERIODIC_ITEM_SAVE*)(((BYTE*)lpMsg)+sizeof(SDHP_PERIODIC_ITEM_SAVE_RECV)+(sizeof(SDHP_PERIODIC_ITEM_SAVE)*n));

		if(gQueryManager.ExecQuery("SELECT ItemSerial FROM PeriodicItem WHERE ItemSerial=%d",lpInfo->serial) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();
			gQueryManager.ExecQuery("INSERT INTO PeriodicItem (ItemSerial,Time) VALUES (%d,%d)",lpInfo->serial,lpInfo->time);
			gQueryManager.Close();
		}
		else
		{
			gQueryManager.Close();
			gQueryManager.ExecQuery("UPDATE PeriodicItem SET Time=%d WHERE ItemSerial=%d",lpInfo->time,lpInfo->serial);
			gQueryManager.Close();
		}
	}
}
