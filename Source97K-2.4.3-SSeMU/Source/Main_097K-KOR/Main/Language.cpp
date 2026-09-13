#include "stdafx.h"
#include "Language.h"
#include "Protocol.h"
#include "CustomMessage.h"
#include "Util.h"

Language gLanguage;

Language::Language()
{
	this->iLanguageValues[LANGUAGE_ENGLISH] = std::make_pair<std::string, std::string>("English", "Eng");
	this->iLanguageValues[LANGUAGE_SPANISH] = std::make_pair<std::string, std::string>("Spanish", "Spn");
	this->iLanguageValues[LANGUAGE_PORTUGUESE] = std::make_pair<std::string, std::string>("Portuguese", "Por");

	this->LangNum = LANGUAGE_ENGLISH;

	char szLang[8] = { 0 };
	GetPrivateProfileString("Language", "LangSelection", "", szLang, sizeof(szLang), ".\\Config.ini");

	if (strlen(szLang) > 0)
	{
		strcpy_s(this->m_Language, szLang);
	}
	else
	{
		HKEY key;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, KEY_ALL_ACCESS, &key) == ERROR_SUCCESS)
		{
			DWORD type = REG_SZ, size = sizeof(this->m_Language);
			if (RegQueryValueEx(key, "LangSelection", 0, &type, (BYTE*)this->m_Language, &size) != ERROR_SUCCESS)
			{
				strcpy_s(this->m_Language, "Eng");
			}
			RegCloseKey(key);
		}
		else
		{
			strcpy_s(this->m_Language, "Eng");
		}
	}
}

Language::~Language()
{
	WritePrivateProfileString("Language", "LangSelection", this->m_Language, ".\\Config.ini");

	HKEY key;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, KEY_ALL_ACCESS, &key) == ERROR_SUCCESS)
	{
		RegSetValueEx(key, "LangSelection", 0, REG_SZ, (BYTE*)this->m_Language, 4);
		RegCloseKey(key);
	}
}

void Language::Init()
{
	this->SetLanguage();

	SetDword(0x00510F26, (DWORD)&this->filename[0]);
	SetDword(0x00510E21, (DWORD)&this->filename[1]);
	SetDword(0x00510EF4, (DWORD)&this->filename[1]);
	SetDword(0x00510F4D, (DWORD)&this->filename[2]);
	SetDword(0x00510F39, (DWORD)&this->filename[3]);
	SetDword(0x00510E4D, (DWORD)&this->filename[4]);
	SetDword(0x00510EFE, (DWORD)&this->filename[4]);
	SetDword(0x00511019, (DWORD)&this->filename[5]);
	SetDword(0x00511039, (DWORD)&this->filename[5]);
}

void Language::ReloadLanguage()
{
	this->SetLanguage();

	WritePrivateProfileString("Language", "LangSelection", this->m_Language, ".\\Config.ini");

	HKEY key;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Webzen\\Mu\\Config", 0, KEY_ALL_ACCESS, &key) == ERROR_SUCCESS)
	{
		RegSetValueEx(key, "LangSelection", 0, REG_SZ, (BYTE*)this->m_Language, 4);
		RegCloseKey(key);
	}

	OpenTextData();

	OpenDialogFile(this->filename[0]); // filename[0] is Dialog

	this->SendLanguage();
}

void Language::SetLanguage()
{
	if (_stricmp(this->m_Language, "Eng") != 0 && _stricmp(this->m_Language, "Por") != 0 && _stricmp(this->m_Language, "Spn") != 0)
	{
		strcpy_s(this->m_Language, "Eng");
	}

	if (_stricmp(this->m_Language, "Por") == 0)
	{
		gCustomMessage.m_LangSelected = 1;
		this->LangNum = LANGUAGE_PORTUGUESE;
	}
	else if (_stricmp(this->m_Language, "Spn") == 0)
	{
		gCustomMessage.m_LangSelected = 2;
		this->LangNum = LANGUAGE_SPANISH;
	}
	else
	{
		gCustomMessage.m_LangSelected = 0;
		this->LangNum = LANGUAGE_ENGLISH;
	}

	wsprintf(this->filename[0], "Data\\Local\\%s\\Dialog_%s.bmd", this->m_Language, this->m_Language);
	wsprintf(this->filename[1], "Data\\Local\\%s\\Item_%s.bmd", this->m_Language, this->m_Language);
	wsprintf(this->filename[2], "Data\\Local\\%s\\NpcName_%s.txt", this->m_Language, this->m_Language);
	wsprintf(this->filename[3], "Data\\Local\\%s\\Quest_%s.bmd", this->m_Language, this->m_Language);
	wsprintf(this->filename[4], "Data\\Local\\%s\\Skill_%s.bmd", this->m_Language, this->m_Language);
	wsprintf(this->filename[5], "Data\\Local\\%s\\Text_%s.bmd", this->m_Language, this->m_Language);
}

void Language::SendLanguage()
{
	// Adicione a logica se necessario para o GameServer. O Kayito usava isso com um pacote para notificar o servidor da troca de lingua.
	// Vamos enviar o PMSG_LANGUAGE_RECV para o servidor.
	struct PMSG_LANGUAGE_RECV
	{
		PBMSG_HEAD header;
		BYTE Language;
	};

	PMSG_LANGUAGE_RECV pMsg;
	pMsg.header.set(0x0E, sizeof(pMsg));
	pMsg.Language = this->LangNum;

	// DataSend((BYTE*)&pMsg, pMsg.header.size); // Desabilitado para 97k pois 0x0E entra em conflito com o Ping e causa DC
}