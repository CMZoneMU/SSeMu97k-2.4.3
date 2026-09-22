// CustomMap.h: interface for the CCustomMap class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_CUSTOM_MAP 80

// Update SSeMU 92 2.4.9 -> 97K SSeMU Update 97 (2.5.5) - Add MusicPath to CUSTOM_MAP_INFO
struct CUSTOM_MAP_INFO
{
	int Index;
	int MapIndex;
	char MapName[32];
	char MusicPath[100];
};

class CCustomMap
{
public:
	bool Load(char* path);
	void SetInfo(CUSTOM_MAP_INFO info);
	CUSTOM_MAP_INFO m_CustomMapInfo[MAX_CUSTOM_MAP];
};

extern CCustomMap gCustomMap;