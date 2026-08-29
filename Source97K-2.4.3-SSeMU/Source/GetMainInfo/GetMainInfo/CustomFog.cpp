// CustomFog.cpp: implementation of the CCustomFog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomFog.h"
#include "MemScript.h"

CCustomFog gCustomFog;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool CCustomFog::Load(char* path) // OK
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

	for(int n=0;n < MAX_CUSTOM_FOG;n++)
	{
		this->m_CustomFogInfo[n].Index = -1;
	}

	static int CustomFogCount = 0;

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

			CUSTOM_FOG_INFO info;

			memset(&info,0,sizeof(info));

			info.Index = CustomFogCount++;

			info.MapIndex = lpMemScript->GetNumber();

			info.FogStart = lpMemScript->GetAsNumber();

			info.FogEnd = lpMemScript->GetAsNumber();

			info.ColorR = lpMemScript->GetAsNumber();

			info.ColorG = lpMemScript->GetAsNumber();

			info.ColorB = lpMemScript->GetAsNumber();

			this->SetInfo(info);
		}
	}
	catch(...)
	{
		printf(lpMemScript->GetLastError());
		return 0;
	}

	delete lpMemScript;

	printf("[Custom Fog] Loaded: %d\n",CustomFogCount);

	return 1;
}

void CCustomFog::SetInfo(CUSTOM_FOG_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_CUSTOM_FOG)
	{
		return;
	}

	this->m_CustomFogInfo[info.Index] = info;
}