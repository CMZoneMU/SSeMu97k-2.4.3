// EffectManager.cpp: implementation of the CEffectManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectManager.h"
#include "Attack.h"
#include "Fruit.h"
#include "ItemManager.h"
#include "Map.h"
#include "MemScript.h"
#include "ObjectManager.h"
#include "Party.h"
#include "ServerInfo.h"
#include "Util.h"
#include "Viewport.h"

CEffectManager gEffectManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectManager::CEffectManager() // OK
{
	this->Init();
}

CEffectManager::~CEffectManager() // OK
{

}

void CEffectManager::Init() // OK
{
	for(int n=0;n < MAX_EFFECT;n++)
	{
		this->m_EffectInfo[n].Index = -1;
	}
}

void CEffectManager::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->Init();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			EFFECT_INFO info;

			info.Index = lpMemScript->GetNumber();

			info.Group = lpMemScript->GetAsNumber();

			info.ItemIndex = lpMemScript->GetAsNumber();

			info.ItemIndex = (info.ItemIndex!=-1)?SafeGetItem(GET_ITEM(info.ItemIndex,lpMemScript->GetAsNumber())):info.ItemIndex;

			strcpy_s(info.Name,lpMemScript->GetAsString());

			info.Save = lpMemScript->GetAsNumber();

			info.Type = lpMemScript->GetAsNumber();

			info.Flag = lpMemScript->GetAsNumber();

			info.Count = lpMemScript->GetAsNumber();

			info.Value[0] = lpMemScript->GetAsNumber();

			info.Value[1] = lpMemScript->GetAsNumber();

			info.Value[2] = lpMemScript->GetAsNumber();

			info.Value[3] = lpMemScript->GetAsNumber();

			this->SetInfo(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CEffectManager::SetInfo(EFFECT_INFO info) // OK
{
	if(EFFECT_RANGE(info.Index) == 0)
	{
		return;
	}

	this->m_EffectInfo[info.Index] = info;
}

EFFECT_INFO* CEffectManager::GetInfo(int index) // OK
{
	if(EFFECT_RANGE(index) == 0)
	{
		return 0;
	}

	if(this->m_EffectInfo[index].Index != index)
	{
		return 0;
	}

	return &this->m_EffectInfo[index];
}

EFFECT_INFO* CEffectManager::GetInfoByItem(int ItemIndex) // OK
{
	for(int n=0;n < MAX_EFFECT;n++)
	{
		EFFECT_INFO* lpInfo = this->GetInfo(n);

		if(lpInfo == 0)
		{
			continue;
		}

		if(lpInfo->ItemIndex == ItemIndex)
		{
			return lpInfo;
		}
	}

	return 0;
}

void CEffectManager::MainProc() // OK
{
	for(int n=0;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[n];

		for(int i=0;i < MAX_EFFECT_LIST;i++)
		{
			CEffect* lpEffect = &lpObj->Effect[i];

			if(lpEffect->IsEffect() == 0)
			{
				continue;
			}

			if(((lpEffect->m_count==0)?0:(lpEffect->m_count--)) > 0)
			{
				this->PeriodicEffect(lpObj,lpEffect);

				if(lpEffect->m_count == 0)
				{
					this->RemoveEffect(lpObj,lpEffect);
					this->GCEffectStateSend(lpObj,0,lpEffect->m_index);
					lpEffect->Clear();
				}
			}
		}
	}
}

bool CEffectManager::AddEffect(LPOBJ lpObj,bool type,int index,int count,WORD value1,WORD value2,WORD value3,WORD value4) // OK
{
	EFFECT_INFO* lpInfo = this->GetInfo(index);

	if(lpInfo == 0)
	{
		return 0;
	}

	if(type == 0 && lpInfo->Count != -1)
	{
		count = ((lpInfo->Type==2)?((int)time(0)+lpInfo->Count):lpInfo->Count);
	}

	if(lpInfo->Value[0] != -1)
	{
		value1 = lpInfo->Value[0];
	}

	if(lpInfo->Value[1] != -1)
	{
		value2 = lpInfo->Value[1];
	}

	if(lpInfo->Value[2] != -1)
	{
		value3 = lpInfo->Value[2];
	}

	if(lpInfo->Value[3] != -1)
	{
		value4 = lpInfo->Value[3];
	}

	CEffect* lpEffect = this->GetEffectByGroup(lpObj,lpInfo->Group);

	if(lpEffect == 0)
	{
		for(int n=0;n < MAX_EFFECT_LIST;n++)
		{
			lpEffect = &lpObj->Effect[n];

			if(lpEffect->Set(index,lpInfo->Group,lpInfo->Save,lpInfo->Type,lpInfo->Flag,0,count,value1,value2,value3,value4) != 0)
			{
				this->InsertEffect(lpObj,lpEffect);
				this->GCEffectStateSend(lpObj,1,lpEffect->m_index);
				return 1;
			}
		}
	}
	else
	{
		if(index != lpEffect->m_index || gServerInfo.m_EffectOverwriteMode != 0 || (value1 >= lpEffect->m_value[0] && value2 >= lpEffect->m_value[1] && value3 >= lpEffect->m_value[2] && value4 >= lpEffect->m_value[3]))
		{
			this->RemoveEffect(lpObj,lpEffect);
			lpEffect->Clear();

			if(lpEffect->Set(index,lpInfo->Group,lpInfo->Save,lpInfo->Type,lpInfo->Flag,0,count,value1,value2,value3,value4) != 0)
			{
				this->InsertEffect(lpObj,lpEffect);
				this->GCEffectStateSend(lpObj,1,lpEffect->m_index);
				return 1;
			}
		}
	}

	return 0;
}

bool CEffectManager::DelEffect(LPOBJ lpObj,int index) // OK
{
	for(int n=0;n < MAX_EFFECT_LIST;n++)
	{
		CEffect* lpEffect = &lpObj->Effect[n];

		if(lpEffect->IsEffect() == 0)
		{
			continue;
		}

		if(lpEffect->m_index == index)
		{
			this->RemoveEffect(lpObj,lpEffect);
			this->GCEffectStateSend(lpObj,0,lpEffect->m_index);
			lpEffect->Clear();
			return 1;
		}
	}

	return 0;
}

bool CEffectManager::DelEffectByGroup(LPOBJ lpObj,int group) // OK
{
	for(int n=0;n < MAX_EFFECT_LIST;n++)
	{
		CEffect* lpEffect = &lpObj->Effect[n];

		if(lpEffect->IsEffect() == 0)
		{
			continue;
		}

		if(lpEffect->m_group == group)
		{
			this->RemoveEffect(lpObj,lpEffect);
			this->GCEffectStateSend(lpObj,0,lpEffect->m_index);
			lpEffect->Clear();
			return 1;
		}
	}

	return 0;
}

CEffect* CEffectManager::GetEffect(LPOBJ lpObj,int index) // OK
{
	for(int n=0;n < MAX_EFFECT_LIST;n++)
	{
		CEffect* lpEffect = &lpObj->Effect[n];

		if(lpEffect->IsEffect() == 0)
		{
			continue;
		}

		if(lpEffect->m_index == index)
		{
			return lpEffect;
		}
	}

	return 0;
}

CEffect* CEffectManager::GetEffectByGroup(LPOBJ lpObj,int group) // OK
{
	for(int n=0;n < MAX_EFFECT_LIST;n++)
	{
		CEffect* lpEffect = &lpObj->Effect[n];

		if(lpEffect->IsEffect() == 0)
		{
			continue;
		}

		if(lpEffect->m_group == group)
		{
			return lpEffect;
		}
	}

	return 0;
}

bool CEffectManager::CheckEffect(LPOBJ lpObj,int index) // OK
{
	if(this->GetEffect(lpObj,index) != 0)
	{
		return 1;
	}

	return 0;
}

void CEffectManager::InsertEffect(LPOBJ lpObj,CEffect* lpEffect) // OK
{
	lpObj->ViewSkillState |= this->GetViewSkillState(lpEffect->m_index);

	switch(lpEffect->m_index)
	{
		case EFFECT_GREATER_DAMAGE:
			lpObj->EffectOption.AddPhysiDamage += lpEffect->m_value[0];
			lpObj->EffectOption.AddMagicDamage += lpEffect->m_value[0];
			break;
		case EFFECT_GREATER_DEFENSE:
			lpObj->EffectOption.AddDefense += lpEffect->m_value[0];
			break;
		case EFFECT_MANA_SHIELD:
			lpObj->EffectOption.AddDamageReduction += lpEffect->m_value[0];
			break;
		case EFFECT_GREATER_CRITICAL_DAMAGE:
			lpObj->EffectOption.AddCriticalDamage += lpEffect->m_value[0];
			break;
		case EFFECT_GREATER_LIFE:
			lpObj->EffectOption.MulMaxHP += lpEffect->m_value[0];
			break;
		case EFFECT_ICE:
			lpObj->DelayLevel = 1;
			lpObj->DelayActionTime = 800;
			break;
		case EFFECT_FIRE_SLASH:
			lpObj->EffectOption.DivDefense += lpEffect->m_value[0];
			break;
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);
}

void CEffectManager::RemoveEffect(LPOBJ lpObj,CEffect* lpEffect) // OK
{
	lpObj->ViewSkillState &= ~this->GetViewSkillState(lpEffect->m_index);

	switch(lpEffect->m_index)
	{
		case EFFECT_GREATER_DAMAGE:
			lpObj->EffectOption.AddPhysiDamage -= lpEffect->m_value[0];
			lpObj->EffectOption.AddMagicDamage -= lpEffect->m_value[0];
			break;
		case EFFECT_GREATER_DEFENSE:
			lpObj->EffectOption.AddDefense -= lpEffect->m_value[0];
			break;
		case EFFECT_MANA_SHIELD:
			lpObj->EffectOption.AddDamageReduction -= lpEffect->m_value[0];
			break;
		case EFFECT_GREATER_CRITICAL_DAMAGE:
			lpObj->EffectOption.AddCriticalDamage -= lpEffect->m_value[0];
			break;
		case EFFECT_GREATER_LIFE:
			lpObj->EffectOption.MulMaxHP -= lpEffect->m_value[0];
			break;
		case EFFECT_ICE:
			lpObj->DelayLevel = 0;
			lpObj->DelayActionTime = 0;
			break;
		case EFFECT_FIRE_SLASH:
			lpObj->EffectOption.DivDefense -= lpEffect->m_value[0];
			break;
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);
}

bool CEffectManager::ConvertEffectByte(CEffect* lpEffect,BYTE* lpMsg) // OK
{
	if(lpMsg[0] == 0xFF)
	{
		return 0;
	}

	lpEffect->m_index = lpMsg[0];

	lpEffect->m_count = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg[1],lpMsg[2]),MAKE_NUMBERW(lpMsg[3],lpMsg[4]));

	lpEffect->m_value[0] = MAKE_NUMBERW(lpMsg[5],lpMsg[6]);
	lpEffect->m_value[1] = MAKE_NUMBERW(lpMsg[7],lpMsg[8]);
	lpEffect->m_value[2] = MAKE_NUMBERW(lpMsg[9],lpMsg[10]);
	lpEffect->m_value[3] = MAKE_NUMBERW(lpMsg[11],lpMsg[12]);
	return 1;
}

void CEffectManager::EffectByteConvert(BYTE* lpMsg,CEffect* lpEffect) // OK
{
	if(lpEffect->IsEffect() == 0 || lpEffect->m_save == 0)
	{
		memset(lpMsg,0xFF,13);
		return;
	}

	lpMsg[0] = lpEffect->m_index;

	if(lpEffect->m_type == 2)
	{
		lpMsg[1] = SET_NUMBERHB(SET_NUMBERHW(lpEffect->m_time));
		lpMsg[2] = SET_NUMBERLB(SET_NUMBERHW(lpEffect->m_time));
		lpMsg[3] = SET_NUMBERHB(SET_NUMBERLW(lpEffect->m_time));
		lpMsg[4] = SET_NUMBERLB(SET_NUMBERLW(lpEffect->m_time));
	}
	else
	{
		lpMsg[1] = SET_NUMBERHB(SET_NUMBERHW(lpEffect->m_count));
		lpMsg[2] = SET_NUMBERLB(SET_NUMBERHW(lpEffect->m_count));
		lpMsg[3] = SET_NUMBERHB(SET_NUMBERLW(lpEffect->m_count));
		lpMsg[4] = SET_NUMBERLB(SET_NUMBERLW(lpEffect->m_count));
	}

	lpMsg[5] = SET_NUMBERHB(lpEffect->m_value[0]);
	lpMsg[6] = SET_NUMBERLB(lpEffect->m_value[0]);

	lpMsg[7] = SET_NUMBERHB(lpEffect->m_value[1]);
	lpMsg[8] = SET_NUMBERLB(lpEffect->m_value[1]);

	lpMsg[9] = SET_NUMBERHB(lpEffect->m_value[2]);
	lpMsg[10] = SET_NUMBERLB(lpEffect->m_value[2]);

	lpMsg[11] = SET_NUMBERHB(lpEffect->m_value[3]);
	lpMsg[12] = SET_NUMBERLB(lpEffect->m_value[3]);
}

WORD CEffectManager::GetViewSkillState(BYTE effect) // OK
{
	switch(effect)
	{
		case EFFECT_POISON:
			return 0x1;
		case EFFECT_ICE:
			return 0x2;
		case EFFECT_GREATER_DAMAGE:
			return 0x4;
		case EFFECT_GREATER_DEFENSE:
			return 0x8;
		case EFFECT_GREATER_LIFE:
			return 0x10;
		case EFFECT_ICE_ARROW:
			return 0x20;
		case EFFECT_FIRE_SLASH:
			return 0x40;
		case EFFECT_MANA_SHIELD:
			return 0x100;
		case EFFECT_GREATER_CRITICAL_DAMAGE:
			return 0x800;
		case EFFECT_MAGIC_DAMAGE_IMMUNITY:
			return 0x8;
		case EFFECT_PHYSI_DAMAGE_IMMUNITY:
			return 0x100;
		case EFFECT_INVISIBILITY:
			return 0x4000;
	}

	return 0;
}

bool CEffectManager::CheckImmobilizeEffect(LPOBJ lpObj) // OK
{
	if(this->CheckEffect(lpObj,EFFECT_ICE_ARROW) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void CEffectManager::ClearAllEffect(LPOBJ lpObj) // OK
{
	for(int n=0;n < MAX_EFFECT_LIST;n++)
	{
		CEffect* lpEffect = &lpObj->Effect[n];

		if(lpEffect->IsEffect() == 0 || lpEffect->m_type != 0)
		{
			continue;
		}

		this->RemoveEffect(lpObj,lpEffect);
		this->GCEffectStateSend(lpObj,0,lpEffect->m_index);
		lpEffect->Clear();
	}
}

void CEffectManager::PeriodicEffect(LPOBJ lpObj,CEffect* lpEffect) // OK
{
	if(lpEffect->m_index != EFFECT_POISON)
	{
		return;
	}

	if(lpEffect->m_value[1] == 0 || (lpEffect->m_count%lpEffect->m_value[1]) != 0)
	{
		return;
	}

	if(lpObj->Type == OBJECT_NPC || lpObj->Live == 0 || OBJECT_RANGE(lpEffect->m_value[0]) == 0)
	{
		return;
	}

	LPOBJ lpTarget = &gObj[lpEffect->m_value[0]];

	int damage = 0;
	int shield = 0;
	int absorb = 0;
	int effect = 0;
	int type = 0;

	if(lpEffect->m_index == EFFECT_POISON)
	{
		damage = (((damage=(int)((lpObj->Life*lpEffect->m_value[2])/100))>lpEffect->m_value[3])?((lpEffect->m_value[3]==0)?damage:lpEffect->m_value[3]):damage);
		absorb = 0;
		effect = 2;
	}

	if(lpObj->Life < damage)
	{
		lpObj->Life = 0;
	}
	else
	{
		lpObj->Life -= damage;
	}

	if((lpTarget->Life+absorb) > (lpTarget->MaxLife+lpTarget->AddLife))
	{
		lpTarget->Life = lpTarget->MaxLife+lpTarget->AddLife;
	}
	else
	{
		lpTarget->Life += absorb;
	}

	if(damage > 0)
	{
		gObjectManager.CharacterLifeCheck(lpTarget,lpObj,(damage-shield),effect,0,type,0,0);
	}

	if(absorb > 0)
	{
		GCLifeSend(lpTarget->Index,0xFF,(int)lpTarget->Life);
	}
}

void CEffectManager::GCEffectStateSend(LPOBJ lpObj,BYTE state,BYTE effect) // OK
{
	PMSG_EFFECT_STATE_SEND pMsg;

	pMsg.header.set(0x07,sizeof(pMsg));

	pMsg.state = state;

	pMsg.effect = this->GetViewSkillState(effect);

	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);

	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);

	if(pMsg.effect == 0)
	{
		return;
	}

	if(lpObj->Type == OBJECT_USER)
	{
		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
	}

	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
}