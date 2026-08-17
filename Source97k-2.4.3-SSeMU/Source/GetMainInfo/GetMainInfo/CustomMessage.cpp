// CustomJewel.cpp: implementation of the CCustomJewel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomMessage.h"
#include "MemScript.h"

CCustomMessage gCustomMessage;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomMessage::CCustomMessage() // OK
{
	this->Init();
}

CCustomMessage::~CCustomMessage() // OK
{

}

void CCustomMessage::Init() // OK
{
	for(int n=0;n < MAX_CUSTOM_MESSAGE;n++)
	{
		this->m_EngCustomMessageInfo[n].Index = -1;
		this->m_PorCustomMessageInfo[n].Index = -1;
		this->m_SpnCustomMessageInfo[n].Index = -1;
	}
}

bool CCustomMessage::Load(char* path) // OK
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

	this->Init();

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

					CUSTOM_MESSAGE_INFO info;

					memset(&info,0,sizeof(info));

					info.Index = lpMemScript->GetNumber();

					strcpy_s(info.Message,lpMemScript->GetAsString());

					this->SetInfoEng(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					CUSTOM_MESSAGE_INFO info;

					memset(&info,0,sizeof(info));

					info.Index = lpMemScript->GetNumber();

					strcpy_s(info.Message,lpMemScript->GetAsString());

					this->SetInfoPor(info);
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					CUSTOM_MESSAGE_INFO info;

					memset(&info,0,sizeof(info));

					info.Index = lpMemScript->GetNumber();

					strcpy_s(info.Message,lpMemScript->GetAsString());

					this->SetInfoSpn(info);
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

	return 1;
}

void CCustomMessage::SetInfoEng(CUSTOM_MESSAGE_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_CUSTOM_MESSAGE)
	{
		return;
	}

	this->m_EngCustomMessageInfo[info.Index] = info;
}

void CCustomMessage::SetInfoPor(CUSTOM_MESSAGE_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_CUSTOM_MESSAGE)
	{
		return;
	}

	this->m_PorCustomMessageInfo[info.Index] = info;
}

void CCustomMessage::SetInfoSpn(CUSTOM_MESSAGE_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_CUSTOM_MESSAGE)
	{
		return;
	}

	this->m_SpnCustomMessageInfo[info.Index] = info;
}