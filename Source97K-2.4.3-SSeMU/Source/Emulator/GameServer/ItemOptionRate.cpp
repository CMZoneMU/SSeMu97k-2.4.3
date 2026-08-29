// ItemOptionManager.cpp: implementation of the CItemOptionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemOptionRate.h"
#include "ExcellentOptionRate.h"
#include "ItemManager.h"
#include "MemScript.h"
#include "RandomManager.h"
#include "Util.h"

CItemOptionRate gItemOptionRate;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemOptionRate::CItemOptionRate() // OK
{
	this->m_ItemOption0RateInfo.clear();

	this->m_ItemOption1RateInfo.clear();

	this->m_ItemOption2RateInfo.clear();

	this->m_ItemOption3RateInfo.clear();

	this->m_ItemOption4RateInfo.clear();

}

CItemOptionRate::~CItemOptionRate() // OK
{

}

void CItemOptionRate::Load(char* path) // OK
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

	this->m_ItemOption0RateInfo.clear();

	this->m_ItemOption1RateInfo.clear();

	this->m_ItemOption2RateInfo.clear();

	this->m_ItemOption3RateInfo.clear();

	this->m_ItemOption4RateInfo.clear();

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

					ITEM_OPTION0_RATE_INFO info;

					info.Index = lpMemScript->GetNumber();

					for(int n=0;n < MAX_ITEM_OPTION0_RATE;n++)
					{
						info.Rate[n] = lpMemScript->GetAsNumber();
					}

					this->m_ItemOption0RateInfo.insert(std::pair<int,ITEM_OPTION0_RATE_INFO>(info.Index,info));
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_OPTION1_RATE_INFO info;

					info.Index = lpMemScript->GetNumber();

					for(int n=0;n < MAX_ITEM_OPTION1_RATE;n++)
					{
						info.Rate[n] = lpMemScript->GetAsNumber();
					}

					this->m_ItemOption1RateInfo.insert(std::pair<int,ITEM_OPTION1_RATE_INFO>(info.Index,info));
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_OPTION2_RATE_INFO info;

					info.Index = lpMemScript->GetNumber();

					for(int n=0;n < MAX_ITEM_OPTION2_RATE;n++)
					{
						info.Rate[n] = lpMemScript->GetAsNumber();
					}

					this->m_ItemOption2RateInfo.insert(std::pair<int,ITEM_OPTION2_RATE_INFO>(info.Index,info));
				}
				else if(section == 3)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_OPTION3_RATE_INFO info;

					info.Index = lpMemScript->GetNumber();

					for(int n=0;n < MAX_ITEM_OPTION3_RATE;n++)
					{
						info.Rate[n] = lpMemScript->GetAsNumber();
					}

					this->m_ItemOption3RateInfo.insert(std::pair<int,ITEM_OPTION3_RATE_INFO>(info.Index,info));
				}
				else if(section == 4)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_OPTION4_RATE_INFO info;

					info.Index = lpMemScript->GetNumber();

					for(int n=0;n < MAX_ITEM_OPTION4_RATE;n++)
					{
						info.Rate[n] = lpMemScript->GetAsNumber();
					}

					this->m_ItemOption4RateInfo.insert(std::pair<int,ITEM_OPTION4_RATE_INFO>(info.Index,info));
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
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CItemOptionRate::GetItemOption0(int index,BYTE* option) // OK
{
	std::map<int,ITEM_OPTION0_RATE_INFO>::iterator it = this->m_ItemOption0RateInfo.find(index);

	if(it == this->m_ItemOption0RateInfo.end())
	{
		return 0;
	}

	CRandomManager RandomManager;

	for(int n=0;n < MAX_ITEM_OPTION0_RATE;n++)
	{
		RandomManager.AddElement(n,it->second.Rate[n]);
	}

	return RandomManager.GetRandomElement(option);
}

bool CItemOptionRate::GetItemOption1(int index,BYTE* option) // OK
{
	std::map<int,ITEM_OPTION1_RATE_INFO>::iterator it = this->m_ItemOption1RateInfo.find(index);

	if(it == this->m_ItemOption1RateInfo.end())
	{
		return 0;
	}

	CRandomManager RandomManager;

	for(int n=0;n < MAX_ITEM_OPTION1_RATE;n++)
	{
		RandomManager.AddElement(n,it->second.Rate[n]);
	}

	return RandomManager.GetRandomElement(option);
}

bool CItemOptionRate::GetItemOption2(int index,BYTE* option) // OK
{
	std::map<int,ITEM_OPTION2_RATE_INFO>::iterator it = this->m_ItemOption2RateInfo.find(index);

	if(it == this->m_ItemOption2RateInfo.end())
	{
		return 0;
	}

	CRandomManager RandomManager;

	for(int n=0;n < MAX_ITEM_OPTION2_RATE;n++)
	{
		RandomManager.AddElement(n,it->second.Rate[n]);
	}

	return RandomManager.GetRandomElement(option);
}

bool CItemOptionRate::GetItemOption3(int index,BYTE* option) // OK
{
	std::map<int,ITEM_OPTION3_RATE_INFO>::iterator it = this->m_ItemOption3RateInfo.find(index);

	if(it == this->m_ItemOption3RateInfo.end())
	{
		return 0;
	}

	CRandomManager RandomManager;

	for(int n=0;n < MAX_ITEM_OPTION3_RATE;n++)
	{
		RandomManager.AddElement(n,it->second.Rate[n]);
	}

	return RandomManager.GetRandomElement(option);
}

bool CItemOptionRate::GetItemOption4(int index,BYTE* option) // OK
{
	std::map<int,ITEM_OPTION4_RATE_INFO>::iterator it = this->m_ItemOption4RateInfo.find(index);

	if(it == this->m_ItemOption4RateInfo.end())
	{
		return 0;
	}

	CRandomManager RandomManager;

	for(int n=0;n < MAX_ITEM_OPTION4_RATE;n++)
	{
		RandomManager.AddElement(n,it->second.Rate[n]);
	}

	return RandomManager.GetRandomElement(option);
}

bool CItemOptionRate::GetItemOption5(int index,BYTE* option) // OK
{
	return 0;
}

void CItemOptionRate::MakeLevelOption(int MinLevel,int MaxLevel,short* level) // OK
{
	(*level) = 0;

	if(MinLevel >= MaxLevel)
	{
		(*level) = MinLevel;
		return;
	}

	(*level) = (short)(MinLevel+(GetLargeRand()%((MaxLevel-MinLevel)+1)));
}

void CItemOptionRate::MakeNewOption(int ItemIndex,int random,int value,BYTE* option) // OK
{
	(*option) = 0;

	int MaxNewOption = MAX_EXC_OPTION;

	if((ItemIndex >= GET_ITEM(12,3) && ItemIndex <= GET_ITEM(12,6)) || ItemIndex == GET_ITEM(13,30)) // 2sd Wings
	{
		MaxNewOption = ((ItemIndex==GET_ITEM(13,30))?4:3);
	}
	else if(ItemIndex == GET_ITEM(13,3)) // Dinorant
	{
		MaxNewOption = 3;
	}

	if(random == 0)
	{
		if(((*option) & value) == 0)
		{
			(*option) |= value;
		}
	}
	else
	{
		if(random == 2)
		{
			value = GetLargeRand()%value;
		}

		int count = 0;

		while(true)
		{
			if(count >= value)
			{
				break;
			}

			if(count >= MaxNewOption)
			{
				break;
			}

			int value = gExcellentOptionRate.GetNewOption(ItemIndex);

			if(((*option) & value) == 0)
			{
				(*option) |= value;
				count++;
			}
		}
	}
}

bool CItemOptionRate::MakeSetOption(int ItemIndex,int value,BYTE* option) // OK
{
	(*option) = 0;
	return 0;
}