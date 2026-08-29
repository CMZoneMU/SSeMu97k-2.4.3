// SkillAttackSpeed.cpp: implementation of the CSkillAttackSpeed class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillAttackSpeed.h"

CSkillAttackSpeed gSkillAttackSpeed;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillAttackSpeed::CSkillAttackSpeed() // OK
{
	this->Init();
}

CSkillAttackSpeed::~CSkillAttackSpeed() // OK
{

}

void CSkillAttackSpeed::Init() // OK
{
	for(int n=0;n < MAX_ATTACK_SPEED;n++)
	{
		this->m_SkillAttackSpeedInfo[n].SkillIndex = -1;
	}
}

void CSkillAttackSpeed::Load(SKILL_ATTACK_SPEED_INFO* info) // OK
{
	for(int n=0;n < MAX_ATTACK_SPEED;n++)
	{
		this->SetInfo(info[n]);
	}
}

void CSkillAttackSpeed::SetInfo(SKILL_ATTACK_SPEED_INFO info) // OK
{
	if(info.SkillIndex < 0 || info.SkillIndex >= MAX_ATTACK_SPEED)
	{
		return;
	}

	this->m_SkillAttackSpeedInfo[info.SkillIndex] = info;
}

SKILL_ATTACK_SPEED_INFO* CSkillAttackSpeed::GetInfo(int index) // OK
{
	if(index < 0 || index >= MAX_ATTACK_SPEED)
	{
		return 0;
	}

	if(this->m_SkillAttackSpeedInfo[index].SkillIndex != index)
	{
		return 0;
	}

	return &this->m_SkillAttackSpeedInfo[index];
}