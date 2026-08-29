// CLanguage.cpp: implementation of the CLanguage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Language.h"
#include "CustomMessage.h"
#include "Util.h"

char lang[4],filename[6][MAX_PATH];

void InitLanguage() // OK
{
	HKEY key;

	if(RegOpenKeyEx(HKEY_CURRENT_USER,"SOFTWARE\\Webzen\\Mu\\Config",0,KEY_ALL_ACCESS,&key) == ERROR_SUCCESS)
	{
		DWORD type=REG_SZ,size=sizeof(lang);

		if(RegQueryValueEx(key,"LangSelection",0,&type,(BYTE*)lang,&size) != ERROR_SUCCESS)
		{
			strcpy_s(lang,"Eng");
		}

		RegCloseKey(key);
	}

	if(_stricmp(lang,"Eng") != 0 && _stricmp(lang,"Por") != 0 && _stricmp(lang,"Spn") != 0)
	{
		strcpy_s(lang,"Eng");
	}

	if(_stricmp(lang,"Por") == 0)
	{
		gCustomMessage.m_LangSelected = 1;
	}
	else if(_stricmp(lang,"Spn") == 0)
	{
		gCustomMessage.m_LangSelected = 2;
	}
	else
	{
		gCustomMessage.m_LangSelected = 0;
	}

	wsprintf(filename[0],"Data\\Local\\%s\\Dialog_%s.bmd",lang,lang);
	wsprintf(filename[1],"Data\\Local\\%s\\Item_%s.bmd",lang,lang);
	wsprintf(filename[2],"Data\\Local\\%s\\NpcName_%s.txt",lang,lang);
	wsprintf(filename[3],"Data\\Local\\%s\\Quest_%s.bmd",lang,lang);
	wsprintf(filename[4],"Data\\Local\\%s\\Skill_%s.bmd",lang,lang);
	wsprintf(filename[5],"Data\\Local\\%s\\Text_%s.bmd",lang,lang);

	SetDword(0x00510F26,(DWORD)&filename[0]);
	SetDword(0x00510E21,(DWORD)&filename[1]);
	SetDword(0x00510EF4,(DWORD)&filename[1]);
	SetDword(0x00510F4D,(DWORD)&filename[2]);
	SetDword(0x00510F39,(DWORD)&filename[3]);
	SetDword(0x00510E4D,(DWORD)&filename[4]);
	SetDword(0x00510EFE,(DWORD)&filename[4]);
	SetDword(0x00511019,(DWORD)&filename[5]);
	SetDword(0x00511039,(DWORD)&filename[5]);	
}