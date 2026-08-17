// Skill.cpp: implementation of the CSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Skill.h"
#include "SkillManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkill::CSkill() // OK
{
	this->Clear();
}

CSkill::~CSkill() // OK
{

}

void CSkill::Clear() // OK
{
	this->m_index = 0xFFFF;
	this->m_DamageMin = 0;
	this->m_DamageMax = 0;
}

bool CSkill::IsSkill() // OK
{
	if(this->m_index == 0xFFFF)
	{
		return 0;
	}

	return 1;
}

bool CSkill::Set(int index) // OK
{
	if(this->IsSkill() != 0 && index < 300)
	{
		return 0;
	}

	this->m_index = index;

	this->m_DamageMin = gSkillManager.GetSkillDamage(index);

	this->m_DamageMax = this->m_DamageMin+(this->m_DamageMin/2);

	return 1;
}
