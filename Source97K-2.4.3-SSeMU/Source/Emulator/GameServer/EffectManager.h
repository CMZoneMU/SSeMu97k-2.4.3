// EffectManager.h: interface for the CEffectManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Effect.h"
#include "Protocol.h"
#include "User.h"

#define MAX_EFFECT 128
#define MAX_EFFECT_LIST 16

#define EFFECT_RANGE(x) (((x)<0)?0:((x)>=MAX_EFFECT)?0:1)

enum eEffectNumber
{
	EFFECT_GREATER_DAMAGE = 1,
	EFFECT_GREATER_DEFENSE = 2,
	EFFECT_MANA_SHIELD = 4,
	EFFECT_GREATER_CRITICAL_DAMAGE = 5,
	EFFECT_GREATER_LIFE = 8,
	EFFECT_INVISIBILITY = 18,
	EFFECT_POISON = 55,
	EFFECT_ICE = 56,
	EFFECT_ICE_ARROW = 57,
	EFFECT_FIRE_SLASH = 58,
	EFFECT_PHYSI_DAMAGE_IMMUNITY = 59,
	EFFECT_MAGIC_DAMAGE_IMMUNITY = 60,
	EFFECT_MONSTER_PHYSI_DAMAGE_IMMUNITY = 63,
	EFFECT_MONSTER_MAGIC_DAMAGE_IMMUNITY = 64,
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_EFFECT_STATE_SEND
{
	PBMSG_HEAD header; // C1:07
	BYTE state;
	WORD effect;
	BYTE index[2];
};

//**********************************************//
//**********************************************//
//**********************************************//

struct EFFECT_INFO
{
	int Index;
	int Group;
	int ItemIndex;
	char Name[32];
	int Save;
	int Type;
	int Flag;
	int Count;
	int Value[4];
};

class CEffectManager
{
public:
	CEffectManager();
	virtual ~CEffectManager();
	void Init();
	void Load(char* path);
	void SetInfo(EFFECT_INFO info);
	EFFECT_INFO* GetInfo(int index);
	EFFECT_INFO* GetInfoByItem(int ItemIndex);
	void MainProc();
	bool AddEffect(LPOBJ lpObj,bool type,int index,int count,WORD value1,WORD value2,WORD value3,WORD value4);
	bool DelEffect(LPOBJ lpObj,int index);
	bool DelEffectByGroup(LPOBJ lpObj,int group);
	CEffect* GetEffect(LPOBJ lpObj,int index);
	CEffect* GetEffectByGroup(LPOBJ lpObj,int group);
	bool CheckEffect(LPOBJ lpObj,int index);
	void InsertEffect(LPOBJ lpObj,CEffect* lpEffect);
	void RemoveEffect(LPOBJ lpObj,CEffect* lpEffect);
	bool ConvertEffectByte(CEffect* lpEffect,BYTE* lpMsg);
	void EffectByteConvert(BYTE* lpMsg,CEffect* lpEffect);
	WORD GetViewSkillState(BYTE effect);
	bool CheckImmobilizeEffect(LPOBJ lpObj);
	void ClearAllEffect(LPOBJ lpObj);
	void PeriodicEffect(LPOBJ lpObj,CEffect* lpEffect);
	void GCEffectStateSend(LPOBJ lpObj,BYTE state,BYTE effect);
private:
	EFFECT_INFO m_EffectInfo[MAX_EFFECT];
};

extern CEffectManager gEffectManager;
