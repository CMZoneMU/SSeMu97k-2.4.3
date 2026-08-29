// CustomEffect.h: interface for the CCustomEffect class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_CUSTOM_EFFECT 5000

struct CUSTOM_EFFECT_INFO
{
	int Index;
	int ItemIndex;
	int EffectType;
	int EffectIndex;
	int EffectValue;
	int EffectLevel;
	BYTE ColorR;
	BYTE ColorG;
	BYTE ColorB;
	float Scale;
};

class CCustomEffect
{
public:
	CCustomEffect();
	virtual ~CCustomEffect();
	void Init();
	void Load(CUSTOM_EFFECT_INFO* info);
	void SetInfo(CUSTOM_EFFECT_INFO info);
public:
	CUSTOM_EFFECT_INFO m_CustomEffectInfo[MAX_CUSTOM_EFFECT];
};

extern CCustomEffect gCustomEffect;
