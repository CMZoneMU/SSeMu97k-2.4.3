#pragma once

#include <string>
#include <vector>
#include <map>

enum eLanguages
{
	LANGUAGE_ENGLISH = 0,
	LANGUAGE_SPANISH = 1,
	LANGUAGE_PORTUGUESE = 2,
	MAX_LANGUAGE
};

class Language
{
public:
	Language();
	virtual ~Language();
	void Init();
	void ReloadLanguage();
	void SendLanguage();

private:
	void SetLanguage();

public:
	int LangNum;
	char m_Language[4];
	std::pair<std::string, std::string> iLanguageValues[MAX_LANGUAGE];

private:
	char filename[6][MAX_PATH];
};

extern Language gLanguage;