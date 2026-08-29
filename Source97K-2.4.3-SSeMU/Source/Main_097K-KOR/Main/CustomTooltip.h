// CustomTooltip.h: interface for the CCustomTooltip class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_CUSTOM_TOOLTIP 2000

struct CUSTOM_TOOLTIP_INFO
{
	int Index;
	int ItemIndex;
	int ItemLevel;
	int Position;
	int FontColor;
	int FontBold;
	char Text[64];
};

class CCustomTooltip
{
public:
	CCustomTooltip();
	virtual ~CCustomTooltip();
	void Init();
	void Load(CUSTOM_TOOLTIP_INFO* info);
	void SetInfo(CUSTOM_TOOLTIP_INFO info);
public:
	CUSTOM_TOOLTIP_INFO m_CustomTooltipInfo[MAX_CUSTOM_TOOLTIP];
};

extern CCustomTooltip gCustomTooltip;