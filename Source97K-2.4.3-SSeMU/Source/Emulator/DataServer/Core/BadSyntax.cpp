// BadSyntax.cpp: implementation of the CBadSyntax class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BadSyntax.h"
#include "MemScript.h"
#include "Util.h"

CBadSyntax gBadSyntax;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBadSyntax::CBadSyntax()
{
	this->m_BadSyntaxInfo.clear();
}

CBadSyntax::~CBadSyntax()
{

}

void CBadSyntax::Load(char* path)
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

	this->m_BadSyntaxInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			char* token = lpMemScript->GetString();

			if(strcmp("end",token) == 0)
			{
				break;
			}

			std::string value = token;

			std::transform(value.begin(),value.end(),value.begin(),::tolower);

			if(value.empty() == 0)
			{
				this->m_BadSyntaxInfo.push_back(value);
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CBadSyntax::CheckSyntax(char* text)
{
	if(text == 0)
	{
		return 0;
	}

	std::string lowerText(text);

	std::transform(lowerText.begin(),lowerText.end(),lowerText.begin(),::tolower);

	for(std::vector<std::string>::iterator it = this->m_BadSyntaxInfo.begin();it != this->m_BadSyntaxInfo.end();it++)
	{
		if(lowerText.find(*it) != std::string::npos)
		{
			return 1;
		}
	}

	return 0;
}
