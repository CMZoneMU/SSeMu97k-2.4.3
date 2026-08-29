// CustomFog.h: interface for the CCustomFog class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_CUSTOM_FOG 80

struct CUSTOM_FOG_INFO
{
	int Index;
	int MapIndex;
	int FogStart;
	int FogEnd;
	BYTE ColorR;
	BYTE ColorG;
	BYTE ColorB;
};

class CCustomFog
{
public:
	CCustomFog();
	virtual ~CCustomFog();
	void Init();
	void Load(CUSTOM_FOG_INFO* info);
	void SetInfo(CUSTOM_FOG_INFO info);
	CUSTOM_FOG_INFO* GetInfo(int index);
	CUSTOM_FOG_INFO* GetInfoByNumber(int MapNumber);
public:
	CUSTOM_FOG_INFO m_CustomFogInfo[MAX_CUSTOM_FOG];
};

extern CCustomFog gCustomFog;