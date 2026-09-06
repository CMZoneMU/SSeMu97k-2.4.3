// HackMoveSpeedCheck.cpp: implementation of the CHackMoveSpeedCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HackMoveSpeedCheck.h"
#include "ItemManager.h"
#include "Log.h"
#include "Message.h"
#include "Notice.h"
#include "ServerInfo.h"
#include "Util.h"

// Update 91 2.4.9 -> 97K - Reformulacao do HackMoveSpeedCheck como singleton
CHackMoveSpeedCheck gHackMoveSpeedCheck;

CHackMoveSpeedCheck::CHackMoveSpeedCheck() // OK
{

}

CHackMoveSpeedCheck::~CHackMoveSpeedCheck() // OK
{

}

void CHackMoveSpeedCheck::Reset(LPOBJ lpObj) // OK
{
	lpObj->LastX = lpObj->X;
	lpObj->LastY = lpObj->Y;
	lpObj->MoveTime = GetTickCount();
}

void CHackMoveSpeedCheck::MainProc(LPOBJ lpObj) // OK
{
	if(gServerInfo.m_CheckMoveHack == 0)
	{
		return;
	}

	if(lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->Live == 0)
	{
		return;
	}

	if(lpObj->Attack.Started != 0)
	{
		lpObj->MoveTime = GetTickCount();
		return;
	}

	if((GetTickCount()-lpObj->MoveTime) > (DWORD)gServerInfo.m_CheckMoveHackMaxDelay)
	{
		int MaxDistance = gServerInfo.m_CheckMoveHackMaxCount;

		// Update 91 2.4.9 -> 97K - Tolerancia de movimento para montaria (Uniria ou Dinorant)
		if(lpObj->Inventory[INVENTORY_SLOT_HELPER].IsItem() != 0 && (lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,2) || lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,3)))
		{
			MaxDistance += 2;
		}

		int Distance = gObjCalcDistance(lpObj,lpObj->LastX,lpObj->LastY);

		if(Distance > MaxDistance)
		{
			gLog.Output(LOG_HACK,"[HackMoveCheck][%s][%s] Move count error (Map: %d, Count: [%d][%d])",lpObj->Account,lpObj->Name,lpObj->Map,Distance,MaxDistance);

			if(gServerInfo.m_CheckMoveHackAction == 1)
			{
				GCNewMessageSend(lpObj->Index,0,0,gMessage.GetMessage(719));
			}
			else if(gServerInfo.m_CheckMoveHackAction == 2)
			{
				gObjUserKill(lpObj->Index);
			}
		}

		lpObj->LastX = lpObj->X;
		lpObj->LastY = lpObj->Y;
		lpObj->MoveTime = GetTickCount();
	}
}