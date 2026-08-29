// CustomTooltip.cpp: implementation of the CCustomTooltip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomTooltip.h"

CCustomTooltip gCustomTooltip;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomTooltip::CCustomTooltip() // OK
{
	this->Init();
}

CCustomTooltip::~CCustomTooltip() // OK
{

}

void CCustomTooltip::Init() // OK
{
	for(int n=0;n<MAX_CUSTOM_TOOLTIP;n++)
	{
		this->m_CustomTooltipInfo[n].Index=-1;
	}
}

void CCustomTooltip::Load(CUSTOM_TOOLTIP_INFO* info) // OK
{
	for(int n=0;n<MAX_CUSTOM_TOOLTIP;n++)
	{
		this->SetInfo(info[n]);
	}
}

void CCustomTooltip::SetInfo(CUSTOM_TOOLTIP_INFO info) // OK
{
	if(info.Index<0 || info.Index >= MAX_CUSTOM_TOOLTIP)
	{
		return;
	}

	this->m_CustomTooltipInfo[info.Index]=info;
}