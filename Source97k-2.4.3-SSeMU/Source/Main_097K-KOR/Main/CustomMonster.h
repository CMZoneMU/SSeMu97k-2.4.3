// CustomMonster.h: interface for the CCustomMonster class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_SKIN_SLOT 8
#define MAX_CUSTOM_MONSTER 150

struct CUSTOM_MONSTER_SKIN_INFO
{
	bool Use;
	int Index;
	int Slot;
	int ItemIndex;
	int ItemLevel;
	int SetOption;
	int NewOption;
};

struct CUSTOM_MONSTER_INFO
{
	int Index;
	int MonsterIndex;
	int NpcClass;
	int PKLevel;
	int Type;
	int Effect;
	float Scale;
	char FolderPath[128];
	char ModelName[32];
	char Name[25];
	CUSTOM_MONSTER_SKIN_INFO SkinInfo[MAX_SKIN_SLOT];
};

class CCustomMonster
{
public:
	CCustomMonster();
	virtual ~CCustomMonster();
	void Init();
	void Load(CUSTOM_MONSTER_INFO* info);
	void SetInfo(CUSTOM_MONSTER_INFO info);
	CUSTOM_MONSTER_INFO* GetInfo(int index);
	CUSTOM_MONSTER_INFO* GetInfoByIndex(int index);
public:
	CUSTOM_MONSTER_INFO m_CustomMonsterInfo[MAX_CUSTOM_MONSTER];
};

extern CCustomMonster gCustomMonster;