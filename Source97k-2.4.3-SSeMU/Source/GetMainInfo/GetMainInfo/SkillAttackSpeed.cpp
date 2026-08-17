// SkillAttackSpeed.cpp: implementation of the CSkillAttackSpeed class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillAttackSpeed.h"
#include "MemScript.h"

CSkillAttackSpeed gSkillAttackSpeed;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool CSkillAttackSpeed::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		printf(MEM_SCRIPT_ALLOC_ERROR, path);
		return 0;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		printf(lpMemScript->GetLastError());
		delete lpMemScript;
		return 0;
	}

	for(int n=0;n< MAX_ATTACK_SPEED;n++)
	{
		this->m_SkillAttackSpeedInfo[n].SkillIndex = -1;
	}

	static int AttackSpeedCount = 0;

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

			SKILL_ATTACK_SPEED_INFO info;

			memset(&info,0,sizeof(info));

			info.SkillIndex = lpMemScript->GetNumber();

			info.MaxPhysiSpeed = lpMemScript->GetAsNumber();
			
			info.MaxMagicSpeed = lpMemScript->GetAsNumber();
			
			info.AnimationValue = lpMemScript->GetAsNumber();

			this->SetInfo(info);

			AttackSpeedCount++;
		}
	}
	catch(...)
	{
		printf(lpMemScript->GetLastError());
		return 0;
	}

	delete lpMemScript;

	printf("[Skill Attack Speed] Loaded: %d\n",AttackSpeedCount);

	return 1;
}

void CSkillAttackSpeed::SetInfo(SKILL_ATTACK_SPEED_INFO info) // OK
{
	if(info.SkillIndex < 0 || info.SkillIndex >= MAX_ATTACK_SPEED)
	{
		return;
	}

	this->m_SkillAttackSpeedInfo[info.SkillIndex] = info;
}
