// Skill.h: interface for the CSkill class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CSkill
{
public:
	CSkill();
	virtual ~CSkill();
	void Clear();
	bool IsSkill();
	bool Set(int index);
public:
	WORD m_index;
	int m_DamageMin;
	int m_DamageMax;
};
