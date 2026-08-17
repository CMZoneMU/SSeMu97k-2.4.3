// CustomEffect.cpp: implementation of the CCustomEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomEffect.h"
#include "Util.h"

CCustomEffect gCustomEffect;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomEffect::CCustomEffect() // OK
{
	this->Init();
}

CCustomEffect::~CCustomEffect() // OK
{

}

void CCustomEffect::Init() // OK
{
	for(int n=0;n < MAX_CUSTOM_EFFECT;n++)
	{
		this->m_CustomEffectInfo[n].Index = -1;
	}
}

void CCustomEffect::Load(CUSTOM_EFFECT_INFO* info) // OK
{
	for(int n=0;n < MAX_CUSTOM_EFFECT;n++)
	{
		this->SetInfo(info[n]);
	}
}

void CCustomEffect::SetInfo(CUSTOM_EFFECT_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_CUSTOM_EFFECT)
	{
		return;
	}

	this->m_CustomEffectInfo[info.Index] = info;
}