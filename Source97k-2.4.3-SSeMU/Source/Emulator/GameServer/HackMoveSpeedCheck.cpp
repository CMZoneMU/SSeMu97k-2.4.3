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

CHackMoveSpeedCheck gHackMoveSpeedCheck[MAX_OBJECT_USER];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHackMoveSpeedCheck::CHackMoveSpeedCheck() // OK
{
	this->Clear();
}

CHackMoveSpeedCheck::~CHackMoveSpeedCheck() // OK
{

}

void CHackMoveSpeedCheck::Clear() // OK
{
	this->aIndex = -1;

	this->X = -1;

	this->Y = -1;

	this->m_DetectTime = GetTickCount();
}

void CHackMoveSpeedCheck::Reset() // OK
{
	if(gObjIsConnectedGP(this->aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	this->X = lpObj->X;
	
	this->Y = lpObj->Y;
}

void CHackMoveSpeedCheck::Set(int aIndex) // OK
{
	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	this->aIndex = aIndex;

	this->X = lpObj->X;
	
	this->Y = lpObj->Y;

	this->m_DetectTime = GetTickCount();
}

void CHackMoveSpeedCheck::MainProc() // OK
{
	if(gServerInfo.m_CheckMoveHack == 0)
	{
		return;
	}

	if(gObjIsConnectedGP(this->aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[this->aIndex];

	if(lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != 0 || lpObj->Live == 0)
	{
		return;
	}

	if(lpObj->Attack.Started != 0)
	{
		this->m_DetectTime = GetTickCount();
		return;
	}

	if((GetTickCount()-this->m_DetectTime) > (DWORD)gServerInfo.m_CheckMoveHackMaxDelay)
	{
		this->m_DetectTime = GetTickCount();

		int MaxDistance = gServerInfo.m_CheckMoveHackMaxCount;

		int Distance = (int)sqrt(pow(((float)lpObj->X-(float)this->X),2)+pow(((float)lpObj->Y-(float)this->Y),2));

		if(Distance > MaxDistance)
		{
			gLog.Output(LOG_HACK,"[HackMoveSpeedCheck][%s][%s] Speed Hack Detect [%d][%d]",lpObj->Account,lpObj->Name,Distance,MaxDistance);

			if(gServerInfo.m_CheckMoveHackAction == 1)
			{
				GCNewMessageSend(aIndex,0,0,gMessage.GetMessage(719));
			}
			else if(gServerInfo.m_CheckMoveHackAction == 2)
			{
				gObjUserKill(aIndex);
			}
		}

		this->Reset();
	}
}