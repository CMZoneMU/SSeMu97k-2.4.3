// ItemBagManager.h: interface for the CItemBagManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "ItemBag.h"
#include "ItemBagEx.h"
#include "User.h"

enum eItemBagSpecialValue
{
	ITEM_BAG_SPECIAL_CHAOS_BOX = 0,
	ITEM_BAG_SPECIAL_DEVIL_SQUARE = 1,
	ITEM_BAG_SPECIAL_BLOOD_CASTLE = 2,
	ITEM_BAG_SPECIAL_CHAOS_CASTLE = 3,
	ITEM_BAG_SPECIAL_SENIOR_MIX = 4,
	ITEM_BAG_SPECIAL_CUSTOM_ARENA = 20,
};

struct ITEM_BAG_MANAGER_INFO
{
	int ItemIndex;
	int ItemLevel;
	int MonsterClass;
	int SpecialValue;
	int SpecialLevel;
	int SpecialState;
	char BagPath[MAX_PATH];
	CItemBag ItemBag;
};

class CItemBagManager
{
public:
	CItemBagManager();
	virtual ~CItemBagManager();
	void Load(char* path);
	void LoadEventItemBag();
	bool GetItemBySpecialValue(int SpecialValue,int SpecialLevel,int SpecialState,LPOBJ lpObj,CItem* lpItem);
	bool DropItemByItemIndex(int ItemIndex,int ItemLevel,LPOBJ lpObj,int map,int x,int y);
	bool DropItemByMonsterClass(int MonsterClass,LPOBJ lpObj,int map,int x,int y);
	bool DropItemBySpecialValue(int SpecialValue,int SpecialLevel,int SpecialState,LPOBJ lpObj,int map,int x,int y);
private:
	std::vector<ITEM_BAG_MANAGER_INFO> m_ItemBagManagerInfo;
};

extern CItemBagManager gItemBagManager;