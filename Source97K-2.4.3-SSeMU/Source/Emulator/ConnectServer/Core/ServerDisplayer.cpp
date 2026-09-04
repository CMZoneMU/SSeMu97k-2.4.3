// ServerDisplayer.cpp: implementation of the CServerDisplayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerDisplayer.h"
#include "ClientManager.h"
#include "../Utils/Log.h"
#include "../Resource.h"
#include "ServerList.h"
#include "../Network/SocketManager.h"
#include "../Utils/Util.h"

CServerDisplayer gServerDisplayer;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerDisplayer::CServerDisplayer() // OK
{
	for(int n=0;n < MAX_LOG_TEXT_LINE;n++)
	{
		memset(&this->m_log[n],0,sizeof(this->m_log[n]));
	}

	this->m_brush = (HBRUSH)GetStockObject(WHITE_BRUSH);
}

CServerDisplayer::~CServerDisplayer() // OK
{
	DeleteObject(this->m_brush);
}

void CServerDisplayer::Init(HWND hWnd) // OK
{
	PROTECT_START

	this->m_hwnd = hWnd;

	PROTECT_FINAL

	gLog.AddLog(1,"LOG");

	gLog.AddLog(1,".\\LOG\\HACK_LOG");

	gLog.AddLog(1,".\\LOG\\CONNECTION_LOG");
}

void CServerDisplayer::Run() // OK
{
	this->LogTextPaint();

	this->PaintBarInfo();
}

void CServerDisplayer::LogTextPaint() // OK
{
	RECT rect;

	GetClientRect(this->m_hwnd,&rect);

	rect.top = 80;
	rect.bottom = 530;

	HDC hdc = GetDC(this->m_hwnd);

	FillRect(hdc,&rect,this->m_brush);

	int line = MAX_LOG_TEXT_LINE;

	int count = (((this->m_count-1)>=0)?(this->m_count-1):(MAX_LOG_TEXT_LINE-1));

	for(int n=0;n < MAX_LOG_TEXT_LINE;n++)
	{
		switch(this->m_log[count].color)
		{
			case LOG_BLACK:
				SetTextColor(hdc,RGB(20,20,20));
				break;
			case LOG_RED:
				SetTextColor(hdc,RGB(220,10,10));
				break;
			case LOG_GREEN:
				SetTextColor(hdc,RGB(0,150,0));
				break;
			case LOG_BLUE:
				SetTextColor(hdc,RGB(0,160,255));
				break;
		}

		SetBkMode(hdc,TRANSPARENT);

		int size = strlen(this->m_log[count].text);

		if(size > 1)
		{
			TextOut(hdc,0,(65+(line*15)),this->m_log[count].text,size);
			line--;
		}

		count = (((--count)>=0)?count:(MAX_LOG_TEXT_LINE-1));
	}

	ReleaseDC(this->m_hwnd,hdc);
}

void CServerDisplayer::PaintBarInfo() // OK
{
	HWND hWndStatusBar = GetDlgItem(this->m_hwnd,IDC_STATUSBAR);

	char buff[256];

	wsprintf(buff,"UserCount: %d/%d",GetUserCount(),MAX_CLIENT);
	SendMessage(hWndStatusBar,SB_SETTEXT,0,(LPARAM)buff);

	wsprintf(buff,"ServerCount: %d/%d",gServerList.m_GameServersList,gServerList.m_GameServersCount);
	SendMessage(hWndStatusBar,SB_SETTEXT,1,(LPARAM)buff);

	wsprintf(buff,"QueueSize: %d",gSocketManager.GetQueueSize());
	SendMessage(hWndStatusBar,SB_SETTEXT,2,(LPARAM)buff);

	SendMessage(hWndStatusBar,SB_SETTEXT,3,0);

	ShowWindow(hWndStatusBar,SW_SHOW);
}

void CServerDisplayer::LogAddText(eLogColor color,char* text,int size) // OK
{
	PROTECT_START

	size = ((size>=MAX_LOG_TEXT_SIZE)?(MAX_LOG_TEXT_SIZE-1):size);

	memset(&this->m_log[this->m_count].text,0,sizeof(this->m_log[this->m_count].text));

	memcpy(&this->m_log[this->m_count].text,text,size);

	this->m_log[this->m_count].color = color;

	this->m_count = (((++this->m_count)>=MAX_LOG_TEXT_LINE)?0:this->m_count);

	PROTECT_FINAL

	gLog.Output(LOG_GENERAL,"%s",&text[9]);
}
