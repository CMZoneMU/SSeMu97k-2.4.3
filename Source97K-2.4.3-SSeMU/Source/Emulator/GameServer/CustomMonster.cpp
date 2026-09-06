// CustomMonster.cpp: implementation of the CCustomMonster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomMonster.h"
#include "DSProtocol.h"
#include "ItemBagManager.h"
#include "MemScript.h"
#include "Message.h"
#include "Monster.h"
#include "Notice.h"
#include "Util.h"

CCustomMonster gCustomMonster;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomMonster::CCustomMonster() // OK
{
	this->m_CustomMonsterInfo.clear();
}

CCustomMonster::~CCustomMonster() // OK
{

}

void CCustomMonster::Load(char* path) // OK
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

	this->m_CustomMonsterInfo.clear();

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

			CUSTOM_MONSTER_INFO info;

			memset(&info,0,sizeof(info));

			info.MonsterClass = lpMemScript->GetNumber();

			info.MapNumber = lpMemScript->GetAsNumber();

			info.MaxLife = lpMemScript->GetAsNumber();

			info.DamageMin = lpMemScript->GetAsNumber();

			info.DamageMax = lpMemScript->GetAsNumber();

			info.Defense = lpMemScript->GetAsNumber();

			info.AttackRate = lpMemScript->GetAsNumber();

			info.DefenseRate = lpMemScript->GetAsNumber();

			info.ExperienceRate = lpMemScript->GetAsNumber();

			info.KillMessage = lpMemScript->GetAsNumber();

			// Update 88 2.4.6 -> 97K - Suporte a SpecialBag em CustomMonster.txt
			info.BagSpecial = lpMemScript->GetAsNumber();

			this->m_CustomMonsterInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomMonster::SetCustomMonsterInfo(LPOBJ lpObj) // OK
{
	CUSTOM_MONSTER_INFO CustomMonsterInfo;

	if(this->GetCustomMonsterInfo(lpObj->Class,lpObj->Map,&CustomMonsterInfo) == 0)
	{
		return;
	}

	// Update 88 2.4.6 -> 97K - Correção do cálculo de vida, defesa, dano e ataques em MonsterList.txt
	lpObj->Life = (float)((ULONGLONG)lpObj->Life*((CustomMonsterInfo.MaxLife==-1)?100:CustomMonsterInfo.MaxLife))/100;

	lpObj->MaxLife = (float)((ULONGLONG)lpObj->MaxLife*((CustomMonsterInfo.MaxLife==-1)?100:CustomMonsterInfo.MaxLife))/100;

	lpObj->ScriptMaxLife = (float)((ULONGLONG)lpObj->ScriptMaxLife*((CustomMonsterInfo.MaxLife==-1)?100:CustomMonsterInfo.MaxLife))/100;

	lpObj->PhysiDamageMin = ((ULONGLONG)lpObj->PhysiDamageMin*((CustomMonsterInfo.DamageMin==-1)?100:CustomMonsterInfo.DamageMin))/100;

	lpObj->PhysiDamageMax = ((ULONGLONG)lpObj->PhysiDamageMax*((CustomMonsterInfo.DamageMax==-1)?100:CustomMonsterInfo.DamageMax))/100;

	lpObj->Defense = ((ULONGLONG)lpObj->Defense*((CustomMonsterInfo.Defense==-1)?100:CustomMonsterInfo.Defense))/100;

	lpObj->AttackSuccessRate = ((ULONGLONG)lpObj->AttackSuccessRate*((CustomMonsterInfo.AttackRate==-1)?100:CustomMonsterInfo.AttackRate))/100;

	lpObj->DefenseSuccessRate = ((ULONGLONG)lpObj->DefenseSuccessRate*((CustomMonsterInfo.DefenseRate==-1)?100:CustomMonsterInfo.DefenseRate))/100;

	// Update 88 2.4.6 -> 97K - Suporte a SpecialBag em CustomMonster.txt
	if(CustomMonsterInfo.BagSpecial != -1)
	{
		lpObj->CustomMonsterDrop = true;
	}
}

void CCustomMonster::MonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	CUSTOM_MONSTER_INFO CustomMonsterInfo;

	if(this->GetCustomMonsterInfo(lpObj->Class,lpObj->Map,&CustomMonsterInfo) == 0)
	{
		return;
	}

	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);

	if(OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	if(CustomMonsterInfo.KillMessage != -1)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(CustomMonsterInfo.KillMessage),lpTarget->Name);
	}

	// Update 88 2.4.6 -> 97K - Suporte a SpecialBag em CustomMonster.txt
	if(CustomMonsterInfo.BagSpecial != -1)
	{
		gItemBagManager.DropItemBySpecialValue(CustomMonsterInfo.BagSpecial,-1,-1,lpTarget,lpObj->Map,lpObj->X,lpObj->Y);
	}
}

long CCustomMonster::GetCustomMonsterExperienceRate(int index,int map) // OK
{
	CUSTOM_MONSTER_INFO CustomMonsterInfo;

	if(this->GetCustomMonsterInfo(index,map,&CustomMonsterInfo) == 0)
	{
		return 100;
	}
	else
	{
		return ((CustomMonsterInfo.ExperienceRate==-1)?100:CustomMonsterInfo.ExperienceRate);
	}
}

bool CCustomMonster::GetCustomMonsterInfo(int index,int map,CUSTOM_MONSTER_INFO* lpInfo) // OK
{
	for(std::vector<CUSTOM_MONSTER_INFO>::iterator it=this->m_CustomMonsterInfo.begin();it != this->m_CustomMonsterInfo.end();it++)
	{
		if(it->MonsterClass == index && (it->MapNumber == -1 || it->MapNumber == map))
		{
			(*lpInfo) = (*it);
			return 1;
		}
	}

	return 0;
}