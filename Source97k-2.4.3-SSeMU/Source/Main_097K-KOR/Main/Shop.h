#pragma once

#include "Protocol.h"

#define MAX_MONEY 2000000000

struct PMSG_ITEM_LIST_INFO_RECV
{
	PSWMSG_HEAD header; // C2:F3:[EC:ED]
	BYTE count;
};

struct ITEM_STACK_INFO
{
	WORD Index;
	BYTE Level;
	BYTE MaxStack;
};

struct ITEM_VALUE_INFO
{
	WORD Index;
	BYTE Level;
	BYTE Grade;
	DWORD Value;
};

struct ITEM_INFO
{
	char Name[30];
	BYTE TwoHand;
	BYTE Level;
	BYTE Width;
	BYTE Height;
	BYTE DamageMin;
	BYTE DamageMax;
	BYTE DefenseSuccessRate;
	BYTE Defense;
	BYTE MagicDefense;
	BYTE AttackSpeed;
	BYTE WalkSpeed;
	BYTE Durability;
	BYTE MagicDurability;
	BYTE MagicDamageRate;
	WORD RequireStrength;
	WORD RequireDexterity;
	WORD RequireEnergy;
	BYTE RequireLevel;
	BYTE Value;
	DWORD BuyMoney;
	BYTE RequireClass[4];
	BYTE Resistance[4];
};

#define GetItemInfo(x) (ITEM_INFO*)(*(DWORD*)(0x07D78068) + x * sizeof(ITEM_INFO))

void InitShop();
WORD GetItemDurability(DWORD a,ITEM_INFO* lpItemInfo,int level);
DWORD GetItemRepairMoney(int money,int dur,int maxdur,WORD index,char* TextOut);
DWORD GetItemValue(DWORD a,int type);
int ItemMaxStack(int index,int level);
bool ItemValue(int index,int level,int grade,int dur,int* value);
void NpcTalkClose();
void GCItemStackListRecv(PMSG_ITEM_LIST_INFO_RECV* lpMsg);
void GCItemValueListRecv(PMSG_ITEM_LIST_INFO_RECV* lpMsg);