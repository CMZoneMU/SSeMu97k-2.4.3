// SkillDamage.cpp: interface for the CSkillDamage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillDamage.h"
#include "MemScript.h"
#include "Util.h"

CSkillDamage gSkillDamage;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillDamage::CSkillDamage() // OK
{
	this->m_SkillDamageRateInfo.clear();

	this->m_SkillEffectRateInfo.clear();
}

CSkillDamage::~CSkillDamage() // OK
{

}

void CSkillDamage::Load(char* path) // OK
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

	this->m_SkillDamageRateInfo.clear();

	this->m_SkillEffectRateInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}
		
			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					SKILL_DAMAGE_INFO info;

					info.Index = lpMemScript->GetNumber();

					info.RatePvM = lpMemScript->GetAsNumber();

					info.RatePvP = lpMemScript->GetAsNumber();

					std::map<int,SKILL_DAMAGE_INFO>::iterator it = this->m_SkillDamageRateInfo.find(info.Index);

					if(it == this->m_SkillDamageRateInfo.end())
					{
						this->m_SkillDamageRateInfo.insert(std::pair<int,SKILL_DAMAGE_INFO>(info.Index,info));
					}
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					SKILL_EFFECT_INFO info;

					info.Index = lpMemScript->GetNumber();

					info.Rate = lpMemScript->GetAsNumber();

					std::map<int,SKILL_EFFECT_INFO>::iterator it = this->m_SkillEffectRateInfo.find(info.Index);

					if(it == this->m_SkillEffectRateInfo.end())
					{
						this->m_SkillEffectRateInfo.insert(std::pair<int,SKILL_EFFECT_INFO>(info.Index,info));
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CSkillDamage::GetDamageRate(CSkill* lpSkill,WORD type,int damage) // OK
{
	if(lpSkill != 0)
	{
		std::map<int,SKILL_DAMAGE_INFO>::iterator it = this->m_SkillDamageRateInfo.find(lpSkill->m_index);

		if(it != this->m_SkillDamageRateInfo.end())
		{
			damage = (damage*((type==OBJECT_MONSTER)? it->second.RatePvM:it->second.RatePvP))/100;
		}
	}
	
	return damage;
}

int CSkillDamage::GetEffectRate(CSkill* lpSkill) // OK
{
	if(lpSkill != 0)
	{
		std::map<int,SKILL_EFFECT_INFO>::iterator it = this->m_SkillEffectRateInfo.find(lpSkill->m_index);

		if(it != this->m_SkillEffectRateInfo.end())
		{
			if((GetLargeRand()%100) > it->second.Rate)
			{
				return 1;
			}
		}
	}
	
	return 0;
}