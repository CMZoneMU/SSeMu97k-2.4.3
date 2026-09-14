// Update 93 2.5.1 -> 97K - Barra de vida de monstros customizada
// CustomHealthBar.h: interface for the CCustomHealthBar class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

struct MONSTER_HEALTH_BAR_INFO
{
	int MonsterIndex;
	int MapNumber;
	int BarSwitch;
};

class CCustomHealthBar
{
public:
	CCustomHealthBar();
	virtual ~CCustomHealthBar();
	void Load(char* path);
	int CheckHealthBar(int MonsterIndex,int MapNumber);
private:
	std::vector<MONSTER_HEALTH_BAR_INFO> m_MonsterHealthBarInfo;
};

extern CCustomHealthBar gCustomHealthBar;
