#include "stdafx.h"
#include "PeriodicItem.h"
#include "CustomMessage.h"
#include "Offset.h"
#include "ItemMove.h"
#include "ItemToolTip.h"
#include "Util.h"
#include <ctime>

DWORD m_PeriodicInfo[76];

void RenderPeriodicTipText() // OK
{
	if(pMouseOnZone(450, 0, 190, 433, 1))
	{
		DWORD lpItem = 0;

		if(ItemTarget)
		{
			lpItem = ItemTarget;
		}
		else if(*(DWORD*)0x07EAA13C == 1)
		{
			lpItem = PickedItem;
		}

		if(lpItem != 0)
		{
			int SourceSlot = -1;

			for(int i = 0; i < 2; i++)
			{
				for(int n = 0; n < INVENTORY_WEAR_SIZE; ++n)
				{
					if(*(DWORD*)0x07EA840C == (slotx[n]-(190*i)) && *(DWORD*)0x07EA8408 == sloty[n])
					{
						SourceSlot = n;
						break;
					}
				}
			}

			if(SourceSlot == -1)
			{
				SourceSlot = *(BYTE*)(lpItem + 62) + 8 * *(BYTE*)(lpItem + 63) + INVENTORY_WEAR_SIZE;
			}

			if(m_PeriodicInfo[SourceSlot] > 0)
			{
				tm ExpireTime;
	
				_tzset();

				time_t msgTime = m_PeriodicInfo[SourceSlot];

				if(localtime_s(&ExpireTime,&msgTime) == 0) 
				{
					char buff[32] = {0};
					strftime(buff,sizeof(buff),gCustomMessage.GetMessage(24),&ExpireTime);
					AddTipText(gCustomMessage.GetMessage(25),3,1);
					AddTipText(buff,1,0);
					AddTipText("\n",0,0);
				}
			}
		}
	}
}

void GCPeriodicItemInit() // OK
{
	for(int n = 0;n < 76; n++)
	{
		m_PeriodicInfo[n] = 0;
	}
}

void GCPeriodicItemRecv(PMSG_PERIODIC_ITEM_RECV* lpMsg) // OK
{
	if(lpMsg->sourceslot != 0xFFFF)
	{
		m_PeriodicInfo[lpMsg->sourceslot] = 0;
	}

	m_PeriodicInfo[lpMsg->targetslot] = lpMsg->time;
}

void GCPeriodicItemDeleteRecv(PMSG_PERIODIC_ITEM_DELETE_RECV* lpMsg) // OK
{
	int slot = lpMsg->slot;

	if(slot >= 0 && slot < INVENTORY_WEAR_SIZE)
	{
		STRUCT_DECRYPT;

		DWORD ItemStruct = GetItemEquiped(slot);

		*(WORD*)(ItemStruct) = -1;
		*(DWORD*)(ItemStruct + 4) = 0;
		*(BYTE*)(ItemStruct + 27) = 0;

		STRUCT_ENCRYPT;

		DeleteEquippingEffect(Hero);

		if(slot == 8)
		{
			DeleteBugOrPet(Hero);
		}
	}

	m_PeriodicInfo[slot] = 0;
}