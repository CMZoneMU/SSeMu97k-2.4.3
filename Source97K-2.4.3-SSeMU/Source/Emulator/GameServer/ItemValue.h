// ItemValue.h: interface for the CItemValue class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Item.h"
#include "Protocol.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct PMSG_ITEM_LIST_VALUE_SEND
{
	PSWMSG_HEAD header; // C2:F3:[EC:ED]
	BYTE count;
};

struct PMSG_ITEM_VALUE_INFO
{
	WORD Index;
	BYTE Level;
	BYTE Grade;
	DWORD Value;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct ITEM_VALUE_INFO
{
	int ItemIndex;
	int Level;
	int Grade;
	int Value;
};

class CItemValue
{
public:
	CItemValue();
	virtual ~CItemValue();
	void Load(char* path);
	bool GetItemValue(CItem* lpItem,int* value);
	void GCItemListValueSend(int aIndex = -1);
private:
	std::vector<ITEM_VALUE_INFO> m_ItemValueInfo;
};

extern CItemValue gItemValue;
