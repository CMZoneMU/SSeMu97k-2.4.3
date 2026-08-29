#pragma once

#define MAX_CUSTOM_ITEM 256

struct CUSTOM_ITEM_INFO
{
	int Index;
	int ItemIndex;
	int ColorR;
	int ColorG;
	int ColorB;
	char ModelName[32];
};

class CCustomItem
{
public:

	CCustomItem();

	~CCustomItem();

	void Load(CUSTOM_ITEM_INFO* info);

	void SetInfo(CUSTOM_ITEM_INFO info);

	CUSTOM_ITEM_INFO* GetInfo(int index);

	CUSTOM_ITEM_INFO* GetInfoByItem(int ItemIndex);

	bool GetCustomItemColor(int ItemIndex, float* ItemColor);

public:

	CUSTOM_ITEM_INFO m_CustomItemInfo[MAX_CUSTOM_ITEM];
};

extern CCustomItem gCustomItem;