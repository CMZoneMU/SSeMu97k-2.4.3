// EventEntryLevel.h: interface for the CEventEntryLevel class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "BloodCastle.h"
#include "DevilSquare.h"
#include "User.h"

class CEventEntryLevel
{
public:
	void Load(char* path);
	int GetBCLevel(LPOBJ lpObj);
	int GetDSLevel(LPOBJ lpObj);
	int GetCCLevel(LPOBJ lpObj);
private:
	int m_BloodCastleEntryLevelCommon[MAX_BC_LEVEL][2];
	int m_BloodCastleEntryLevelSpecial[MAX_BC_LEVEL][2];
	int m_DevilSquareEntryLevelCommon[MAX_DS_LEVEL][2];
	int m_DevilSquareEntryLevelSpecial[MAX_DS_LEVEL][2];
};

extern CEventEntryLevel gEventEntryLevel;