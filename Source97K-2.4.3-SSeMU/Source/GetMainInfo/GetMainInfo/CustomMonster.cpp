// CustomMonster.cpp: implementation of the CCustomMonster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomMonster.h"
#include "MemScript.h"

CCustomMonster gCustomMonster;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool CCustomMonster::Load(char* path) // OK
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

	for(int n=0;n < MAX_CUSTOM_MONSTER;n++)
	{
		this->m_CustomMonsterInfo[n].Index = -1;
	}

	static int MonsterCount = 0;

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

					CUSTOM_MONSTER_INFO info;

					memset(&info,0,sizeof(info));

					info.Index = lpMemScript->GetNumber();

					info.MonsterIndex = lpMemScript->GetAsNumber();

					info.NpcClass = lpMemScript->GetAsNumber();

					info.PKLevel = lpMemScript->GetAsNumber();

					info.Type = lpMemScript->GetAsNumber();

					info.Effect = lpMemScript->GetAsNumber();

					info.Scale = lpMemScript->GetAsFloatNumber();

					strcpy_s(info.FolderPath,lpMemScript->GetAsString());

					strcpy_s(info.ModelName,lpMemScript->GetAsString());

					strcpy_s(info.Name,lpMemScript->GetAsString());

					MonsterCount++;

					this->SetInfo(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					CUSTOM_MONSTER_SKIN_INFO info;

					memset(&info,0,sizeof(info));

					info.Use = true;

					info.Index = lpMemScript->GetNumber();

					info.Slot = lpMemScript->GetAsNumber();

					info.ItemIndex = GET_ITEM(lpMemScript->GetAsNumber(),lpMemScript->GetAsNumber());

					info.ItemLevel = lpMemScript->GetAsNumber();

					info.SetOption = lpMemScript->GetAsNumber();

					info.NewOption = lpMemScript->GetAsNumber();

					this->SetSkinInfo(info);
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
		printf(lpMemScript->GetLastError());
		return 0;
	}

	delete lpMemScript;

	printf("[Custom Monster] Loaded: %d\n",MonsterCount);

	return 1;
}

void CCustomMonster::SetInfo(CUSTOM_MONSTER_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_CUSTOM_MONSTER)
	{
		return;
	}

	this->m_CustomMonsterInfo[info.Index] = info;
}

void CCustomMonster::SetSkinInfo(CUSTOM_MONSTER_SKIN_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_CUSTOM_MONSTER)
	{
		return;
	}

	if(info.Slot < 0 || info.Slot >= MAX_SKIN_SLOT)
	{
		return;
	}

	this->m_CustomMonsterInfo[info.Index].SkinInfo[info.Slot] = info;
}