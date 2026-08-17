// PeriodicItem.cpp: implementation of the CPeriodicItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PeriodicItem.h"
#include "GameMain.h"
#include "ItemManager.h"
#include "Util.h"

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

void CPeriodicItem::MainProc() // OK
{
	for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if(gObjIsConnectedGP(n) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[n];

		for(int i = 0; i < INVENTORY_SIZE; i++)
		{
			if(lpObj->Inventory[i].IsItem() == 0)
			{
				continue;
			}

			if(lpObj->Inventory[i].m_IsPeriodicItem == 0)
			{
				continue;
			}

			if(lpObj->Inventory[i].m_LoadPeriodicItem == 0)
			{
				continue;
			}

			if(lpObj->Inventory[i].m_PeriodicItemTime <= ((int)time(0)))
			{
				gItemManager.InventoryDelItem(lpObj->Index,i);
				gItemManager.GCItemDeleteSend(lpObj->Index,i,1);
				gItemManager.UpdateInventoryViewport(lpObj->Index,i);
				this->GCPeriodicItemDeleteSend(lpObj->Index,i);
			}
		}
	}
}

void CPeriodicItem::DGPeriodicItemRecv(SDHP_PERIODIC_ITEM_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGPeriodicItemRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	for(int n = 0; n < lpMsg->count; n++)
	{
		SDHP_PERIODIC_ITEM1* lpInfo = (SDHP_PERIODIC_ITEM1*)(((BYTE*)lpMsg)+sizeof(SDHP_PERIODIC_ITEM_RECV)+(sizeof(SDHP_PERIODIC_ITEM1)*n));

		if(INVENTORY_RANGE(lpInfo->slot) == 0)
		{
			continue;
		}

		if(lpObj->Inventory[lpInfo->slot].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->Inventory[lpInfo->slot].m_Serial != lpInfo->serial)
		{
			continue;
		}

		if(lpObj->Inventory[lpInfo->slot].m_IsPeriodicItem == 0)
		{
			continue;
		}

		if(lpObj->Inventory[lpInfo->slot].m_LoadPeriodicItem != 0)
		{
			continue;
		}

		lpObj->Inventory[lpInfo->slot].m_LoadPeriodicItem = 1;

		lpObj->Inventory[lpInfo->slot].m_PeriodicItemTime = lpInfo->time;

		this->GCPeriodicItemSend(lpObj->Index,-1,lpInfo->slot,lpObj->Inventory[lpInfo->slot].m_PeriodicItemTime);
	}
}

void CPeriodicItem::GDPeriodicItemListSend(int aIndex) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[4096];

	SDHP_PERIODIC_ITEM_SEND pMsg;

	pMsg.header.set(0x2A,0x00,0);

	int size = sizeof(pMsg);

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	pMsg.count = 0;

	SDHP_PERIODIC_ITEM info;

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		if(lpObj->Inventory[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->Inventory[n].m_IsPeriodicItem == 0)
		{
			continue;
		}

		info.slot = n;

		info.serial = lpObj->Inventory[n].m_Serial;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	if(pMsg.count > 0)
	{
		pMsg.header.size[0] = SET_NUMBERHB(size);
		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send,&pMsg,sizeof(pMsg));

		gDataServerConnection.DataSend(send,size);
	}
}

void CPeriodicItem::GDPeriodicItemSaveSend(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[4096];

	SDHP_PERIODIC_ITEM_SAVE_SEND pMsg;

	pMsg.header.set(0x2A,0x30,0);

	int size = sizeof(pMsg);

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	pMsg.count = 0;

	SDHP_PERIODIC_ITEM_SAVE info;

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		if(lpObj->Inventory[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->Inventory[n].m_IsPeriodicItem == 0)
		{
			continue;
		}

		if(lpObj->Inventory[n].m_LoadPeriodicItem == 0)
		{
			continue;
		}

		info.serial = lpObj->Inventory[n].m_Serial;

		info.time = lpObj->Inventory[n].m_PeriodicItemTime;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	if(pMsg.count > 0)
	{
		pMsg.header.size[0] = SET_NUMBERHB(size);
		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send,&pMsg,sizeof(pMsg));

		gDataServerConnection.DataSend(send,size);
	}
}

void CPeriodicItem::GCPeriodicItemInit(int aIndex) // OK
{
	PSBMSG_HEAD pMsg;

	pMsg.set(0xD5,0x00,sizeof(pMsg));

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.size);
}

void CPeriodicItem::GCPeriodicItemSend(int aIndex,int sourceslot,int targetslot,int time) // OK
{
	PMSG_PERIODIC_ITEM_SEND pMsg;

	pMsg.header.set(0xD5,0x01,sizeof(pMsg));

	pMsg.sourceslot = sourceslot;

	pMsg.targetslot = targetslot;

	pMsg.time = time;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CPeriodicItem::GCPeriodicItemDeleteSend(int aIndex,int slot) // OK
{
	PMSG_PERIODIC_ITEM_DELETE_SEND pMsg;

	pMsg.header.set(0xD5,0x02,sizeof(pMsg));

	pMsg.slot = slot;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}