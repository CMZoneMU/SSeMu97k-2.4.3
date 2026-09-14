// Update 93 2.5.1 -> 97K - Barra de vida de monstros customizada
// CustomHealthBar.cpp: implementation of the CCustomHealthBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomHealthBar.h"
#include "MemScript.h"
#include "Util.h"

CCustomHealthBar gCustomHealthBar;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomHealthBar::CCustomHealthBar()
{
	this->m_MonsterHealthBarInfo.clear();
}

CCustomHealthBar::~CCustomHealthBar()
{

}

void CCustomHealthBar::Load(char* path)
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

	this->m_MonsterHealthBarInfo.clear();

	try
	{
		while (true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			MONSTER_HEALTH_BAR_INFO info;
			memset(&info,0,sizeof(info));

			info.MonsterIndex = lpMemScript->GetNumber();

			info.MapNumber = lpMemScript->GetAsNumber();

			info.BarSwitch = lpMemScript->GetAsNumber();

			this->m_MonsterHealthBarInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CCustomHealthBar::CheckHealthBar(int MonsterIndex,int MapNumber)
{
	for(std::vector<MONSTER_HEALTH_BAR_INFO>::iterator it = this->m_MonsterHealthBarInfo.begin(); it != this->m_MonsterHealthBarInfo.end(); it++)
	{
		if(it->MonsterIndex != -1 && it->MonsterIndex != MonsterIndex)
		{
			continue;
		}

		if(it->MapNumber != -1 && it->MapNumber != MapNumber)
		{
			continue;
		}

		return it->BarSwitch;
	}

	return 1;
}
