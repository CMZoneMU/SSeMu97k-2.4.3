// HackMoveSpeedCheck.h: interface for the CHackMoveSpeedCheck class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

// Update 91 2.4.9 -> 97K - Reformulacao do HackMoveSpeedCheck como singleton
class CHackMoveSpeedCheck
{
public:
	CHackMoveSpeedCheck();
	virtual ~CHackMoveSpeedCheck();
	void Reset(LPOBJ lpObj);
	void MainProc(LPOBJ lpObj);
};

extern CHackMoveSpeedCheck gHackMoveSpeedCheck;