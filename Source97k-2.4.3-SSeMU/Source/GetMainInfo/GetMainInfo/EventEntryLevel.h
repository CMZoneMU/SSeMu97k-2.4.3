// EventEntryLevel.h: interface for the CEventEntryLevel class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CEventEntryLevel
{
public:
	bool Load(char* path);
	int m_BloodCastleEntryLevelCommon[7][2];
	int m_BloodCastleEntryLevelSpecial[7][2];
	int m_DevilSquareEntryLevelCommon[4][2];
	int m_DevilSquareEntryLevelSpecial[4][2];
	int m_ChaosCastleEntryLevelCommon[6][2];
	int m_ChaosCastleEntryLevelSpecial[6][2];
};

extern CEventEntryLevel gEventEntryLevel;