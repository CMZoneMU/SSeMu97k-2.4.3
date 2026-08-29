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
	bool Load(char* path);
	void SetInfo(SKILL_ATTACK_SPEED_INFO info);
	SKILL_ATTACK_SPEED_INFO m_SkillAttackSpeedInfo[MAX_ATTACK_SPEED];
};

extern CSkillAttackSpeed gSkillAttackSpeed;