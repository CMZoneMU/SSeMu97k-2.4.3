#include "stdafx.h"
#include "Monster.h"
#include "Attack.h"
#include "BattleSoccer.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "CriticalSection.h"
#include "CustomMonster.h"
#include "DevilSquare.h"
#include "DropEvent.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "ItemBagManager.h"
#include "ItemDrop.h"
#include "ItemMoneyDrop.h"
#include "ItemOptionRate.h"
#include "Map.h"
#include "MapManager.h"
#include "MapRateInfo.h"
#include "MemoryAllocator.h"
#include "MonsterManager.h"
#include "MonsterSetBase.h"
#include "ObjectManager.h"
#include "Party.h"
#include "Quest.h"
#include "QuestObjective.h"
#include "ServerInfo.h"
#include "ShopManager.h"
#include "SkillManager.h"
#include "Util.h"
#include "Viewport.h"

void gObjMonsterDieGiveItem(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);

	if(OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	if(OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		return;
	}

	if(lpObj->Class == 132 || lpObj->Class == 133 || lpObj->Class == 134) // Statue of Saint
	{
		return;
	}

	if(lpObj->Class == 247 || lpObj->Class == 249) // Guard
	{
		return;
	}

	if(gItemBagManager.DropItemByMonsterClass(lpObj->Class,lpTarget,lpObj->Map,lpObj->X,lpObj->Y) != 0)
	{
		return;
	}

	if(gQuestObjective.MonsterItemDrop(lpObj) != 0)
	{
		return;
	}

	if(gDropEvent.DropItem(lpObj,lpTarget) != 0)
	{
		return;
	}

	if(gItemDrop.DropItem(lpObj,lpTarget) != 0)
	{
		return;
	}

	if(gItemMoneyDrop.DropMoney(lpObj,lpTarget) != 0)
	{
		return;
	}

	int DropIndex = -1;

	int DropLevel = lpObj->Level;

	int ExcItemDrop = 0;

	int ItemDropRate = gServerInfo.m_ItemDropRate[lpTarget->AccountLevel];

	ItemDropRate = (ItemDropRate*gMapManager.GetMapItemDropRate(lpTarget->Map))/100;

	ItemDropRate = (ItemDropRate*gBonusManager.GetBonusValue(lpTarget,BONUS_INDEX_ITEM_DROP_RATE,100))/100;

	int ExcItemDropRate = gMapManager.GetMapExcItemDropRate(lpTarget->Map);

	ExcItemDropRate = gBonusManager.GetBonusValue(lpTarget,BONUS_INDEX_EXC_ITEM_DROP_RATE,ExcItemDropRate);

	if(lpObj->Level >= 25 && (GetLargeRand()%1000000) < ExcItemDropRate)
	{
		DropLevel -= 25;
		ExcItemDrop = 1;
	}

	if((DropIndex=gMonsterManager.GetMonsterItem(DropLevel,ExcItemDrop)) >= GET_ITEM(0,0) && (GetLargeRand()%((lpObj->ItemRate==0)?1:lpObj->ItemRate)) < ItemDropRate)
	{
		WORD ItemIndex = DropIndex;
		BYTE ItemLevel = 0;
		BYTE ItemOption1 = 0;
		BYTE ItemOption2 = 0;
		BYTE ItemOption3 = 0;
		BYTE ItemNewOption = 0;
		BYTE ItemSetOption = 0;

		gItemOptionRate.GetItemOption0(gMapRateInfo.GetDropOption0(lpObj->Map,((ExcItemDrop==0)?DROP_TYPE_COMMON:DROP_TYPE_EXCELLENT)),&ItemLevel);

		gItemOptionRate.GetItemOption1(gMapRateInfo.GetDropOption1(lpObj->Map,((ExcItemDrop==0)?DROP_TYPE_COMMON:DROP_TYPE_EXCELLENT)),&ItemOption1);

		gItemOptionRate.GetItemOption2(gMapRateInfo.GetDropOption2(lpObj->Map,((ExcItemDrop==0)?DROP_TYPE_COMMON:DROP_TYPE_EXCELLENT)),&ItemOption2);

		gItemOptionRate.GetItemOption3(gMapRateInfo.GetDropOption3(lpObj->Map,((ExcItemDrop==0)?DROP_TYPE_COMMON:DROP_TYPE_EXCELLENT)),&ItemOption3);

		gItemOptionRate.GetItemOption4(gMapRateInfo.GetDropOption4(lpObj->Map,((ExcItemDrop==0)?DROP_TYPE_COMMON:DROP_TYPE_EXCELLENT)),&ItemNewOption);

		gItemOptionRate.GetItemOption5(gMapRateInfo.GetDropOption5(lpObj->Map,((ExcItemDrop==0)?DROP_TYPE_COMMON:DROP_TYPE_EXCELLENT)),&ItemSetOption);

		ItemLevel = ((ItemIndex>=GET_ITEM(12,0))?0:ItemLevel);

		ItemOption1 = ((ItemIndex>=GET_ITEM(12,0))?0:ItemOption1);

		ItemOption2 = ((ItemIndex>=GET_ITEM(12,0))?0:ItemOption2);

		ItemNewOption = ((ItemIndex>=GET_ITEM(12,0))?0:ItemNewOption);

		gItemOptionRate.MakeNewOption(ItemIndex,1,ItemNewOption,&ItemNewOption);

		GDCreateItemSend(lpTarget->Index,lpObj->Map,(BYTE)lpObj->X,(BYTE)lpObj->Y,ItemIndex,ItemLevel,0,ItemOption1,ItemOption2,ItemOption3,lpTarget->Index,ItemNewOption,ItemSetOption,0);

	}
	else if(lpObj->Money > 0 && (GetLargeRand()%((lpObj->MoneyRate==0)?1:lpObj->MoneyRate)) < (gMapManager.GetMoneyDropRate(lpObj->Map)/10))
	{
		int money = ((__int64)lpObj->Money*gServerInfo.m_MoneyAmountDropRate[lpTarget->AccountLevel])/100;

		money = ((__int64)money*lpTarget->MoneyAmountDropRate)/100;

		money = ((money>0)?money:1);

		gMap[lpObj->Map].MoneyItemDrop(money,lpObj->X,lpObj->Y);
	}
}

bool gObjSetPosMonster(int aIndex,int number) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(number < 0 || number > MAX_MSB_MONSTER)
	{
		return 0;
	}

	MONSTER_SET_BASE_INFO* lpInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[number];

	short x,y;

	if(gMonsterSetBase.GetPosition(number,lpInfo->Map,&x,&y) == 0)
	{
		return 0;
	}

	lpObj->PosNum = number;
	lpObj->X = x;
	lpObj->Y = y;
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->OldX = lpObj->X;
	lpObj->OldY = lpObj->Y;
	lpObj->StartX = (BYTE)lpObj->X;
	lpObj->StartY = (BYTE)lpObj->Y;
	lpObj->Dir = lpInfo->Dir;
	lpObj->Map = lpInfo->Map;
	return 1;
}

bool gObjSetMonster(int aIndex,int MonsterClass) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->ConnectTickCount = GetTickCount();
	lpObj->ShopNumber = gShopManager.GetShopNumber(MonsterClass,lpObj->Map,lpObj->X,lpObj->Y);
	lpObj->TargetNumber = -1;
	lpObj->SummonIndex = -1;
	lpObj->LastAttackerID = -1;
	lpObj->Connected = OBJECT_ONLINE;
	lpObj->Live = 1;
	lpObj->State = OBJECT_CREATE;
	lpObj->DieRegen = 0;
	lpObj->Class = MonsterClass;

	if(MonsterClass == 200)
	{
		gBattleSoccer.SetBallIndex(aIndex);
	}

	MONSTER_INFO* lpInfo = gMonsterManager.GetInfo(MonsterClass);

	if(lpInfo == 0)
	{
		return 0;
	}

	if(lpInfo->Type == 0)
	{
		lpObj->Type = OBJECT_MONSTER;
	}
	else
	{
		lpObj->Type = OBJECT_NPC;
	}

	memcpy(lpObj->Name,lpInfo->Name,sizeof(lpObj->Name));
	memcpy(lpObj->MonsterName,lpInfo->Name,sizeof(lpObj->MonsterName));

	lpObj->Level = lpInfo->Level;
	lpObj->PhysiSpeed = lpInfo->AttackSpeed;
	lpObj->PhysiDamageMin = lpInfo->DamageMin;
	lpObj->PhysiDamageMax = lpInfo->DamageMax;
	lpObj->Defense = lpInfo->Defense;
	lpObj->MagicDefense = lpInfo->MagicDefense;
	lpObj->AttackSuccessRate = lpInfo->AttackRate;
	lpObj->DefenseSuccessRate = lpInfo->DefenseRate;
	lpObj->Life = (float)lpInfo->Life;
	lpObj->MaxLife = (float)lpInfo->Life;
	lpObj->Mana = (float)lpInfo->Mana;
	lpObj->MaxMana = (float)lpInfo->Mana;
	lpObj->MoveRange = lpInfo->MoveRange;
	lpObj->MoveSpeed = lpInfo->MoveSpeed;
	lpObj->MaxRegenTime = lpInfo->RegenTime*1000;
	lpObj->AttackRange = lpInfo->AttackRange;
	lpObj->ViewRange = lpInfo->ViewRange;
	lpObj->Attribute = lpInfo->Attribute;
	lpObj->AttackType = lpInfo->AttackType;
	lpObj->ItemRate = lpInfo->ItemRate;
	lpObj->MoneyRate = lpInfo->MoneyRate;
	lpObj->Resistance[0] = ((lpInfo->Resistance[0]>255)?255:lpInfo->Resistance[0]);
	lpObj->Resistance[1] = ((lpInfo->Resistance[1]>255)?255:lpInfo->Resistance[1]);
	lpObj->Resistance[2] = ((lpInfo->Resistance[2]>255)?255:lpInfo->Resistance[2]);
	lpObj->Resistance[3] = ((lpInfo->Resistance[3]>255)?255:lpInfo->Resistance[3]);
	lpObj->ScriptMaxLife = (float)lpInfo->ScriptLife;
	lpObj->RegenType = 0;

	gObjSetInventory1Pointer(lpObj);

	gCustomMonster.SetCustomMonsterInfo(lpObj);

	if(lpObj->AttackType != 0)
	{
		if(lpObj->AttackType == 150)
		{
			gSkillManager.AddSkill(lpObj,SKILL_MONSTER_AREA_ATTACK);
		}
		else
		{
			gSkillManager.AddSkill(lpObj,lpObj->AttackType);
		}

		if(MonsterClass == 66 || MonsterClass == 73) // Cursed King,Drakan
		{
			gSkillManager.AddSkill(lpObj,SKILL_ENERGY_BALL);
		}

		if(MonsterClass == 77) // Phoenix of Darkness
		{
			gSkillManager.AddSkill(lpObj,SKILL_LIGHTNING);
		}
	}

	gMap[lpObj->Map].SetStandAttr(lpObj->X,lpObj->Y);
	lpObj->OldX = lpObj->X;
	lpObj->OldY = lpObj->Y;
	return 1;
}

bool gObjMonsterRegen(LPOBJ lpObj) // OK
{
	gObjClearViewport(lpObj);

	lpObj->TargetNumber = -1;
	lpObj->LastAttackerID = -1;
	lpObj->NextActionTime = 5000;
	lpObj->ActionState.Reset();

	if(lpObj->Attribute == ATTRIBUTE_KALIMA)
	{
		lpObj->Live = 1;
		lpObj->DieRegen = 0;
		gObjClearViewport(lpObj);
		gObjViewportListCreate(lpObj->Index);
		gObjViewportListProtocolCreate(lpObj);
		return 0;
	}

	if(lpObj->Attribute == ATTRIBUTE_INVASION)
	{
		int px = lpObj->X;
		int py = lpObj->Y;

		if(gObjGetRandomFreeLocation(lpObj->Map,&px,&py,5,5,30) == 0)
		{
			lpObj->Live = 0;
			lpObj->State = OBJECT_DYING;
			lpObj->DieRegen = 1;
			lpObj->RegenTime = GetTickCount();
			return 0;
		}

		lpObj->X = px;
		lpObj->Y = py;
	}
	else
	{
		if(gMonsterSetBase.GetPosition(lpObj->PosNum,lpObj->Map,&lpObj->X,&lpObj->Y) == 0)
		{
			lpObj->Live = 0;
			lpObj->State = OBJECT_DYING;
			lpObj->DieRegen = 1;
			lpObj->RegenTime = GetTickCount();
			return 0;
		}
	}

	lpObj->MTX = lpObj->X;
	lpObj->MTY = lpObj->Y;
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->StartX = (BYTE)lpObj->X;
	lpObj->StartY = (BYTE)lpObj->Y;
	lpObj->PathCur = 0;
	lpObj->PathCount = 0;
	lpObj->PathStartEnd = 0;

	gObjMonsterInitHitDamage(lpObj);

	return 1;
}

bool gObjMonsterMoveCheck(LPOBJ lpObj,int tx,int ty) // OK
{
	if(lpObj->ActionState.Emotion == EMOTION_ATTACK)
	{
		return 1;
	}

	if(lpObj->PosNum == -1 && lpObj->MoveRange != 0)
	{
		if(sqrt((pow((float)(tx-lpObj->StartX),(float)2)+pow((float)(ty-lpObj->StartY),(float)2))) > lpObj->MoveRange)
		{
			return 0;
		}

		return 1;
	}

	if(lpObj->PosNum == -1)
	{
		return 0;
	}

	if(gMonsterSetBase.m_MonsterSetBaseInfo[lpObj->PosNum].Dis == 0)
	{
		return 0;
	}

	if(sqrt((pow((float)(tx-lpObj->StartX),(float)2)+pow((float)(ty-lpObj->StartY),(float)2))) > gMonsterSetBase.m_MonsterSetBaseInfo[lpObj->PosNum].Dis)
	{
		return 0;
	}

	return 1;
}

void gObjMonsterInitHitDamage(LPOBJ lpObj) // OK
{
	for(int n=0;n < MAX_HIT_DAMAGE;n++)
	{
		lpObj->HitDamage[n].index = -1;
		lpObj->HitDamage[n].damage = 0;
		lpObj->HitDamage[n].time = 0;
	}
}

void gObjMonsterSetHitDamage(LPOBJ lpObj,int aIndex,int damage) // OK
{
	int HitDamageIndex = -1;

	for(int n=0;n < MAX_HIT_DAMAGE;n++)
	{
		if(OBJECT_RANGE(lpObj->HitDamage[n].index) == 0)
		{
			HitDamageIndex = n;
			continue;
		}

		if(lpObj->HitDamage[n].index == aIndex)
		{
			lpObj->HitDamage[n].damage = (((lpObj->HitDamage[n].damage+damage)>lpObj->MaxLife)?(int)lpObj->MaxLife:(lpObj->HitDamage[n].damage+damage));
			lpObj->HitDamage[n].time = GetTickCount();
			return;
		}
	}

	if(CHECK_RANGE(HitDamageIndex,MAX_HIT_DAMAGE) != 0 || OBJECT_RANGE(HitDamageIndex=gObjMonsterDelHitDamageUser(lpObj)) != 0)
	{
		lpObj->HitDamage[HitDamageIndex].index = aIndex;

		lpObj->HitDamage[HitDamageIndex].damage = ((damage>lpObj->MaxLife)?(int)lpObj->MaxLife:damage);

		lpObj->HitDamage[HitDamageIndex].time = GetTickCount();
	}
}

int gObjMonsterDelHitDamageUser(LPOBJ lpObj) // OK
{
	int HitDamageIndex = -1;

	for(int n=0;n < MAX_HIT_DAMAGE;n++)
	{
		if(OBJECT_RANGE(lpObj->HitDamage[n].index) == 0)
		{
			continue;
		}

		int result = 0;

		if(gObjIsConnected(lpObj->HitDamage[n].index) == 0)
		{
			result = 1;
		}
		else if((GetTickCount()-lpObj->HitDamage[n].time) > (DWORD)(gServerInfo.m_MonsterGetTopHitDamageUserMaxTime*1000))
		{
			result = 1;
		}
		else if(lpObj->Map != gObj[lpObj->HitDamage[n].index].Map)
		{
			result = 1;
		}
		else if(gObjCalcDistance(lpObj,&gObj[lpObj->HitDamage[n].index]) > 20)
		{
			result = 1;
		}

		if(result != 0)
		{
			lpObj->HitDamage[n].index = -1;
			HitDamageIndex = n;
		}
	}

	return HitDamageIndex;
}

int gObjMonsterGetTopHitDamageUser(LPOBJ lpObj) // OK
{
	int TopHitDamageUser = -1;
	int TopHitDamage = 0;
	int PartyTopHitDamageUser = -1;
	int PartyTopHitDamage = 0;

	for(int n=0;n < MAX_HIT_DAMAGE;n++)
	{
		if(gObjIsConnected(lpObj->HitDamage[n].index) == 0)
		{
			continue;
		}

		if((GetTickCount()-lpObj->HitDamage[n].time) > (DWORD)(gServerInfo.m_MonsterGetTopHitDamageUserMaxTime*1000))
		{
			continue;
		}

		if(lpObj->Map != gObj[lpObj->HitDamage[n].index].Map)
		{
			continue;
		}

		if(gObjCalcDistance(lpObj,&gObj[lpObj->HitDamage[n].index]) > 20)
		{
			continue;
		}

		if(OBJECT_RANGE(gObj[lpObj->HitDamage[n].index].PartyNumber) == 0)
		{
			if(lpObj->HitDamage[n].damage > TopHitDamage)
			{
				TopHitDamageUser = lpObj->HitDamage[n].index;
				TopHitDamage = lpObj->HitDamage[n].damage;
			}
		}
		else
		{
			if((PartyTopHitDamage=gObjMonsterGetTopHitDamageParty(lpObj,gObj[lpObj->HitDamage[n].index].PartyNumber,&PartyTopHitDamageUser)) > TopHitDamage)
			{
				TopHitDamageUser = PartyTopHitDamageUser;
				TopHitDamage = PartyTopHitDamage;
			}
		}
	}

	return TopHitDamageUser;
}

int gObjMonsterGetTopHitDamageParty(LPOBJ lpObj,int PartyNumber,int* TopHitDamageUser) // OK
{
	int TopHitDamage = 0;
	int TotalHitDamage = 0;

	for(int n=0;n < MAX_HIT_DAMAGE;n++)
	{
		if(gObjIsConnected(lpObj->HitDamage[n].index) == 0)
		{
			continue;
		}

		if((GetTickCount()-lpObj->HitDamage[n].time) > (DWORD)(gServerInfo.m_MonsterGetTopHitDamageUserMaxTime*1000))
		{
			continue;
		}

		if(lpObj->Map != gObj[lpObj->HitDamage[n].index].Map)
		{
			continue;
		}

		if(gObjCalcDistance(lpObj,&gObj[lpObj->HitDamage[n].index]) > 20)
		{
			continue;
		}

		for(int i=0;i < MAX_PARTY_USER;i++)
		{
			if(gParty.m_PartyInfo[PartyNumber].Index[i] == lpObj->HitDamage[n].index)
			{
				if(lpObj->HitDamage[n].damage > TopHitDamage)
				{
					(*TopHitDamageUser) = lpObj->HitDamage[n].index;
					TopHitDamage = lpObj->HitDamage[n].damage;
					TotalHitDamage += lpObj->HitDamage[n].damage;
				}
				else
				{
					(*TopHitDamageUser) = (*TopHitDamageUser);
					TopHitDamage = TopHitDamage;
					TotalHitDamage += lpObj->HitDamage[n].damage;
				}
			}
		}
	}

	return TotalHitDamage;
}

int gObjAddMonster(int map) // OK
{
	static CCriticalSection critical;

	critical.lock();

	int index = -1;
	int count = gObjMonCount;

	if(gMemoryAllocator.GetMemoryAllocatorFree(&index,OBJECT_START_MONSTER,MAX_OBJECT_MONSTER,10000) != 0)
	{
		gObjAllocData(index);
		gObjCharZeroSet(index);
		gObj[index].Index = index;
		gObj[index].Connected = OBJECT_CONNECTED;
		critical.unlock();
		return index;
	}

	for(int n=OBJECT_START_MONSTER;n < MAX_OBJECT_MONSTER;n++)
	{
		if(gObj[count].Connected == OBJECT_OFFLINE)
		{
			if(gObjAllocData(count) == 0)
			{
				break;
			}

			gObjCharZeroSet(count);
			gObj[count].Index = count;
			gObj[count].Connected = OBJECT_CONNECTED;
			critical.unlock();
			return count;
		}
		else
		{
			count = (((++count)>=MAX_OBJECT_MONSTER)?OBJECT_START_MONSTER:count);
		}
	}

	critical.unlock();
	return -1;
}

int gObjAddSummon() // OK
{
	static CCriticalSection critical;

	critical.lock();

	int index = -1;
	int count = gObjCallMonCount;

	if(gMemoryAllocator.GetMemoryAllocatorFree(&index,MAX_OBJECT_MONSTER,OBJECT_START_USER,10000) != 0)
	{
		gObjAllocData(index);
		gObjCharZeroSet(index);
		gObj[index].Index = index;
		gObj[index].Connected = OBJECT_CONNECTED;
		critical.unlock();
		return index;
	}

	for(int n=MAX_OBJECT_MONSTER;n < OBJECT_START_USER;n++)
	{
		if(gObj[count].Connected == OBJECT_OFFLINE)
		{
			if(gObjAllocData(count) == 0)
			{
				break;
			}

			gObjCharZeroSet(count);
			gObj[count].Index = count;
			gObj[count].Connected = OBJECT_CONNECTED;
			critical.unlock();
			return count;
		}
		else
		{
			count = (((++count)>=OBJECT_START_USER)?MAX_OBJECT_MONSTER:count);
		}
	}

	critical.unlock();
	return -1;
}

int gObjSummonSetEnemy(LPOBJ lpObj,int aIndex) // OK
{
	if(lpObj->Type != OBJECT_USER)
	{
		return -1;
	}

	LPOBJ lpSummon = &gObj[lpObj->SummonIndex];

	if(OBJECT_RANGE(lpSummon->TargetNumber) != 0)
	{
		return -1;
	}

	if(gObjIsConnected(aIndex) == 0 || aIndex == lpObj->Index || aIndex == lpSummon->Index)
	{
		return -1;
	}

	lpSummon->TargetNumber = aIndex;
	lpSummon->ActionState.Emotion = EMOTION_ATTACK;
	lpSummon->ActionState.EmotionCount = 30;
	return aIndex;
}

void gObjSummonKill(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(OBJECT_RANGE(lpObj->SummonIndex) == 0)
	{
		return;
	}

	LPOBJ lpSummon = &gObj[lpObj->SummonIndex];

	if(lpSummon->Type != OBJECT_MONSTER || lpSummon->SummonIndex != aIndex)
	{
		return;
	}

	if(gObjIsConnected(lpSummon->Index) != 0)
	{
		lpSummon->SummonIndex = -1;
	}

	lpObj->SummonIndex = -1;

	gObjDel(lpSummon->Index);
}

void gObjMonsterMagicAttack(LPOBJ lpObj,int skill) // OK
{
	CSkill* lpSkill = &lpObj->Skill[skill];

	if(lpSkill->IsSkill() == 0)
	{
		return;
	}

	gSkillManager.GCDurationSkillAttackSend(lpObj,lpSkill->m_index,(BYTE)lpObj->X,(BYTE)lpObj->Y,0);

	BYTE send[256];

	PMSG_MULTI_SKILL_ATTACK_RECV pMsg;

	pMsg.header.set(PROTOCOL_CODE4,sizeof(pMsg));

	int size = sizeof(pMsg);

	pMsg.skill = 0;

	pMsg.x = (BYTE)lpObj->X;

	pMsg.y = (BYTE)lpObj->Y;

	pMsg.serial = 0;

	pMsg.count = 0;

	PMSG_MULTI_SKILL_ATTACK info;

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state == VIEWPORT_NONE || OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0 || lpObj->VpPlayer2[n].type != OBJECT_USER)
		{
			continue;
		}

		if(gObjCalcDistance(lpObj,&gObj[lpObj->VpPlayer2[n].index]) >= 6)
		{
			continue;
		}

		info.index[0] = SET_NUMBERHB(lpObj->VpPlayer2[n].index);

		info.index[1] = SET_NUMBERLB(lpObj->VpPlayer2[n].index);

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		if(CHECK_SKILL_ATTACK_EXTENDED_COUNT(pMsg.count) == 0)
		{
			break;
		}
	}

	pMsg.header.size = size;

	memcpy(send,&pMsg,sizeof(pMsg));

	if(lpObj->Class == 161 || lpObj->Class == 181 || lpObj->Class == 189 || lpObj->Class == 197 || lpObj->Class == 267 || lpObj->Class == 275) // Kundum
	{
		gObjMonsterBeattackRecv(send,lpObj->Index);
	}
	else
	{
		gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)send,lpObj->Index,1);
	}
}

void gObjMonsterBeattackRecv(BYTE* aRecv,int aIndex) // OK
{
	PMSG_MULTI_SKILL_ATTACK_RECV* lpMsg = (PMSG_MULTI_SKILL_ATTACK_RECV*)aRecv;

	CSkill* lpSkill = &gObj[aIndex].Skill[lpMsg->skill];

	if(lpSkill->IsSkill() == 0)
	{
		return;
	}

	for(int n=0;n < lpMsg->count;n++)
	{
		PMSG_MULTI_SKILL_ATTACK* lpInfo = (PMSG_MULTI_SKILL_ATTACK*)(((BYTE*)lpMsg)+sizeof(PMSG_MULTI_SKILL_ATTACK_RECV)+(sizeof(PMSG_MULTI_SKILL_ATTACK)*n));

		int bIndex = MAKE_NUMBERW(lpInfo->index[0],lpInfo->index[1]);

		if(OBJECT_RANGE(bIndex) == 0)
		{
			continue;
		}

		gAttack.Attack(&gObj[aIndex],&gObj[bIndex],lpSkill,1,1,0,0,0);
	}
}

void gObjMonsterAttack(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(lpObj->Class == 66 || lpObj->Class == 73 || lpObj->Class == 77)
	{
		PMSG_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x19,sizeof(pMsg));

		pMsg.skill = 1;

		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

		pMsg.dis = 0;

		gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);
	}
	else if(lpObj->Class == 89 || lpObj->Class == 95 || lpObj->Class == 112 || lpObj->Class == 118 || lpObj->Class == 124 || lpObj->Class == 130 || lpObj->Class == 143)
	{
		PMSG_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x19,sizeof(pMsg));

		pMsg.skill = 1;

		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

		pMsg.dis = 0;

		gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);
	}
	else if(lpObj->Class == 144 || lpObj->Class == 174 || lpObj->Class == 182 || lpObj->Class == 190 || lpObj->Class == 260 || lpObj->Class == 268)
	{
		if((GetLargeRand()%2) == 0)
		{
			PMSG_SKILL_ATTACK_RECV pMsg;

			pMsg.header.set(0x19,sizeof(pMsg));

			pMsg.skill = 0;

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

			pMsg.dis = 0;

			gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);
		}
		else
		{
			PMSG_ATTACK_RECV pMsg;

			pMsg.header.set(PROTOCOL_CODE2,sizeof(pMsg));

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

			pMsg.action = ACTION_ATTACK1;

			pMsg.dir = lpObj->Dir;

			gAttack.CGAttackRecv(&pMsg,lpObj->Index);
		}
	}
	else if(lpObj->Class == 145 || lpObj->Class == 146 || lpObj->Class == 147 || lpObj->Class == 148 || lpObj->Class == 160 || lpObj->Class == 175 || lpObj->Class == 176 || lpObj->Class == 177 || lpObj->Class == 178 || lpObj->Class == 180 || lpObj->Class == 183 || lpObj->Class == 184 || lpObj->Class == 185 || lpObj->Class == 186 || lpObj->Class == 188 || lpObj->Class == 191 || lpObj->Class == 192 || lpObj->Class == 193 || lpObj->Class == 194 || lpObj->Class == 196 || lpObj->Class == 261 || lpObj->Class == 262 || lpObj->Class == 263 || lpObj->Class == 264 || lpObj->Class == 266 || lpObj->Class == 269 || lpObj->Class == 270 || lpObj->Class == 271 || lpObj->Class == 272 || lpObj->Class == 274)
	{
		if((GetLargeRand()%2) == 0)
		{
			PMSG_SKILL_ATTACK_RECV pMsg;

			pMsg.header.set(0x19,sizeof(pMsg));

			pMsg.skill = 0;

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

			pMsg.dis = 0;

			gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);
		}
		else
		{
			PMSG_ATTACK_RECV pMsg;

			pMsg.header.set(PROTOCOL_CODE2,sizeof(pMsg));

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

			pMsg.action = ACTION_ATTACK1;

			pMsg.dir = lpObj->Dir;

			gAttack.CGAttackRecv(&pMsg,lpObj->Index);
		}
	}
	else if(lpObj->Class == 149 || lpObj->Class == 179 || lpObj->Class == 187 || lpObj->Class == 195 || lpObj->Class == 265 || lpObj->Class == 273)
	{
		PMSG_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x19,sizeof(pMsg));

		pMsg.skill = (0+(GetLargeRand()%2));

		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

		pMsg.dis = 0;

		gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);
	}
	else if(lpObj->Class == 161 || lpObj->Class == 181 || lpObj->Class == 189 || lpObj->Class == 197 || lpObj->Class == 267 || lpObj->Class == 275)
	{
		PMSG_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x19,sizeof(pMsg));

		pMsg.skill = (1+(GetLargeRand()%6));

		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

		pMsg.dis = 0;

		gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);

		gObjMonsterMagicAttack(lpObj,0);
	}
	else if(lpObj->Class == 163 || lpObj->Class == 165 || lpObj->Class == 167 || lpObj->Class == 169 || lpObj->Class == 171 || lpObj->Class == 173)
	{
		PMSG_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x19,sizeof(pMsg));

		pMsg.skill = 0;

		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

		pMsg.dis = 0;

		gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);
	}
	else
	{
		if(lpObj->AttackType == 0 || lpObj->AttackType >= 100)
		{
			PMSG_ATTACK_RECV pMsg;

			pMsg.header.set(PROTOCOL_CODE2,sizeof(pMsg));

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

			pMsg.action = ACTION_ATTACK1;

			pMsg.dir = lpObj->Dir;

			gAttack.CGAttackRecv(&pMsg,lpObj->Index);
		}
		else
		{
			PMSG_SKILL_ATTACK_RECV pMsg;

			pMsg.header.set(0x19,sizeof(pMsg));

			pMsg.skill = 0;

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

			pMsg.dis = 0;

			gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);
		}
	}
}

void gObjMonsterDie(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	gObjMonsterDieGiveItem(lpObj,lpTarget);

	lpObj->NextActionTime = 500;

	if(OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		gObjSummonKill(lpObj->SummonIndex);
	}
}

void gObjMonsterStateProc(LPOBJ lpObj,int code,int aIndex,int SubCode) // OK
{
	switch(code)
	{
		case 0:
			gObjMonsterProcessHit(lpObj,aIndex);
			break;
		case 1:
			gObjMonsterDie(lpObj,&gObj[aIndex]);
			break;
		case 2:
			if(gObj[aIndex].Live != 0)
			{
				gObjBackSpring(lpObj,&gObj[aIndex]);
			}
			break;
		case 3:
			lpObj->TargetNumber = -1;
			lpObj->LastAttackerID = -1;
			lpObj->NextActionTime = 1000;
			lpObj->ActionState.Attack = 0;
			lpObj->ActionState.Move = 0;
			lpObj->ActionState.Emotion = EMOTION_REST;
			break;
		case 4:
			lpObj->ActionState.Emotion = EMOTION_ESCAPE;
			lpObj->ActionState.EmotionCount = 1;
			break;
		case 6:
			if(gObj[aIndex].Live != 0)
			{
				gObjBackSpring2(lpObj,&gObj[aIndex],2);
			}
			break;
		case 7:
			if(gObj[aIndex].Live != 0)
			{
				gObjBackSpring2(lpObj,&gObj[aIndex],3);
			}
			break;
	}
}

void gObjTrapFindTarget(LPOBJ lpObj) // OK
{
	int IndexTable[MAX_VIEWPORT];
	int IndexCount = 0;

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state == VIEWPORT_NONE || OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0 || lpObj->VpPlayer2[n].type != OBJECT_USER)
		{
			continue;
		}

		if(gObj[lpObj->VpPlayer2[n].index].Authority == AUTHORITY_INVISIBLE || gObj[lpObj->VpPlayer2[n].index].Authority == AUTHORITY_ADMINISTRATOR)
		{
			continue;
		}

		if(gSkillManager.CheckSkillTarget(lpObj,lpObj->VpPlayer2[n].index,-1,lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if(gViewport.CheckViewportObjectPosition(lpObj->Index,gObj[lpObj->VpPlayer2[n].index].Map,gObj[lpObj->VpPlayer2[n].index].X,gObj[lpObj->VpPlayer2[n].index].Y,lpObj->AttackRange) == 0)
		{
			continue;
		}

		IndexTable[IndexCount++] = lpObj->VpPlayer2[n].index;

		if(lpObj->AttackRange == 0)
		{
			break;
		}
	}

	lpObj->TargetNumber = ((IndexCount==0)?-1:IndexTable[GetLargeRand()%IndexCount]);

	if(OBJECT_RANGE(lpObj->TargetNumber) != 0)
	{
		lpObj->ActionState.Attack = 1;
		lpObj->NextActionTime = lpObj->PhysiSpeed;
	}
	else
	{
		lpObj->NextActionTime = lpObj->MoveSpeed;
	}
}

void gObjMonsterUpdateProc(LPOBJ lpObj) // OK
{
	if(lpObj->Live == 0 || lpObj->State != OBJECT_PLAYING)
	{
		return;
	}

	if((GetTickCount()-lpObj->CurActionTime) < (lpObj->NextActionTime+lpObj->DelayActionTime))
	{
		return;
	}

	lpObj->CurActionTime = GetTickCount();

	if(lpObj->Attribute == ATTRIBUTE_KALIMA || lpObj->Class == 131 || lpObj->Class == 132 || lpObj->Class == 133 || lpObj->Class == 134)
	{
		return;
	}

	if(lpObj->Class >= 100 && lpObj->Class <= 110) // Trap
	{
		gObjTrapFindTarget(lpObj);
	}
	else if(lpObj->Class == 200) // Soccer Ball
	{
		gBattleSoccer.MonsterProc(lpObj);
		return;
	}
	else
	{
		gObjMonsterReactionProc(lpObj);
	}

	if(lpObj->Attribute == ATTRIBUTE_SUMMONED && OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		LPOBJ lpSummon = &gObj[lpObj->SummonIndex];

		if(lpObj->Map != lpSummon->Map || gObjCalcDistance(lpObj,lpSummon) >= lpObj->MoveRange)
		{
			gObjTeleportMagicUse(lpObj->Index,lpSummon->X+1,lpSummon->Y);
			lpObj->Map = lpSummon->Map;
			return;
		}
	}

	if(lpObj->ActionState.Move == 1)
	{
		gObjMonsterFindPathToMove(lpObj);
		lpObj->ActionState.Move = 0 ;
		return;		
	}

	if(lpObj->ActionState.Attack == 1)
	{
		if(OBJECT_RANGE(lpObj->TargetNumber) == 0 || gObj[lpObj->TargetNumber].Connected != OBJECT_ONLINE || gObj[lpObj->TargetNumber].CloseCount > 0 || gObj[lpObj->TargetNumber].Live == 0 || gObj[lpObj->TargetNumber].Teleport != 0)
		{
			lpObj->TargetNumber = -1;
			lpObj->LastAttackerID = -1;
			lpObj->NextActionTime = 1000;
			lpObj->ActionState.Attack = 0;
			lpObj->ActionState.Move = 0;
			lpObj->ActionState.Emotion = EMOTION_REST;
			return;
		}

		lpObj->Dir = GetPathPacketDirPos((gObj[lpObj->TargetNumber].X-lpObj->X),(gObj[lpObj->TargetNumber].Y-lpObj->Y));

		if(lpObj->AttackType == 50 || (lpObj->AttackType >= 100 && (GetLargeRand()%5) == 0))
		{
			gObjMonsterMagicAttack(lpObj,0);
		}
		else
		{
			gObjMonsterAttack(lpObj,&gObj[lpObj->TargetNumber]);
		}

		lpObj->ActionState.Attack = 0;
	}
}

void gObjMonsterReactionProc(LPOBJ lpObj) // OK
{
	if(OBJECT_RANGE(lpObj->TargetNumber) == 0)
	{
		lpObj->ActionState.Emotion = EMOTION_REST;
	}

	if(lpObj->ActionState.Emotion == EMOTION_REST)
	{
		if(lpObj->Attribute != ATTRIBUTE_NONE)
		{
			if(lpObj->ActionState.Attack == 1)
			{
				lpObj->ActionState.Attack = 0;
				lpObj->TargetNumber = -1;
				lpObj->NextActionTime = 500;
			}
			
			int action_code = lpObj->Attribute == ATTRIBUTE_SUMMONED ? 1 : (GetLargeRand()%2);

			if(action_code == 0)
			{
				lpObj->NextActionTime = 500;
			}
			else if(lpObj->MoveRange > 0 && gMonsterCheckMovingCondition(lpObj) != 0)
			{
				if(lpObj->Attribute != ATTRIBUTE_SUMMONED)
				{
					gObjMonsterGetXYToPatrol(lpObj);
				}
				else
				{
					gObjMonsterGetXYToOwner(lpObj);
				}
			}
			
			gObjMonsterFindTarget(lpObj);

			if(OBJECT_RANGE(lpObj->TargetNumber) != 0)
			{
				lpObj->ActionState.EmotionCount = 30;
				lpObj->ActionState.Emotion = EMOTION_ATTACK;
			}
		}
	}
	else if(lpObj->ActionState.Emotion == EMOTION_ATTACK)
	{
		lpObj->ActionState.Update();

		if(OBJECT_RANGE(lpObj->TargetNumber) != 0 && lpObj->PathStartEnd == 0)
		{
			if(gObjCalcDistance(lpObj,&gObj[lpObj->TargetNumber]) <= (lpObj->AttackType>=100?(lpObj->AttackRange+2):lpObj->AttackRange))
			{
				if(gMap[lpObj->Map].CheckWall(lpObj->X,lpObj->Y,gObj[lpObj->TargetNumber].X,gObj[lpObj->TargetNumber].Y) != 0)
				{
					if(gMap[lpObj->Map].CheckAttr(gObj[lpObj->TargetNumber].X,gObj[lpObj->TargetNumber].Y,1) == 0)
					{
						lpObj->ActionState.Attack = 1;
					}
					else
					{
						lpObj->TargetNumber = -1;
						lpObj->ActionState.EmotionCount = 30;
						lpObj->ActionState.Emotion = EMOTION_ATTACK;
					}

					lpObj->Dir = GetPathPacketDirPos(gObj[lpObj->TargetNumber].X-lpObj->X,gObj[lpObj->TargetNumber].Y-lpObj->Y);
					lpObj->NextActionTime = lpObj->PhysiSpeed;
				}
				else
				{
					lpObj->TargetNumber = -1;
					lpObj->ActionState.Attack = 0;
					lpObj->NextActionTime = 500;
					lpObj->ActionState.Emotion = EMOTION_REST;
					lpObj->ActionState.Move = 1;
				}
			}
			else
			{
				if(gObjMonsterGetXYToChase(lpObj) != 0)
				{
					if(gMap[lpObj->Map].CheckWall(lpObj->X,lpObj->Y,lpObj->MTX,lpObj->MTY) != 0)
					{
						lpObj->ActionState.Move = 1;
						lpObj->NextActionTime = 400;
						lpObj->Dir = GetPathPacketDirPos(gObj[lpObj->TargetNumber].X-lpObj->X,gObj[lpObj->TargetNumber].Y-lpObj->Y);
					}
					else
					{
						gObjMonsterGetXYToPatrol(lpObj);
						lpObj->ActionState.Emotion = EMOTION_ESCAPE;
						lpObj->ActionState.EmotionCount = 10;
					}
				}
				else
				{
					gObjMonsterGetXYToPatrol(lpObj);
				}
			}
		}
	}
	else if(lpObj->ActionState.Emotion == EMOTION_MOVE)
	{
		lpObj->ActionState.Update();
		lpObj->ActionState.Move = 1;
		lpObj->NextActionTime = 800;

		if(OBJECT_RANGE(lpObj->TargetNumber) != 0)
		{
			int tdir = GetPathPacketDirPos(gObj[lpObj->TargetNumber].X-lpObj->X,gObj[lpObj->TargetNumber].Y-lpObj->Y)*2;
			lpObj->MTX += RoadPathTable[tdir]*(-3);
			lpObj->MTY += RoadPathTable[tdir+1]*(-3);
		}
	}
	else if(lpObj->ActionState.Emotion == EMOTION_ESCAPE)
	{
		lpObj->ActionState.Update();
		lpObj->ActionState.Move = 0;
		lpObj->ActionState.Attack = 0;
		lpObj->NextActionTime = 400;
	}
}

void gObjMonsterGetXYToPatrol(LPOBJ lpObj) // OK
{
	if(gMonsterCheckMovingCondition(lpObj) == 0)
	{
		return;
	}

	lpObj->NextActionTime = 1000;

	for(int n=0;n < 10;n++)
	{
		int tx = (lpObj->X-lpObj->MoveRange)+(GetLargeRand()%((lpObj->MoveRange*2)+1));
		int ty = (lpObj->Y-lpObj->MoveRange)+(GetLargeRand()%((lpObj->MoveRange*2)+1));

		if(gObjMonsterMoveCheck(lpObj,tx,ty) != 0)
		{
			if(lpObj->Class == 247 || lpObj->Class == 249)
			{
				if(gMap[lpObj->Map].CheckAttr(tx,ty,2) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,4) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,8) == 0)
				{
					lpObj->MTX = tx;
					lpObj->MTY = ty;
					lpObj->ActionState.Move = 1;
					lpObj->NextActionTime = 3000;
					return;
				}
			}
			else
			{
				if(gMap[lpObj->Map].CheckAttr(tx,ty,1) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,2) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,4) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,8) == 0)
				{
					lpObj->ActionState.Emotion = EMOTION_REST;
					lpObj->MTX = tx;
					lpObj->MTY = ty;
					lpObj->ActionState.Move = 1;
					lpObj->NextActionTime = 500;
					lpObj->TargetNumber = -1;
					lpObj->ActionState.Attack = 0;
					return;
				}
			}
		}
	}
}

bool gObjMonsterGetXYToChase(LPOBJ lpObj) // OK
{
	if(gMonsterCheckMovingCondition(lpObj) == 0)
	{
		return 0;
	}

	if(gMonsterFindMonViewportObj(lpObj->Index,lpObj->TargetNumber) == 0)
	{
		return 0;
	}

	if(OBJECT_RANGE(lpObj->TargetNumber) == 0 || gObj[lpObj->TargetNumber].Teleport != 0)
	{
		return 0;
	}

	int sx = gObj[lpObj->TargetNumber].X;
	int sy = gObj[lpObj->TargetNumber].Y;
	int tx = gObj[lpObj->TargetNumber].X;
	int ty = gObj[lpObj->TargetNumber].Y;
	int dis = (lpObj->AttackType>=100?(lpObj->AttackRange+2):lpObj->AttackRange);

	if(lpObj->X < tx)
	{
		sx -= dis;
	}

	if(lpObj->X > tx)
	{
		sx += dis;
	}

	if(lpObj->Y < ty)
	{
		sy -= dis;
	}

	if(lpObj->Y > ty)
	{
		sy += dis;
	}

	int PathValue = GetPathPacketDirPos((gObj[lpObj->TargetNumber].X-sx),(gObj[lpObj->TargetNumber].Y-sy))*2;

	if(gMap[lpObj->Map].CheckStandAttr(sx,sy) == 0)
	{
		for(int n=0;n < (MAX_ROAD_PATH_TABLE/2);n++)
		{
			tx = gObj[lpObj->TargetNumber].X+RoadPathTable[PathValue++];
			ty = gObj[lpObj->TargetNumber].Y+RoadPathTable[PathValue++];

			if(gObjMonsterMoveCheck(lpObj,tx,ty) != 0)
			{
				if(gMap[lpObj->Map].CheckAttr(tx,ty,1) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,2) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,4) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,8) == 0)
				{
					lpObj->MTX = tx;
					lpObj->MTY = ty;
					return 1;
				}
			}

			PathValue = ((PathValue>=MAX_ROAD_PATH_TABLE)?0:PathValue);
		}
	}
	else
	{
		if(gObjMonsterMoveCheck(lpObj,sx,sy) != 0)
		{
			if(gMap[lpObj->Map].CheckAttr(sx,sy,1) == 0 && gMap[lpObj->Map].CheckAttr(sx,sy,2) == 0 && gMap[lpObj->Map].CheckAttr(sx,sy,4) == 0 && gMap[lpObj->Map].CheckAttr(sx,sy,8) == 0)
			{
				lpObj->MTX = sx;
				lpObj->MTY = sy;
				return 1;
			}
		}
	}

	return 0;
}

void gObjMonsterGetXYToOwner(LPOBJ lpObj) // OK
{
	if(OBJECT_RANGE(lpObj->SummonIndex) == 0)
	{
		return;
	}

	if(gObj[lpObj->SummonIndex].Rest != 0)
	{
		return;
	}

	int sx = gObj[lpObj->SummonIndex].X;
	int sy = gObj[lpObj->SummonIndex].Y;
	int tx = gObj[lpObj->SummonIndex].X;
	int ty = gObj[lpObj->SummonIndex].Y;
	int dis = (lpObj->AttackType>=100?(lpObj->AttackRange+2):lpObj->AttackRange);

	if(lpObj->X < tx)
	{
		sx -= dis;
	}

	if(lpObj->X > tx)
	{
		sx += dis;
	}

	if(lpObj->Y < ty)
	{
		sy -= dis;
	}

	if(lpObj->Y > ty)
	{
		sy += dis;
	}

	int PathValue = GetPathPacketDirPos((gObj[lpObj->SummonIndex].X-sx),(gObj[lpObj->SummonIndex].Y-sy))*2;

	if(gMap[lpObj->Map].CheckStandAttr(sx,sy) == 0)
	{
		for(int n=0;n < (MAX_ROAD_PATH_TABLE/2);n++)
		{
			tx = gObj[lpObj->SummonIndex].X+RoadPathTable[PathValue++];
			ty = gObj[lpObj->SummonIndex].Y+RoadPathTable[PathValue++];

			if(gMap[lpObj->Map].CheckAttr(tx,ty,2) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,4) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,8) == 0)
			{
				lpObj->MTX = tx;
				lpObj->MTY = ty;
				lpObj->ActionState.Move = 1;
				lpObj->NextActionTime = 1000;
				return;
			}

			PathValue = ((PathValue>=MAX_ROAD_PATH_TABLE)?0:PathValue);
		}
	}
	else
	{
		if(gMap[lpObj->Map].CheckAttr(sx,sy,2) == 0 && gMap[lpObj->Map].CheckAttr(sx,sy,4) == 0 && gMap[lpObj->Map].CheckAttr(sx,sy,8) == 0)
		{
			lpObj->MTX = sx;
			lpObj->MTY = sy;
			lpObj->ActionState.Move = 1;
			lpObj->NextActionTime = 1000;
			return ;
		}
	}
}

void gObjMonsterFindTarget(LPOBJ lpObj) // OK
{
	int IndexTable[MAX_VIEWPORT];
	int IndexCount = 0;

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state == VIEWPORT_NONE || OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0)
		{
			continue;
		}

		if(gObj[lpObj->VpPlayer2[n].index].Authority == AUTHORITY_INVISIBLE || gObj[lpObj->VpPlayer2[n].index].Authority == AUTHORITY_ADMINISTRATOR)
		{
			continue;
		}

		if(gViewport.CheckViewportObjectPosition(lpObj->Index,gObj[lpObj->VpPlayer2[n].index].Map,gObj[lpObj->VpPlayer2[n].index].X,gObj[lpObj->VpPlayer2[n].index].Y,lpObj->ViewRange) == 0)
		{
			continue;
		}

		if(lpObj->Attribute == ATTRIBUTE_SUMMONED && lpObj->VpPlayer2[n].type == OBJECT_USER)
		{
			continue;
		}

		if((lpObj->Class == 247 || lpObj->Class == 249) && gObj[lpObj->VpPlayer2[n].index].PKLevel > 4)
		{
			IndexTable[IndexCount++] = lpObj->VpPlayer2[n].index;
			continue;
		}

		if(gSkillManager.CheckSkillTarget(lpObj,lpObj->VpPlayer2[n].index,-1,lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		IndexTable[IndexCount++] = lpObj->VpPlayer2[n].index;
	}

	lpObj->TargetNumber = ((IndexCount==0)?-1:IndexTable[GetLargeRand()%IndexCount]);
}

void gObjMonsterFindPathToMove(LPOBJ lpObj) // OK
{
	if(gEffectManager.CheckImmobilizeEffect(lpObj) != 0)
	{
		return;
	}

	PATH_INFO path;

	BYTE bPath[8];

	if(gMap[lpObj->Map].PathFinding2(lpObj->X,lpObj->Y,lpObj->MTX,lpObj->MTY,&path) != 0)
	{
		int pos = 0;

		memset(bPath,0,sizeof(bPath));

		bPath[0] = (lpObj->Dir<<4)+((path.PathNum-1)&0x0F);

		int sx = lpObj->X;
		int sy = lpObj->Y;
		lpObj->PathX[0] = lpObj->X;
		lpObj->PathY[0] = lpObj->Y;
		lpObj->PathDir[0] = lpObj->Dir;

		for (int n = 1; n < path.PathNum; n++)
		{
			int tx = path.PathX[n];
			int ty = path.PathY[n];

			pos = GetPathPacketDirPos(tx - sx, ty - sy);

			lpObj->PathX[n] = tx;
			lpObj->PathY[n] = ty;
			lpObj->PathDir[n] = pos;

			sx = tx;
			sy = ty;

			if ((n % 2) == 1)
			{
				bPath[(n+1)/2] = pos<<4;
			}
			else
			{
				bPath[(n+1)/2] |= pos;
			}
		}

		lpObj->Dir = bPath[0] >> 4;
		lpObj->PathCount = (bPath[0] & 0x0f);
		lpObj->PathCur = 1;
		lpObj->PathTime = GetTickCount();

		bPath[0] &= 0x0F;
		bPath[0] |= pos << 4;

		if(lpObj->PathCount > 0)
		{
			gMap[lpObj->Map].DelStandAttr(lpObj->OldX,lpObj->OldY);
			gMap[lpObj->Map].SetStandAttr(sx, sy);
			lpObj->OldX = sx;
			lpObj->OldY = sy;
		}

		PMSG_MOVE_RECV pMsg;

		pMsg.header.set(PROTOCOL_CODE1,sizeof(pMsg));

		pMsg.x = (BYTE)lpObj->X;

		pMsg.y = (BYTE)lpObj->Y;

		memcpy(pMsg.path,bPath,sizeof(pMsg.path));

		CGMoveRecv((PMSG_MOVE_RECV*)&pMsg,lpObj->Index);
	}
}

bool gMonsterCheckMovingCondition(LPOBJ lpObj) // OK
{
	if(lpObj->Live == 0 || lpObj->RegenOk > 0 || lpObj->Teleport != 0 || lpObj->State != OBJECT_PLAYING)
	{
		return 0;
	}

	if(gEffectManager.CheckImmobilizeEffect(lpObj) != 0)
	{
		return 0;
	}

	return 1;
}

bool gMonsterFindMonViewportObj(int aIndex,int bIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0 || OBJECT_RANGE(bIndex) == 0)
	{
		return 0;
	}

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(gObj[aIndex].VpPlayer2[n].state != VIEWPORT_NONE)
		{
			if(gObj[aIndex].VpPlayer2[n].index == bIndex)
			{
				return 1;
			}
		}
	}

	return 0;
}

void gObjMonsterProcessHit(LPOBJ lpObj,int aIndex) // OK
{
	if(lpObj->Attribute == ATTRIBUTE_NONE)
	{
		return;
	}

	if(OBJECT_RANGE(aIndex) == 0 || gObj[aIndex].Live == 0 || gObj[aIndex].State != OBJECT_PLAYING)
	{
		return;
	}

	if(lpObj->TargetNumber == aIndex)
	{
		return;
	}

	if(gMap[lpObj->Map].CheckWall(lpObj->X,lpObj->Y,gObj[aIndex].X,gObj[aIndex].Y) == 0)
	{
		return;
	}

	if(gMap[gObj[aIndex].Map].CheckAttr(gObj[aIndex].X,gObj[aIndex].Y,1) != 0)
	{
		return;
	}

	if(lpObj->ActionState.Emotion == EMOTION_REST)
	{
		lpObj->ActionState.Emotion = EMOTION_ATTACK;
		lpObj->ActionState.EmotionCount = 10;
	}
	else if(lpObj->ActionState.Emotion == EMOTION_ATTACK)
	{
		lpObj->ActionState.EmotionCount = 10;
	}

	if(lpObj->ActionState.Attack == 0)
	{
		if(gObjCalcDistance(lpObj,&gObj[aIndex]) <= (lpObj->AttackType>=100?(lpObj->AttackRange+2):lpObj->AttackRange))
		{
			if(gObj[aIndex].Attribute == ATTRIBUTE_SUMMONED || (GetLargeRand()%100) < 90)
			{
				lpObj->TargetNumber = aIndex;
			}
		}
		else
		{
			if(OBJECT_RANGE(lpObj->TargetNumber) == 0)
			{
				lpObj->TargetNumber = aIndex;
			}
		}
	}
	else
	{
		if((GetLargeRand()%2) == 1)
		{
			lpObj->ActionState.Attack = 1;
			lpObj->TargetNumber = aIndex;
		}
	}
}
