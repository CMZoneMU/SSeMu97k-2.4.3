// SkillAttackSpeed.h: interface for the CSkillAttackSpeed class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_ATTACK_SPEED 70

struct SKILL_ATTACK_SPEED_INFO
{
	int SkillIndex;
	int MaxPhysiSpeed;
	int MaxMagicSpeed;
	int AnimationValue;
};

class CSkillAttackSpeed
{
public:
	CSkillAttackSpeed();
	virtual ~CSkillAttackSpeed();
	void Init();
	void Load(SKILL_ATTACK_SPEED_INFO* info);
	void SetInfo(SKILL_ATTACK_SPEED_INFO info);
	SKILL_ATTACK_SPEED_INFO* GetInfo(int index);
public:
	SKILL_ATTACK_SPEED_INFO m_SkillAttackSpeedInfo[MAX_ATTACK_SPEED];
};

extern CSkillAttackSpeed gSkillAttackSpeed;