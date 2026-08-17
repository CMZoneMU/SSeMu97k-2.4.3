// Fruit.h: interface for the CFruit class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"
#include "User.h"

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_FRUIT_RESULT_SEND
{
	PBMSG_HEAD header; // C1:2C
	BYTE result;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CFruit
{
public:
	CFruit();
	virtual ~CFruit();
	void Init();
	int GetMaxFruitPoint(LPOBJ lpObj);
	bool UseFruitItem(LPOBJ lpObj,int type);
private:
	int m_NRMaxFruitPointTable[MAX_CHARACTER_LEVEL];
	int m_MGMaxFruitPointTable[MAX_CHARACTER_LEVEL];
	int m_DLMaxFruitPointTable[MAX_CHARACTER_LEVEL];
};

extern CFruit gFruit;
