#include "stdafx.h"
#include "Attack.h"
#include "CustomMessage.h"
#include "Offset.h"
#include "Protocol.h"
#include "Util.h"

int CustomAttack;
DWORD LastTickCount;

void AutoAttackToggle() // OK
{
	STRUCT_DECRYPT;

	if(*(BYTE*)(Hero+846) != 0)
	{
		return;
	}

	if((GetTickCount()-LastTickCount) < 1000)
	{
		return;
	}

	int index = *(BYTE*)(*(BYTE*)(Hero+913)+*(DWORD*)(MAIN_CHARACTER_STRUCT)+87);

	if(index > 0)
	{
		PMSG_CUSTOM_ATTACK_TOGGLE_SEND pMsg;

		pMsg.header.set(0xF3,0x0A,sizeof(pMsg));

		pMsg.index = index;

		DataSend((BYTE*)&pMsg, pMsg.header.size);
	}

	STRUCT_ENCRYPT;
}

void AttackDrawInterface() // OK
{
	if(CustomAttack == 0)
	{
		return;
	}

	EnableAlphaTest(true);
	
	glColor4f(0.0f,0.0f,0.0f,0.6f);
	RenderColor(0,80.f,640.f,25.f);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);

	EnableAlphaTest(false);

	EnableAlphaTest(true);

	SelectObject(m_hFontDC,g_hFontBig);

	SetBackgroundTextColor = 0;

	SetTextColor = Color4f(255,210,40,255);

	DrawInterfaceText(320,85,gCustomMessage.GetMessage(0));

	DisableAlphaBlend();
}

void GCCustomAttackStatusRecv(PMSG_CUSTOM_ATTACK_STATUS_RECV* lpMsg) // OK
{
	if(lpMsg->Started != 0)
	{
		*(BYTE*)(Hero+748) = 0;

		pSetPlayerStop(Hero);

		Attacking = -1;
	}

	CustomAttack = lpMsg->Started;

	LastTickCount = GetTickCount();
}