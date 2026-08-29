#include "stdafx.h"
#include "HealthBar.h"
#include "CustomMessage.h"
#include "Offset.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "Util.h"

NEW_HEALTH_BAR gNewHealthBar[MAX_MAIN_VIEWPORT];

bool MonsterHPBarMode = 0;

void HealthBarToggle() // OK
{
	if (SceneFlag == 5)
	{
		MonsterHPBarMode ^= 1;

		CreateNotice(gCustomMessage.GetMessage(4+MonsterHPBarMode),1);
	}
}

void ClearNewHealthBar() // OK
{
	for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
	{
		gNewHealthBar[n].index = 0xFFFF;
		gNewHealthBar[n].type = 0;
		gNewHealthBar[n].rate = 0;
	}
}

void InsertNewHealthBar(WORD index,BYTE type,BYTE rate) // OK
{
	for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
	{
		if (gNewHealthBar[n].index == 0xFFFF)
		{
			gNewHealthBar[n].index = index;
			gNewHealthBar[n].type = type;
			gNewHealthBar[n].rate = rate;
			return;
		}
	}
}

NEW_HEALTH_BAR* GetNewHealthBar(WORD index,BYTE type) // OK
{
	for(int n=0;n < MAX_MAIN_VIEWPORT;n++)
	{
		if(gNewHealthBar[n].index != 0xFFFF)
		{
			if(gNewHealthBar[n].index == index && gNewHealthBar[n].type == type)
			{
				return &gNewHealthBar[n];
			}
		}
	}

	return 0;
}

void DrawNewHealthBar() // OK
{
	((void(*)())0x004BCA20)();

	if(MonsterHPBarMode != 0)
	{
		return;
	}

	DWORD ViewportAddress;
	NEW_HEALTH_BAR* lpHealthBar;
	float Angle[3];
	int PosX, PosY;
	float LifeBarWidth = 50.0f;
	char LifeDisplay[64];

	for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
	{
		ViewportAddress = CharactersClient + (n * 916);

		if (!ViewportAddress)
		{
			continue;
		}

		if (*(BYTE*)(ViewportAddress) == 0)
		{
			continue;
		}

		lpHealthBar = GetNewHealthBar(*(WORD*)(ViewportAddress + 0x1DC), *(BYTE*)(ViewportAddress + 0x84));

		if (lpHealthBar == 0)
		{
			continue;
		}

		Angle[0] = *(float*)(ViewportAddress + 0x10);

		Angle[1] = *(float*)(ViewportAddress + 0x14);

		Angle[2] = *(float*)(ViewportAddress + 0x12C) + *(float*)(ViewportAddress + 0x18) + 100.0f;

		Projection(Angle, &PosX, &PosY);

		PosX -= (int)floor((double)LifeBarWidth / 2.0);

		if ((MouseX >= PosX) && (MouseX <= PosX + (int)LifeBarWidth) && (MouseY >= PosY - 2) && (MouseY < PosY + 6))
		{
			EnableAlphaTest(true);

			wsprintf(LifeDisplay, "%s: %d%%", (char*)(ViewportAddress + 449), lpHealthBar->rate);

			SelectObject(m_hFontDC,g_hFont);

			SetBackgroundTextColor = Color4f(0,0,0,192);

			SetTextColor = Color4f(255,255,255,255);

			RenderText(PosX, PosY - 8, LifeDisplay, (int)LifeBarWidth * WindowWidth / 640, 1, 0);
		}

		EnableAlphaTest(true);

		glColor4f(0.0f, 0.0f, 0.0f, 0.75f);

		RenderColor((float)PosX, (float)PosY, LifeBarWidth, 6.0f);
 
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		RenderColor((float)PosX + 2, (float)PosY + 2, ((LifeBarWidth - 4) * lpHealthBar->rate) / 100, 2.0f);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glEnable(GL_TEXTURE_2D);

		DisableAlphaBlend();
	}
}