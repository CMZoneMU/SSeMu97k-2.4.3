#include "stdafx.h"
#include "GoldenArcher.h"
#include "CustomMessage.h"
#include "ItemMove.h"
#include "Offset.h"
#include "Protect.h"
#include "Shop.h"
#include "Util.h"
#include "WindowTime.h"

int RegisterRenaCount = 0;
int AvailableRenaCount = 0;
bool ButtonStatus[6] = {false,false,false,false,false,false};
DWORD TickCount = GetTickCount();

void InitGoldenArcher() // OK
{
	if(gProtect.m_MainInfo.GoldenArcherRenewalSwitch == 0)
	{
		return;
	}

	SetFloat(0x004F553E+1,(float)(WINDOW_POSX+30)); // Move Rena Position

	SetFloat(0x004F5558+1,(float)(WINDOW_POSX+30)); // Move Rena Position

	SetCompleteHook(0xE9,0x004F46A0,&DrawGoldenArcher);

	SetCompleteHook(0xE9,0x004E7AC0,&ControlGoldenArcher);
}

void DrawGoldenArcher() // OK
{
	if(EventChipDialogEnable)
	{
		AvailableRenaCount = GetInventoryItemCount(GET_ITEM(14,21),0);

		if(GetTickCount() >= TickCount)
		{
			ButtonStatus[0] = ((RegisterRenaCount >= 10)?true:false);
			
			ButtonStatus[1] = ((RegisterRenaCount >= 20)?true:false);
			
			ButtonStatus[2] = ((RegisterRenaCount >= 30)?true:false);
			
			ButtonStatus[3] = ((RegisterRenaCount >= 50)?true:false);
			
			ButtonStatus[4] = ((RegisterRenaCount >= 100)?true:false);
			
			ButtonStatus[5] = ((AvailableRenaCount >= 1)?true:false);
		}

		RenderBitmap(260, WINDOW_POSX, WINDOW_POSY, 190.0f, 256.0f, 0.0f, 0.0f, 0.7421875f, 1, 1, 1);

		RenderBitmap(261, WINDOW_POSX, WINDOW_POSY + 256, 190.0f, 177.0f, 0.0f, 0.0f, 0.7421875f, 0.69140625, 1, 1);

		RenderBitmap(280, CLOSE_BT_POSX, CLOSE_BT_POSY, 24.0f, 24.0f, 0.0f, 0.0f, 24.0f / 32.0f, 24.0f / 32.0f, 1, 1);

		WindowRenderText((int)WINDOW_POSX + 95, 15, g_hFontBold, 1, 0, gCustomMessage.GetMessage(42));

		for(int n = 0; n < 6; n++)
		{
			if(!ButtonStatus[n])
			{ 
				glColor3f(0.4f, 0.4f, 0.4f); 
			}

			RenderBitmap(240, WINDOW_POSX + 35.0f, WINDOW_POSY + 60.0f + (float)(n * 30) + ((n == 5) ? 30.0f : 0), 120.0f, 24.0f, 0, 0, 0.83203125, 1.0, 1, 1);
			
			WindowRenderText(WINDOW_POSX + 95, WINDOW_POSY + 72 + (n * 30) + ((n == 5) ? 30 : 0), g_hFont, 1, 0, gCustomMessage.GetMessage(43+n));
			
			glColor3f(1.f, 1.f, 1.f);
		}

		WindowRenderText(WINDOW_POSX + 50, 306, g_hFontBold, 0, 0, gCustomMessage.GetMessage(49),RegisterRenaCount);

		WindowRenderText(WINDOW_POSX + 50, 342, g_hFontBold, 0, 0, gCustomMessage.GetMessage(50),AvailableRenaCount);

		if(pMouseOnZone(CLOSE_BT_POSX, CLOSE_BT_POSY, 24, 24, 1))
		{
			pRenderTipText(CLOSE_BT_POSX + 2, CLOSE_BT_POSY-13, gCustomMessage.GetMessage(39));
		}
	}
}

void ControlGoldenArcher() // OK
{
	if(EventChipDialogEnable)
	{
		if(pMouseOnZone(WINDOW_POSX, WINDOW_POSY, 190, 433, 1))
		{
			MouseOnWindow = true;

			if(pMouseOnZone(WINDOW_POSX + 35, WINDOW_POSY + 60, 120, 24, 1) && MouseLButtonPush && ButtonStatus[0])
			{
				PMSG_GOLDEN_ARCHER_REWARD_SEND pMsg;

				pMsg.header.set(0x96,sizeof(pMsg));

				pMsg.type = 0;

				DataSend((BYTE*)&pMsg,pMsg.header.size);

				ButtonStatus[0] = false;

				GoldenArcherButtonPush();
			}
			else if(pMouseOnZone(WINDOW_POSX + 35, WINDOW_POSY + 90, 120, 24, 1) && MouseLButtonPush && ButtonStatus[1])
			{
				PMSG_GOLDEN_ARCHER_REWARD_SEND pMsg;

				pMsg.header.set(0x96,sizeof(pMsg));

				pMsg.type = 1;

				DataSend((BYTE*)&pMsg,pMsg.header.size);

				ButtonStatus[1] = false;

				GoldenArcherButtonPush();
			}
			else if(pMouseOnZone(WINDOW_POSX + 35, WINDOW_POSY + 120, 120, 24, 1) && MouseLButtonPush && ButtonStatus[2])
			{
				PMSG_GOLDEN_ARCHER_REWARD_SEND pMsg;

				pMsg.header.set(0x96,sizeof(pMsg));

				pMsg.type = 2;

				DataSend((BYTE*)&pMsg,pMsg.header.size);

				ButtonStatus[2] = false;

				GoldenArcherButtonPush();
			}
			else if(pMouseOnZone(WINDOW_POSX + 35, WINDOW_POSY + 150, 120, 24, 1) && MouseLButtonPush && ButtonStatus[3])
			{
				PMSG_GOLDEN_ARCHER_REWARD_SEND pMsg;

				pMsg.header.set(0x96,sizeof(pMsg));

				pMsg.type = 3;

				DataSend((BYTE*)&pMsg,pMsg.header.size);

				ButtonStatus[3] = false;

				GoldenArcherButtonPush();
			}
			else if(pMouseOnZone(WINDOW_POSX + 35, WINDOW_POSY + 180, 120, 24, 1) && MouseLButtonPush && ButtonStatus[4])
			{
				PMSG_GOLDEN_ARCHER_REWARD_SEND pMsg;

				pMsg.header.set(0x96,sizeof(pMsg));

				pMsg.type = 4;

				DataSend((BYTE*)&pMsg,pMsg.header.size);

				ButtonStatus[4] = false;

				GoldenArcherButtonPush();
			}
			else if(pMouseOnZone(WINDOW_POSX + 35, WINDOW_POSY + 240, 120, 24, 1) && MouseLButtonPush && ButtonStatus[5])
			{
				ButtonStatus[5] = false;

				GoldenArcherButtonPush();

				PBMSG_HEAD pMsg;

				pMsg.set(0x95,sizeof(pMsg));

				DataSend((BYTE*)&pMsg,pMsg.size);
			}
			else if(pMouseOnZone(CLOSE_BT_POSX, CLOSE_BT_POSY, 24, 24, 1) && MouseLButtonPush)
			{
				GoldenArcherButtonPush();

				EventChipDialogEnable = 0;

				PBMSG_HEAD pMsg;

				pMsg.set(0x97,sizeof(pMsg));

				DataSend((BYTE*)&pMsg,pMsg.size);
			}
		}
	}
}

void GoldenArcherButtonPush() // OK
{
	TickCount = GetTickCount() + 100;

	MouseLButtonPush = 0;
	MouseUpdateTime = 0;
	
	MouseUpdateTimeMax = 6;

	PlayBuffer(25, 0, 0);
}

void GoldenArcherCountRecv(PMSG_GOLDEN_ARCHER_COUNT_RECV* lpMsg) // OK
{
	EventChipDialogEnable = lpMsg->type + 1;

	RegisterRenaCount = lpMsg->count;
}