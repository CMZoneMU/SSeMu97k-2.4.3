// CustomWingEffect.h: interface for the CCustomWingEffect class.
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
	bool Load(char* path);
	void SetInfo(CUSTOM_EFFECT_INFO info);
	CUSTOM_EFFECT_INFO m_CustomEffectInfo[MAX_CUSTOM_EFFECT];
};

extern CCustomEffect gCustomEffect;