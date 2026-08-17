// CustomMessage.h: interface for the CCustomMessage class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_CUSTOM_MESSAGE 60

struct CUSTOM_MESSAGE_INFO
{
	int Index;
	char Message[128];
};

class CCustomMessage
{
public:
	CCustomMessage();
	virtual ~CCustomMessage();
	void Init();
	bool Load(char* path);
	void SetInfoEng(CUSTOM_MESSAGE_INFO info);
	void SetInfoPor(CUSTOM_MESSAGE_INFO info);
	void SetInfoSpn(CUSTOM_MESSAGE_INFO info);
public:
	CUSTOM_MESSAGE_INFO m_EngCustomMessageInfo[MAX_CUSTOM_MESSAGE];
	CUSTOM_MESSAGE_INFO m_PorCustomMessageInfo[MAX_CUSTOM_MESSAGE];
	CUSTOM_MESSAGE_INFO m_SpnCustomMessageInfo[MAX_CUSTOM_MESSAGE];
};

extern CCustomMessage gCustomMessage;
