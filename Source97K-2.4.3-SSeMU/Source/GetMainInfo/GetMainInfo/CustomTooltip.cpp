// CustomTooltip.cpp: implementation of the CCustomTooltip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomTooltip.h"
#include "MemScript.h"

CCustomTooltip gCustomTooltip;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool CCustomTooltip::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		printf(MEM_SCRIPT_ALLOC_ERROR,path);
		return 0;
	}

	if (lpMemScript->SetBuffer(path) == 0)
	{
		printf(lpMemScript->GetLastError());
		delete lpMemScript;
		return 0;
	}

	for(int n=0;n < MAX_CUSTOM_TOOLTIP;n++)
	{
		this->m_CustomTooltipInfo[n].Index = -1;
	}

	static int CustomToolTipCount = 0;

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

			CUSTOM_TOOLTIP_INFO info;

			memset(&info,0,sizeof(info));

			info.Index = CustomToolTipCount++;

			info.ItemIndex = GET_ITEM(lpMemScript->GetNumber(),lpMemScript->GetAsNumber());

			info.Level = lpMemScript->GetAsNumber();

			info.Position = lpMemScript->GetAsNumber();

			info.FontColor = lpMemScript->GetAsNumber();

			info.FontBold = lpMemScript->GetAsNumber();

			strcpy_s(info.Text,lpMemScript->GetAsString());

			this->SetInfo(info);

			}
	}
	catch(...)
	{
		printf(lpMemScript->GetLastError());
		return 0;
	}

	delete lpMemScript;

	printf("[Custom ToolTip] Loaded: %d\n",CustomToolTipCount);

	return 1;
}

void CCustomTooltip::SetInfo(CUSTOM_TOOLTIP_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_CUSTOM_TOOLTIP)
	{
		return;
	}

	this->m_CustomTooltipInfo[info.Index] = info;
}