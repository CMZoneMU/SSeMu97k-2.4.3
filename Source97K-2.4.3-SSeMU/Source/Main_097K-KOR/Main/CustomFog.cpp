// CustomFog.cpp: implementation of the CCustomFog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomFog.h"

CCustomFog gCustomFog;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomFog::CCustomFog() // OK
{
	this->Init();
}

CCustomFog::~CCustomFog() // OK
{

}

void CCustomFog::Init() // OK
{
	for(int n=0;n < MAX_CUSTOM_FOG;n++)
	{
		this->m_CustomFogInfo[n].Index = -1;
	}
}

void CCustomFog::Load(CUSTOM_FOG_INFO* info) // OK
{
	for(int n=0;n < MAX_CUSTOM_FOG;n++)
	{
		this->SetInfo(info[n]);
	}
}

void CCustomFog::SetInfo(CUSTOM_FOG_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_CUSTOM_FOG)
	{
		return;
	}

	this->m_CustomFogInfo[info.Index] = info;
}

CUSTOM_FOG_INFO* CCustomFog::GetInfo(int index) // OK
{
	if(index < 0 || index >= MAX_CUSTOM_FOG)
	{
		return 0;
	}

	if(this->m_CustomFogInfo[index].Index != index)
	{
		return 0;
	}

	return &this->m_CustomFogInfo[index];
}

CUSTOM_FOG_INFO* CCustomFog::GetInfoByNumber(int MapNumber) // OK
{
	for(int n=0;n < MAX_CUSTOM_FOG;n++)
	{
		CUSTOM_FOG_INFO* lpInfo = this->GetInfo(n);

		if(lpInfo == 0)
		{
			continue;
		}

		if(lpInfo->MapIndex == MapNumber)
		{
			return lpInfo;
		}
	}

	return 0;
}