#include "stdafx.h"
#include "ServerList.h"
#include "Offset.h"
#include "Util.h"

int ServerCode = -1;
char ServerName[32][400];

void InitServerList() // OK
{
	SetByte(0x0051F053,0xEB);

	SetByte(0x0051F13D,0x50);

	MemorySet(0x0051F0CF,0x90,0x69);

	SetCompleteHook(0xE8,0x0051F1AE,&PrintServerName1);

	SetCompleteHook(0xE8,0x004ED016,&PrintServerName2);

	SetCompleteHook(0xE8, 0x0051F357, &ServerBoxTexture); // Server
	SetCompleteHook(0xE8, 0x0051F5FD, &ServerBoxTexture); // ServerList
	SetCompleteHook(0xE8, 0x0051F761, &ServerBoxTexture); // ServerListBar
	SetCompleteHook(0xE8, 0x0051F7F7, &ServerBoxTexture); // ServerListBar

	SetCompleteHook(0xE8, 0x0051F39E, &ServerBoxText); // Server
	SetCompleteHook(0xE8, 0x0051F688, &ServerBoxText); // ServerList

	SetDword(0x0051E8B7 + 1, 150 + 135); // ServerMouseX
	SetDword(0x0051E8A5 + 1, 375 + 135); // ServerMouseX
	SetDword(0x0051EC8F + 1, 230 + 135); // ServerListMouseX
	SetDword(0x0051EC9A + 1, 364 + 135); // ServerListMouseX
}

void PrintServerName1(char* a,char* b,char* c,DWORD d) // OK
{
	wsprintf(a,"%s",ServerName[d]);
}

void PrintServerName2(char* a,char* b,char* c,DWORD d) // OK
{
	wsprintf(a,"%s",ServerName[ServerCode]);
}

void ServerBoxTexture(int a, float b, float c, float d, float e, float f, float g, float h, float i, bool j, bool k) // OK
{
	RenderBitmap(a,b + 135.0f,c,d,e,f,g,h,i,j,k);
}

void ServerBoxText(int a, int b, char* c, int d, int e, SIZE* f) // OK
{
	RenderText(a+135,b,c,d,e,f);
}