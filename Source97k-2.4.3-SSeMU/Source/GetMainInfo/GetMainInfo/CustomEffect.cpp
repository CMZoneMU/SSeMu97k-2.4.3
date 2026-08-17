// CustomEffect.cpp: implementation of the CCustomEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomEffect.h"
#include "MemScript.h"

CCustomEffect gCustomEffect;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool CCustomEffect::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		printf(MEM_SCRIPT_ALLOC_ERROR,path);
		return 0;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		printf(lpMemScript->GetLastError());
		delete lpMemScript;
		return 0;
	}

	for(int n=0;n < MAX_CUSTOM_EFFECT;n++)
	{
		this->m_CustomEffectInfo[n].Index = -1;
	}

	static int CustomEffectIndexCount = 0;

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

			CUSTOM_EFFECT_INFO info;

			memset(&info,0,sizeof(info));

			info.Index = CustomEffectIndexCount++;

			info.ItemIndex = GET_ITEM(lpMemScript->GetNumber(),lpMemScript->GetAsNumber());

			info.EffectType = lpMemScript->GetAsNumber();

			info.EffectIndex = lpMemScript->GetAsNumber();

			info.EffectValue = lpMemScript->GetAsNumber();

			info.EffectLevel = lpMemScript->GetAsNumber();

			info.ColorR = lpMemScript->GetAsNumber();

			info.ColorG = lpMemScript->GetAsNumber();

			info.ColorB = lpMemScript->GetAsNumber();

			info.Scale = lpMemScript->GetAsFloatNumber();

			this->SetInfo(info);
		}
	}
	catch(...)
	{
		printf(lpMemScript->GetLastError());
		return 0;
	}

	delete lpMemScript;

	printf("[Custom Effect] Loaded: %d\n",CustomEffectIndexCount);

	return 1;
}

void CCustomEffect::SetInfo(CUSTOM_EFFECT_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_CUSTOM_EFFECT)
	{
		return;
	}

	this->m_CustomEffectInfo[info.Index] = info;
}