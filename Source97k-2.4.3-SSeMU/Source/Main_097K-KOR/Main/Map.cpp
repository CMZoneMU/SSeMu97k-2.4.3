#include "stdafx.h"
#include "Map.h"
#include "CustomMap.h"
#include "Offset.h"
#include "Util.h"

void InitMap()  // OK
{
	SetCompleteHook(0xE8,0x00428C35,&LoadMapName);

	SetCompleteHook(0xE9,0x004EF48D,&LoadMapNameParty);

	SetCompleteHook(0xE9,0x00526D0C,&LoadMapMusic);
}

char* LoadMapName(int index) //OK
{
	CUSTOM_MAP_INFO* lpInfo = gCustomMap.GetInfoByNumber(index);

	if(lpInfo != 0)
	{
		return lpInfo->MapName;
	}

	return ((char*(*)(int))0x004EF120)(index);
}

__declspec(naked) void LoadMapNameParty() // OK
{
	static DWORD LoadMapNamePartyAddress1 = 0x004EF498;
	static DWORD index;
	static char* MapName;

	_asm 
	{
		PushAd
		Movsx Ecx,Byte Ptr Ds:[Esi+0x0C]
		Mov index,Ecx
		PopAd
	}
		
	MapName = gCustomMap.GetMapName(index);

	_asm
	{
		Push MapName
		Lea Ecx,[Esp+0x40]
		Push 0x0055A710
		Push Ecx
		Jmp[LoadMapNamePartyAddress1]
	}
}

__declspec(naked) void LoadMapMusic() // OK
{
	static DWORD jmpBack = 0x00527475;

	switch (World)
	{
	case 0:
		((void(_cdecl*)(char* Name, BOOL bEnforce))0x00412890)("Data\\Music\\Lorencia.mp3", FALSE);
		break;
	case 1:
		((void(_cdecl*)(char* Name, BOOL bEnforce))0x00412890)("Data\\Music\\Dungeon.mp3", FALSE);
		break;
	case 2:
		((void(_cdecl*)(char* Name, BOOL bEnforce))0x00412890)("Data\\Music\\Devias.mp3", FALSE);
		break;
	case 3:
		((void(_cdecl*)(char* Name, BOOL bEnforce))0x00412890)("Data\\Music\\Noria.mp3", FALSE);
		break;
	case 4:
		((void(_cdecl*)(char* Name, BOOL bEnforce))0x00412890)("Data\\Music\\LostTower.mp3", FALSE);
		break;
	case 6:
		((void(_cdecl*)(char* Name, BOOL bEnforce))0x00412890)("Data\\Music\\Stadium.mp3", FALSE);
		break;
	case 7:
		((void(_cdecl*)(char* Name, BOOL bEnforce))0x00412890)("Data\\Music\\Atlans.mp3", FALSE);
		break;
	case 8:
		((void(_cdecl*)(char* Name, BOOL bEnforce))0x00412890)("Data\\Music\\Tarkan.mp3", FALSE);
		break;
	case 9:
		((void(_cdecl*)(char* Name, BOOL bEnforce))0x00412890)("Data\\Music\\DevilSquare.mp3", FALSE);
		break;
	case 10:
		((void(_cdecl*)(char* Name, BOOL bEnforce))0x00412890)("Data\\Music\\Icarus.mp3", FALSE);
		break;
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
		((void(_cdecl*)(char* Name, BOOL bEnforce))0x00412890)("Data\\Music\\BloodCastle.mp3", FALSE);
		break;
	default:
		break;
	}

	_asm
	{
		Jmp jmpBack;
	}
}