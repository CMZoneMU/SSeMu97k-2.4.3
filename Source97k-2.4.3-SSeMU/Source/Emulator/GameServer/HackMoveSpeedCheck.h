// HackMoveSpeedCheck.h: interface for the CHackMoveSpeedCheck class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

class CHackMoveSpeedCheck
{
public:
	CHackMoveSpeedCheck();
	virtual ~CHackMoveSpeedCheck();
	void Clear();
	void Reset();
	void Set(int aIndex);
	void MainProc();
private:
	int aIndex;
	int X;
	int Y;
	DWORD m_DetectTime;
};

extern CHackMoveSpeedCheck gHackMoveSpeedCheck[MAX_OBJECT_USER];