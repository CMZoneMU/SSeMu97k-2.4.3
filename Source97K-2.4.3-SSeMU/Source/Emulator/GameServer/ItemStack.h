// ItemStack.h: interface for the CItemStack class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct PMSG_ITEM_LIST_STACK_SEND
{
	PSWMSG_HEAD header; // C2:F3:[EC:ED]
	BYTE count;
};

struct PMSG_ITEM_STACK_INFO
{
	WORD Index;
	BYTE Level;
	BYTE MaxStack;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct ITEM_STACK_INFO
{
	int ItemIndex;
	int Level;
	int MaxStack;
	int CreateItemIndex;
};

class CItemStack
{
public:
	CItemStack();
	virtual ~CItemStack();
	void Load(char* path);
	bool CheckItemStack(int index,int level);
	int GetItemMaxStack(int index,int level);
	int GetCreateItemIndex(int index);
	void GCItemListStackSend(int aIndex = -1);
private:
	std::vector<ITEM_STACK_INFO> m_ItemStackInfo;
};

extern CItemStack gItemStack;
