// Console.cpp: implementation of the CConsole class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Console.h"
#include "Util.h"

bool ConsoleStatus = 0;

void InitConsole() // OK
{
	if(AllocConsole() == 0)
	{
		ErrorMessageBox("Could not open AllocConsole()");
		return;
	}

	SetConsoleTitleA("Mu Online Emulator (Debugger)");

	DeleteMenu(GetSystemMenu(GetConsoleWindow(),0),SC_CLOSE,MF_BYCOMMAND);

	ShowWindow(GetConsoleWindow(),SW_HIDE);
}

void ShowConsole() // OK
{
	ShowWindow(GetConsoleWindow(),((ConsoleStatus ^= 1))?SW_SHOW:SW_HIDE);
}

void ClearConsole() // OK
{
	if(ConsoleStatus == 0)
	{
		return;
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD hCordinates  = {0,0};
    
	CONSOLE_SCREEN_BUFFER_INFO hScreenInfo;

    DWORD hBuffer;

    GetConsoleScreenBufferInfo(hConsole,&hScreenInfo);

    FillConsoleOutputCharacter(hConsole,' ',hScreenInfo.dwSize.X*hScreenInfo.dwSize.Y,hCordinates,&hBuffer);

    FillConsoleOutputAttribute(hConsole,FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,hScreenInfo.dwSize.X*hScreenInfo.dwSize.Y,hCordinates,&hBuffer);

    SetConsoleCursorPosition(hConsole,hCordinates);
}

void Console(int color,char* text,...) // OK
{
	if(ConsoleStatus == 0)
	{
		return;
	}

	char buff[1024];

	va_list arg;
	va_start(arg,text);
	vsprintf_s(buff,text,arg);
	va_end(arg);

	wsprintf(buff,"%s\n",buff);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	switch(color)
	{
		case 0:
			SetConsoleTextAttribute(hConsole,7);
			break;
		case 1:
			SetConsoleTextAttribute(hConsole,FOREGROUND_RED);
			break;
		case 2:
			SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN);
			break;
		case 3:
			SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE);
			break;
	}

	WriteConsole(hConsole,buff,strlen(buff),0,0);

	SetConsoleTextAttribute(hConsole,7);
}