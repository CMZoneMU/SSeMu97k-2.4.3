// Update 89 2.4.7 -> 97K - Filtro de palavras e nomes reconstruído
// Filter.cpp: implementation of the CFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Filter.h"
#include "MemScript.h"
#include "Util.h"
#include <algorithm>

CFilter gFilter;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilter::CFilter() // OK
{
	this->m_FilterInfo.clear();
}

CFilter::~CFilter() // OK
{

}

void CFilter::Load(char* path) // OK
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

	this->m_FilterInfo.clear();

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

			std::string Syntax(lpMemScript->GetString());

			std::transform(Syntax.begin(),Syntax.end(),Syntax.begin(),tolower);

			this->m_FilterInfo.insert(Syntax);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;

	LogAdd(LOG_BLUE,"[Filter] Total entries: %d",this->m_FilterInfo.size());
}

void CFilter::CheckSyntax(char* text) // OK
{
	std::string originalText(text);

	std::string lowerText = originalText;

	std::transform(lowerText.begin(),lowerText.end(),lowerText.begin(),tolower);

	for(std::set<std::string>::iterator it = this->m_FilterInfo.begin(); it != this->m_FilterInfo.end(); ++it)
	{
		const std::string& lowerLabel = *it;

		if(lowerLabel.empty())
		{
			continue;
		}

		size_t pos = lowerText.find(lowerLabel);

		while(pos != std::string::npos)
		{
			std::fill(originalText.begin()+pos,originalText.begin()+pos+lowerLabel.size(),'*');

			pos = lowerText.find(lowerLabel,pos+lowerLabel.size());
		}
	}

	strcpy_s(text,strlen(text)+1,originalText.c_str());
}
