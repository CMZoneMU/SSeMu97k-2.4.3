// PetDamageTable.cpp: interface for the CPetDamageTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PetDamageTable.h"
#include "MemScript.h"
#include "Util.h"

CPetDamageTable gPetDamageTable;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPetDamageTable::CPetDamageTable() // OK
{
	this->m_PetDamageTableInfo.clear();
}

CPetDamageTable::~CPetDamageTable() // OK
{

}

void CPetDamageTable::Load(char* path) // OK
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

	this->m_PetDamageTableInfo.clear();

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

			PET_DAMAGE_TABLE_INFO info;

			info.Index = lpMemScript->GetNumber();

			info.MinLevel = lpMemScript->GetAsNumber();

			info.MaxLevel = lpMemScript->GetAsNumber();

			info.MinReset = lpMemScript->GetAsNumber();

			info.MaxReset = lpMemScript->GetAsNumber();

			info.DamageRate = lpMemScript->GetAsNumber();

			this->m_PetDamageTableInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CPetDamageTable::GetPetDamage(LPOBJ lpObj,int index,int* damage) // OK
{
	for(std::vector<PET_DAMAGE_TABLE_INFO>::iterator it= this->m_PetDamageTableInfo.begin();it != this->m_PetDamageTableInfo.end();it++)
	{
		if(it->Index != index)
		{
			continue;
		}

		if(it->MinLevel != -1 && it->MinLevel > lpObj->Level)
		{
			continue;
		}

		if(it->MaxLevel != -1 && it->MaxLevel < lpObj->Level)
		{
			continue;
		}

		if(it->MinReset != -1 && it->MinReset > lpObj->Reset)
		{
			continue;
		}

		if(it->MaxReset != -1 && it->MaxReset < lpObj->Reset)
		{
			continue;
		}

		(*damage) = ((*damage)*it->DamageRate)/1000;

		return 1;
	}

	return 0;
}