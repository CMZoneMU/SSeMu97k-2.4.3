#include "stdafx.h"
#include "Resource.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "CommandManager.h"
#include "Console.h"
#include "DevilSquare.h"
#include "GameServer.h"
#include "GameMain.h"
#include "Gate.h"
#include "InvasionManager.h"
#include "JSProtocol.h"
#include "MapManager.h"
#include "Message.h"
#include "MiniDump.h"
#include "Notice.h"
#include "QueueTimer.h"
#include "ServerDisplayer.h"
#include "ServerInfo.h"
#include "SocketManager.h"
#include "SocketManagerUdp.h"
#include "Util.h"

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
HWND hWnd;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) // OK
{
	CMiniDump::Start();

	LoadString(hInstance,IDS_APP_TITLE,szTitle,MAX_LOADSTRING);
	LoadString(hInstance,IDC_GAMESERVER,szWindowClass,MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	if(InitInstance(hInstance,nCmdShow) == 0)
	{
		return 0;
	}

	SetLargeRand();

	gCloseServer = 0;

	gServerInfo.ReadStartupInfo("GameServerInfo",".\\Data\\GameServerInfo - Common.dat");

	InitConsole();

	char buff[256];

	wsprintf(buff,GAMESERVER_VERSION,PROTOCOL_NAME,gServerInfo.m_ServerName,PROTOCOL_NAME);

	SetWindowText(hWnd,buff);

	gServerDisplayer.Init(hWnd);

	WSADATA wsa;

	if(WSAStartup(MAKEWORD(2,2),&wsa) == 0)
	{
		if(gSocketManager.Start((WORD)gServerInfo.m_ServerPort) == 0)
		{
			LogAdd(LOG_RED,"Could not start GameServer");
		}
		else
		{
			GameMainInit(hWnd);

			JoinServerConnect(WM_JOIN_SERVER_MSG_PROC);

			DataServerConnect(WM_DATA_SERVER_MSG_PROC);

			gSocketManagerUdp.Connect(gServerInfo.m_ConnectServerAddress,(WORD)gServerInfo.m_ConnectServerPort);

			SetTimer(hWnd,WM_TIMER_1000,1000,0);

			SetTimer(hWnd,WM_TIMER_2000,2000,0);

			SetTimer(hWnd,WM_TIMER_10000,10000,0);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER_MOVE,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_EVENT,100,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_VIEWPORT,1000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_FIRST,1000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_CLOSE,1000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_ACCOUNT_LEVEL,60000,&QueueTimerCallback);

			gQueueTimer.CreateTimer(QUEUE_TIMER_LUA,100,&QueueTimerCallback);
		}
	}
	else
	{
		LogAdd(LOG_RED,"WSAStartup() failed with error: %d",WSAGetLastError());
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance,(LPCTSTR)IDC_GAMESERVER);

	MSG msg;

	while(GetMessage(&msg,0,0,0) != 0)
	{
		if(TranslateAccelerator(msg.hwnd,hAccelTable,&msg) == 0)
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	CMiniDump::Clean();

	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) // OK
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance,(LPCTSTR)(IDI_GAMESERVER));
	wcex.hCursor = LoadCursor(0,IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = (LPCSTR)IDC_GAMESERVER;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance,(LPCTSTR)(IDI_GAMESERVER));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance,int nCmdShow) // OK
{
	hInst = hInstance;

	hWnd = CreateWindow(szWindowClass,szTitle,WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,CW_USEDEFAULT,0,600,600,0,0,hInstance,0);

	if(hWnd == 0)
	{
		return 0;
	}

	RenderMenu(hWnd);

	HWND hWndSB = CreateWindowEx(0,STATUSCLASSNAME,NULL,WS_CHILD | WS_VISIBLE,0,0,0,0,hWnd,(HMENU)IDC_STATUSBAR,hInstance,NULL);
	ShowWindow(hWndSB,SW_HIDE);

	int parts[] = { 140,300,400,-1 };

	SendMessage(hWndSB,SB_SETPARTS,4,(LPARAM)parts);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	return 1;
}

VOID RenderMenu(HWND hWnd) // OK
{
	HMENU hMenu = CreateMenu();

	HMENU hFile = CreateMenu();
    
	AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hFile,"&File");
	
	AppendMenu(hFile,MF_STRING,IDM_FILE_ALLUSERDISCONNECT,"Disconnect all users");
	AppendMenu(hFile,MF_STRING,IDM_FILE_OFFLINEUSERDISCONNECT,"Disconnect all offline users");
	AppendMenu(hFile,MF_SEPARATOR,0,0);


	HMENU hCloseServer = CreatePopupMenu();

	AppendMenu(hFile,MF_STRING|MF_POPUP,(UINT_PTR)hCloseServer,"Server Close");

	AppendMenu(hCloseServer,MF_STRING,IDM_FILE_10MINUTESERVERCLOSE,"Close server in 10 minutes");
	AppendMenu(hCloseServer,MF_STRING,IDM_FILE_5MINUTESERVERCLOSE,"Close server in 5 minutes");
	AppendMenu(hCloseServer,MF_STRING,IDM_FILE_3MINUTESERVERCLOSE,"Close server in 3 minutes");
	AppendMenu(hCloseServer,MF_STRING,IDM_FILE_1MINUTESERVERCLOSE,"Close server in 1 minute");
	
	AppendMenu(hFile,MF_SEPARATOR,0,0);
	AppendMenu(hFile,MF_STRING,IDM_EXIT,"E&xit");

	HMENU hConsole = CreateMenu();
    
	AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hConsole,"&Console");
	
	AppendMenu(hConsole,MF_STRING,IDM_CONSOLE_SHOW,"Show/Hide");
	AppendMenu(hConsole,MF_SEPARATOR,0,0);
	AppendMenu(hConsole,MF_STRING,IDM_CONSOLE_CLEAR,"Clear");

	HMENU hEvents = CreateMenu();
	
	AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hEvents,"&Events");
	
	HMENU hBonus = CreatePopupMenu();

	AppendMenu(hEvents,MF_STRING|MF_POPUP,(UINT_PTR)hBonus,"&Bonus Manager");

	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS0,"Start Bonus 0");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS1,"Start Bonus 1");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS2,"Start Bonus 2");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS3,"Start Bonus 3");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS4,"Start Bonus 4");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS5,"Start Bonus 5");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS6,"Start Bonus 6");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS7,"Start Bonus 7");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS8,"Start Bonus 8");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS9,"Start Bonus 9");
	AppendMenu(hBonus,MF_SEPARATOR,0,0);
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS10,"Start Bonus 10");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS11,"Start Bonus 11");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS12,"Start Bonus 12");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS13,"Start Bonus 13");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS14,"Start Bonus 14");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS15,"Start Bonus 15");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS16,"Start Bonus 16");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS17,"Start Bonus 17");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS18,"Start Bonus 18");
	AppendMenu(hBonus,MF_STRING,IDM_FORCE_BONUS19,"Start Bonus 19");

	HMENU hInvasion = CreatePopupMenu();
	
	AppendMenu(hEvents,MF_STRING|MF_POPUP,(UINT_PTR)hInvasion,"&Invasion Manager");
	
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION0,"Start Invasion 0");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION1,"Start Invasion 1");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION2,"Start Invasion 2");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION3,"Start Invasion 3");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION4,"Start Invasion 4");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION5,"Start Invasion 5");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION6,"Start Invasion 6");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION7,"Start Invasion 7");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION8,"Start Invasion 8");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION9,"Start Invasion 9");
	AppendMenu(hInvasion,MF_SEPARATOR,0,0);
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION10,"Start Invasion 10");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION11,"Start Invasion 11");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION12,"Start Invasion 12");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION13,"Start Invasion 13");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION14,"Start Invasion 14");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION15,"Start Invasion 15");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION16,"Start Invasion 16");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION17,"Start Invasion 17");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION18,"Start Invasion 18");
	AppendMenu(hInvasion,MF_STRING,IDM_FORCE_INVASION19,"Start Invasion 19");
	
	AppendMenu(hEvents,MF_SEPARATOR,0,0);
	
	AppendMenu(hEvents,MF_STRING,IDM_EVENT_FORCEDEVILSQUARE,"Start Devil Square");
	AppendMenu(hEvents,MF_STRING,IDM_EVENT_FORCEBLOODCASTLE,"Start Blood Castle");
	HMENU hReload = CreateMenu();
	
	AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hReload,"Reload");
	
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADALL,"Reload All");
	AppendMenu(hReload,MF_SEPARATOR,0,0);
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADBLACKLIST,"Reload BlackList");
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADCHAOSMIX,"Reload ChaosMix");
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADCHARACTER,"Reload Character");
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADCOMMAND,"Reload Command");
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADCOMMON,"Reload Common");
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADCUSTOM,"Reload Custom");
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADEVENT,"Reload Event");
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADEVENTITEMBAG,"Reload EventItemBag");
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADHACK,"Reload Hack");
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADITEM,"Reload Item");
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADMONSTER,"Reload Monster");
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADMOVE,"Reload Move");
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADQUEST,"Reload Quest");
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADSCRIPT,"Reload Script");
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADSHOP,"Reload Shop");
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADSKILL,"Reload Skill");
	AppendMenu(hReload,MF_STRING,IDM_RELOAD_RELOADUTIL,"Reload Util");
	
	HMENU hTool = CreateMenu();
	
	AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hTool,"Tool");
	
	AppendMenu(hTool,MF_STRING,IDM_USERS,"Connection Manager");
	AppendMenu(hTool,MF_STRING,IDM_MAP_VIEWER,"Object Map Viewer");
	AppendMenu(hTool,MF_SEPARATOR,0,0);
	AppendMenu(hTool,MF_STRING,IDM_TOOL_GENERATE_ITEM,"Generate Item.bmd");
	
	HMENU hHelp = CreateMenu();
	
	AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hHelp,"&Help");
	AppendMenu(hHelp,MF_STRING,IDM_SSEMU,"SSeMU Web");
	AppendMenu(hHelp,MF_SEPARATOR,0,0);
	AppendMenu(hHelp,MF_STRING,IDM_ABOUT,"&About ...");

    SetMenu(hWnd,hMenu);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	switch(message)
	{
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDM_EXIT:
					if(gObjTotalUser != 0)
					{
						MessageBox(hWnd,"Can not close gameserver with connected players,please disconnect them first!","Danger of data loss!",MB_OK | MB_ICONEXCLAMATION);
						break;
					}

					if(MessageBox(hWnd,"Are you sure to terminate GameServer?","Ask terminate server",MB_YESNO | MB_ICONQUESTION) == IDYES)
					{
						DestroyWindow(hWnd);
					}
					break;
				case IDM_FILE_ALLUSERDISCONNECT:
					gObjAllDisconnect();
					break;
				case IDM_FILE_OFFLINEUSERDISCONNECT:
					gObjOfflineDisconnect();
					break;
				case IDM_FILE_1MINUTESERVERCLOSE:
					if(gCloseMsg == 0)
					{
						gCloseMsg = 1;
						gCloseMsgTime = 60;
						gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(286),gCloseMsgTime/60);
					}
					break;
				case IDM_FILE_3MINUTESERVERCLOSE:
					if(gCloseMsg == 0)
					{
						gCloseMsg = 1;
						gCloseMsgTime = 180;
						gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(286),gCloseMsgTime/60);
					}
					break;
				case IDM_FILE_5MINUTESERVERCLOSE:
					if(gCloseMsg == 0)
					{
						gCloseMsg = 1;
						gCloseMsgTime = 300;
						gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(286),gCloseMsgTime/60);
					}
					break;
				case IDM_FILE_10MINUTESERVERCLOSE:
					if(gCloseMsg == 0)
					{
						gCloseMsg = 1;
						gCloseMsgTime = 600;
						gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(286),gCloseMsgTime/60);
					}
					break;
				case IDM_CONSOLE_SHOW:
					ShowConsole();
					break;
				case IDM_CONSOLE_CLEAR:
					ClearConsole();
					break;
				case IDM_EVENT_FORCEBLOODCASTLE:
					gBloodCastle.ForceStart();
					break;
				case IDM_EVENT_FORCEDEVILSQUARE:
					gDevilSquare.ForceStart();
					break;
				case IDM_FORCE_BONUS0:
					gBonusManager.ForceStart(0);
					break;
				case IDM_FORCE_BONUS1:
					gBonusManager.ForceStart(1);
					break;
				case IDM_FORCE_BONUS2:
					gBonusManager.ForceStart(2);
					break;
				case IDM_FORCE_BONUS3:
					gBonusManager.ForceStart(3);
					break;
				case IDM_FORCE_BONUS4:
					gBonusManager.ForceStart(4);
					break;
				case IDM_FORCE_BONUS5:
					gBonusManager.ForceStart(5);
					break;
				case IDM_FORCE_BONUS6:
					gBonusManager.ForceStart(6);
					break;
				case IDM_FORCE_BONUS7:
					gBonusManager.ForceStart(7);
					break;
				case IDM_FORCE_BONUS8:
					gBonusManager.ForceStart(8);
					break;
				case IDM_FORCE_BONUS9:
					gBonusManager.ForceStart(9);
					break;
				case IDM_FORCE_BONUS10:
					gBonusManager.ForceStart(10);
					break;
				case IDM_FORCE_BONUS11:
					gBonusManager.ForceStart(11);
					break;
				case IDM_FORCE_BONUS12:
					gBonusManager.ForceStart(12);
					break;
				case IDM_FORCE_BONUS13:
					gBonusManager.ForceStart(13);
					break;
				case IDM_FORCE_BONUS14:
					gBonusManager.ForceStart(14);
					break;
				case IDM_FORCE_BONUS15:
					gBonusManager.ForceStart(15);
					break;
				case IDM_FORCE_BONUS16:
					gBonusManager.ForceStart(16);
					break;
				case IDM_FORCE_BONUS17:
					gBonusManager.ForceStart(17);
					break;
				case IDM_FORCE_BONUS18:
					gBonusManager.ForceStart(18);
					break;
				case IDM_FORCE_BONUS19:
					gBonusManager.ForceStart(19);
					break;
				case IDM_FORCE_INVASION0:
					gInvasionManager.ForceStart(0);
					break;
				case IDM_FORCE_INVASION1:
					gInvasionManager.ForceStart(1);
					break;
				case IDM_FORCE_INVASION2:
					gInvasionManager.ForceStart(2);
					break;
				case IDM_FORCE_INVASION3:
					gInvasionManager.ForceStart(3);
					break;
				case IDM_FORCE_INVASION4:
					gInvasionManager.ForceStart(4);
					break;
				case IDM_FORCE_INVASION5:
					gInvasionManager.ForceStart(5);
					break;
				case IDM_FORCE_INVASION6:
					gInvasionManager.ForceStart(6);
					break;
				case IDM_FORCE_INVASION7:
					gInvasionManager.ForceStart(7);
					break;
				case IDM_FORCE_INVASION8:
					gInvasionManager.ForceStart(8);
					break;
				case IDM_FORCE_INVASION9:
					gInvasionManager.ForceStart(9);
					break;
				case IDM_FORCE_INVASION10:
					gInvasionManager.ForceStart(10);
					break;
				case IDM_FORCE_INVASION11:
					gInvasionManager.ForceStart(11);
					break;
				case IDM_FORCE_INVASION12:
					gInvasionManager.ForceStart(12);
					break;
				case IDM_FORCE_INVASION13:
					gInvasionManager.ForceStart(13);
					break;
				case IDM_FORCE_INVASION14:
					gInvasionManager.ForceStart(14);
					break;
				case IDM_FORCE_INVASION15:
					gInvasionManager.ForceStart(15);
					break;
				case IDM_FORCE_INVASION16:
					gInvasionManager.ForceStart(16);
					break;
				case IDM_FORCE_INVASION17:
					gInvasionManager.ForceStart(17);
					break;
				case IDM_FORCE_INVASION18:
					gInvasionManager.ForceStart(18);
					break;
				case IDM_FORCE_INVASION19:
					gInvasionManager.ForceStart(19);
					break;
				case IDM_RELOAD_RELOADALL:
					gServerInfo.ReadBlackListInfo();
					gServerInfo.ReadChaosMixInfo();
					gServerInfo.ReadCharacterInfo();
					gServerInfo.ReadCommandInfo();
					gServerInfo.ReadCommonInfo();
					gServerInfo.ReadCustomInfo();
					gServerInfo.ReadEventInfo();
					gServerInfo.ReadEventItemBagInfo();
					gServerInfo.ReadHackInfo();
					gServerInfo.ReadItemInfo();
					gServerInfo.ReadMoveInfo();
					gServerInfo.ReadQuestInfo();
					gServerInfo.ReadScriptInfo();
					gServerInfo.ReadSkillInfo();
					gServerInfo.ReadUtilInfo();
					gServerInfo.ReloadMonsterInfo();
					gServerInfo.ReadShopInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] All options reloaded successfully");
					break;
				case IDM_RELOAD_RELOADBLACKLIST:
					gServerInfo.ReadBlackListInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] BlackList reloaded successfully");
					break;
				case IDM_RELOAD_RELOADCHAOSMIX:
					gServerInfo.ReadChaosMixInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] ChaosMix reloaded successfully");
					break;
				case IDM_RELOAD_RELOADCHARACTER:
					gServerInfo.ReadCharacterInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Character reloaded successfully");
					break;
				case IDM_RELOAD_RELOADCOMMAND:
					gServerInfo.ReadCommandInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Command reloaded successfully");
					break;
				case IDM_RELOAD_RELOADCOMMON:
					gServerInfo.ReadCommonInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Common reloaded successfully");
					break;
				case IDM_RELOAD_RELOADCUSTOM:
					gServerInfo.ReadCustomInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Custom reloaded successfully");
					break;
				case IDM_RELOAD_RELOADEVENT:
					gServerInfo.ReadEventInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Event reloaded successfully");
					break;
				case IDM_RELOAD_RELOADEVENTITEMBAG:
					gServerInfo.ReadEventItemBagInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] EventItemBag reloaded successfully");
					break;
				case IDM_RELOAD_RELOADHACK:
					gServerInfo.ReadHackInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Hack reloaded successfully");
					break;
				case IDM_RELOAD_RELOADITEM:
					gServerInfo.ReadItemInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Item reloaded successfully");
					break;
				case IDM_RELOAD_RELOADMONSTER:
					gServerInfo.ReloadMonsterInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Monster reloaded successfully");
					break;
				case IDM_RELOAD_RELOADMOVE:
					gServerInfo.ReadMoveInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Move reloaded successfully");
					break;
				case IDM_RELOAD_RELOADQUEST:
					gServerInfo.ReadQuestInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Quest reloaded successfully");
					break;
				case IDM_RELOAD_RELOADSCRIPT:
					gServerInfo.ReadScriptInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Script reloaded successfully");
					break;
				case IDM_RELOAD_RELOADSHOP:
					gServerInfo.ReadShopInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Shop reloaded successfully");
					break;
				case IDM_RELOAD_RELOADSKILL:
					gServerInfo.ReadSkillInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Skill reloaded successfully");
					break;
				case IDM_RELOAD_RELOADUTIL:
					gServerInfo.ReadUtilInfo();
					LogAdd(LOG_BLUE,"[ServerInfo] Util reloaded successfully");
					break;
				case IDM_TOOL_GENERATE_ITEM:
					gServerInfo.WriteItemInfo();
					break;
				case IDM_USERS:
					DialogBox(hInst,(LPCTSTR)IDD_USERS,hWnd,(DLGPROC)Users);
					break;
				case IDM_MAP_VIEWER:
					DialogBox(hInst,(LPCTSTR)IDD_MAP_VIEWER,hWnd,(DLGPROC)MapViewer);
					break;
				case IDM_ABOUT:
					DialogBox(hInst,(LPCTSTR)IDD_ABOUTBOX,hWnd,(DLGPROC)About);
					break;
				case IDM_SSEMU:
					ShellExecute(0,0,"https://www.ssemu.com.ar",0,0,SW_SHOW);
					break;
				default:
					return DefWindowProc(hWnd,message,wParam,lParam);
			}
			break;
		case WM_TIMER:
			switch(wParam)
			{
				case WM_TIMER_1000:
					GJServerUserInfoSend();
					ConnectServerInfoSend();
					break;
				case WM_TIMER_2000:
					gServerDisplayer.Run();
					gObjCountProc();
					break;
				case WM_TIMER_10000:
					JoinServerReconnect(hWnd,WM_JOIN_SERVER_MSG_PROC);
					DataServerReconnect(hWnd,WM_DATA_SERVER_MSG_PROC);
					break;
			}
			break;
		case WM_JOIN_SERVER_MSG_PROC:
			JoinServerMsgProc(wParam,lParam);
			break;
		case WM_DATA_SERVER_MSG_PROC:
			DataServerMsgProc(wParam,lParam);
			break;
		case WM_CLOSE:
			if(gObjTotalUser != 0)
			{
				MessageBox(hWnd,"Can not close gameserver with connected players,please disconnect them first!","Danger of data loss!",MB_OK | MB_ICONEXCLAMATION);
				break;
			}

			if(MessageBox(hWnd,"Are you sure to terminate GameServer?","Ask terminate server",MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		case WM_PAINT:
		{
			PAINTSTRUCT hPaintStruct;
			HDC hdc = BeginPaint(hWnd,&hPaintStruct);
			HDC hMemDC = CreateCompatibleDC(hdc);
			HBITMAP OldBmp = (HBITMAP)SelectObject(hMemDC,LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP1)));
			BitBlt(hdc,0,0,594,80,hMemDC,0,0,SRCCOPY);
			SelectObject(hMemDC,OldBmp);
			DeleteDC(hMemDC);
			EndPaint(hWnd,&hPaintStruct);
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd,message,wParam,lParam);
	}

	return 0;
}

LRESULT CALLBACK About(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	switch(message)
	{
		case WM_INITDIALOG:
			return 1;
		case WM_COMMAND:
			if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg,LOWORD(wParam));
				return 1;
			}
			break;
	}

	return 0;
}

LRESULT CALLBACK Users(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam) // OK
{
	switch(message)
	{
		case WM_INITDIALOG:
		{
			HWND hwndList = GetDlgItem(hDlg,IDC_LIST1);

			for(int n = OBJECT_START_USER;n < MAX_OBJECT;n++)
			{
				if(gObj[n].Connected >= OBJECT_LOGGED && gObj[n].Type == OBJECT_USER)
				{
					TCHAR buff[64];

					if(gObj[n].Attack.Offline != 0)
					{
						wsprintf(buff,"[%s] [%s] OFFLINE",gObj[n].Account,gObj[n].Name);
					}
					else
					{
						wsprintf(buff,"[%s] [%s]",gObj[n].Account,gObj[n].Name);
					}

					int pos = (int)SendMessage(hwndList,LB_ADDSTRING,0,(LPARAM)buff);

					SendMessage(hwndList,LB_SETITEMDATA,pos,(LPARAM)n);
				}
			}

			SetFocus(hwndList);

			return 1;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDOK:
				case IDCANCEL:
				{
					EndDialog(hDlg,LOWORD(wParam));
					return 1;
				}
				case IDC_LIST1:
				{
					switch(HIWORD(wParam))
					{
						case LBN_SELCHANGE:
						{
							HWND hwndList = GetDlgItem(hDlg,IDC_LIST1);

							int lbItem = (int)SendMessage(hwndList,LB_GETCURSEL,0,0);

							int i = (int)SendMessage(hwndList,LB_GETITEMDATA,lbItem,0);

							SetDlgItemText(hDlg,IDC_EDIT1,gObj[i].Account);

							SetDlgItemText(hDlg,IDC_EDIT2,gObj[i].IpAddr);

							SetDlgItemText(hDlg,IDC_EDIT3,gObj[i].HardwareId);

							if (gObj[i].Name[0] == 0)
							{
								SetDlgItemText(hDlg,IDC_EDIT4,"SELECT CHARACTER");

								SetDlgItemText(hDlg,IDC_EDIT5,"SELECT CHARACTER");
							}
							else
							{
								TCHAR buff[64];

								wsprintf(buff,"(%s) X:%d Y:%d",gMapManager.GetMapName(gObj[i].Map),gObj[i].X,gObj[i].Y);

								SetDlgItemText(hDlg,IDC_EDIT4,gObj[i].Name);

								SetDlgItemText(hDlg,IDC_EDIT5,buff);
							}

							return 1;
						}
					}

					return 0;
				}
				case IDC_BUTTONDC:
				{
					HWND hwndList = GetDlgItem(hDlg,IDC_LIST1);

					int lbItem = (int)SendMessage(hwndList,LB_GETCURSEL,0,0);

					int i = (int)SendMessage(hwndList,LB_GETITEMDATA,lbItem,0);

					gObjUserKill(gObj[i].Index);

					SendMessage(hwndList,LB_DELETESTRING,lbItem,0);

					SetDlgItemText(hDlg,IDC_EDIT1,"");

					SetDlgItemText(hDlg,IDC_EDIT2,"");

					SetDlgItemText(hDlg,IDC_EDIT3,"");

					SetDlgItemText(hDlg,IDC_EDIT4,"");

					SetDlgItemText(hDlg,IDC_EDIT5,"");

					return 1;
				}
				case IDC_BUTTONRD:
				{
					HWND hwndList = GetDlgItem(hDlg,IDC_LIST1);

					SendMessage(hwndList,LB_RESETCONTENT,0,0);

					for(int n = OBJECT_START_USER;n < MAX_OBJECT;n++)
					{
						if (gObj[n].Connected >= OBJECT_LOGGED && gObj[n].Type == OBJECT_USER)
						{
							TCHAR buff[64];

							if(gObj[n].Attack.Offline != 0)
							{
								wsprintf(buff,"[%s] [%s] OFFLINE",gObj[n].Account,gObj[n].Name);
							}
							else
							{
								wsprintf(buff,"[%s] [%s]",gObj[n].Account,gObj[n].Name);
							}

							int pos = (int)SendMessage(hwndList,LB_ADDSTRING,0,(LPARAM)buff);

							SendMessage(hwndList,LB_SETITEMDATA,pos,(LPARAM)n);
						}
					}

					SetDlgItemText(hDlg,IDC_EDIT1,"");

					SetDlgItemText(hDlg,IDC_EDIT2,"");

					SetDlgItemText(hDlg,IDC_EDIT3,"");

					SetDlgItemText(hDlg,IDC_EDIT4,"");

					SetDlgItemText(hDlg,IDC_EDIT5,"");

					return 1;
				}
			}
		}
	}

	return 0;
}

LRESULT CALLBACK MapViewer(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
	static HBITMAP hBitmap;
	static HDC hdcMem;
	static int MapShowGates = 1;
	static int MapShowInvasion = 0;
	static int MapShowMonster = 0;
	static int MapShowNPC = 1;
	static int MapShowPlayer = 1;

	HWND m_hwnd = GetDlgItem(hDlg,IDC_MAP_LIST);

	if(message == WM_INITDIALOG)
	{
		SendMessage(GetDlgItem(hDlg,IDC_MAPGATE_VIEW),BM_SETCHECK,MapShowGates,0);
		
		SendMessage(GetDlgItem(hDlg,IDC_INVASION_VIEW),BM_SETCHECK,MapShowInvasion,0);
		
		SendMessage(GetDlgItem(hDlg,IDC_MONSTER_VIEW),BM_SETCHECK,MapShowMonster,0);
		
		SendMessage(GetDlgItem(hDlg,IDC_NPC_VIEW),BM_SETCHECK,MapShowNPC,0);
		
		SendMessage(GetDlgItem(hDlg,IDC_PLAYER_VIEW),BM_SETCHECK,MapShowPlayer,0);

		SendMessage(m_hwnd,LB_RESETCONTENT,0,0);

		char buff[256];

		for (int i = 0; i < MAX_MAP; i++)
		{
			if(gMapManager.IsValidMap(i) != 0)
			{
				wsprintf(buff,"[%02d] - %s",i,gMapManager.GetMapName(i));

				int item = SendMessage(m_hwnd,LB_ADDSTRING,0,(LPARAM)buff);

				SendMessage(m_hwnd,LB_SETITEMDATA,item,i);
			}
		}

		SetFocus(m_hwnd);

		SetTimer(hDlg,MAP_VIEWER_TIMER,1000,0);

		hBitmap = CreateCompatibleBitmap(GetDC(hDlg),255*3,255*3);

		hdcMem = CreateCompatibleDC(GetDC(hDlg));

		SelectObject(hdcMem,hBitmap);

		return 1;
	}
	else if(message == WM_TIMER && wParam == MAP_VIEWER_TIMER)
	{
		char buff[256];

		SIZE textSize;

		int radius = 3;
		int margim = 12;
		int item = SendMessage(m_hwnd,LB_GETCURSEL,0,0);
		int map = SendMessage(m_hwnd,LB_GETITEMDATA,item,0);

		HBRUSH m_brush[5] = { CreateSolidBrush(RGB(255,255,255)),CreateSolidBrush(RGB(255,0,255)),CreateSolidBrush(RGB(110,240,120)),CreateSolidBrush(RGB(255,80,30)),CreateSolidBrush(RGB(255,255,0)) };

		RECT rect;

		GetClientRect(hDlg,&rect);

		rect.top = margim;
		rect.left = margim;
		rect.bottom = 255*3+margim;
		rect.right = 255*3+margim;

		FillRect(hdcMem,&rect,CreateSolidBrush(RGB(40,40,140)));

		if(gMap[map].m_MapAttr != 0)
		{
			for (int x = 0; x < 255; x++)
			{
				for (int y = 0; y < 255; y++)
				{
					if(gMap[map].CheckAttr(x,y,4) != 0 || gMap[map].CheckAttr(x,y,8) != 0)
					{
						rect.top = margim+x*3;
						rect.left = margim+y*3;
						rect.bottom = rect.top+3;
						rect.right = rect.left+3;

						FillRect(hdcMem,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH));
					}
				}
			}
		}

		for (int n = 0; n < MAX_OBJECT; n++)
		{
			if(gObjIsConnected(n) == 0)
			{
				continue;
			}

			LPOBJ lpTarget = &gObj[n];

			if(lpTarget->Map != map || lpTarget->DieRegen != 0 || lpTarget->Teleport != 0 || lpTarget->Live == 0)
			{
				continue;
			}

			int centerX = margim+lpTarget->Y*3;

			int centerY = margim+lpTarget->X*3;

			if(lpTarget->Type == OBJECT_USER && MapShowPlayer != 0)
			{
				SelectObject(hdcMem,m_brush[0]);
				Ellipse(hdcMem,centerX-radius,centerY-radius,centerX+radius,centerY+radius);

				if(lpTarget->Attack.Offline != 0)
				{
					wsprintf(buff,"%s [%d-%d] OFF",lpTarget->Name,lpTarget->X,lpTarget->Y);
				}
				else
				{
					wsprintf(buff,"%s [%d-%d]",lpTarget->Name,lpTarget->X,lpTarget->Y);
				}

				GetTextExtentPoint32(hdcMem,buff,strlen(buff),&textSize);
				
				SetBkMode(hdcMem,TRANSPARENT);

				SetTextColor(hdcMem,RGB(0,0,0));
				TextOut(hdcMem,(centerX-(radius+1))-textSize.cx/2,centerY+(radius+1),buff,strlen(buff));

				SetTextColor(hdcMem,RGB(255,255,255));
				TextOut(hdcMem,centerX-textSize.cx/2,centerY+radius,buff,strlen(buff));
			}
			else if(lpTarget->Type == OBJECT_NPC && MapShowNPC != 0)
			{
				SelectObject(hdcMem,m_brush[1]);
				Ellipse(hdcMem,centerX-radius,centerY-radius,centerX+radius,centerY+radius);
			}
			else if(lpTarget->Type == OBJECT_MONSTER)
			{
				if(lpTarget->InvasionIndex == -1 && MapShowMonster != 0)
				{
					SelectObject(hdcMem,m_brush[2]);
					Ellipse(hdcMem,centerX-radius,centerY-radius,centerX+radius,centerY+radius);
				}
				else if(lpTarget->InvasionIndex != -1 && MapShowInvasion != 0)
				{
					SelectObject(hdcMem,m_brush[3]);
					Ellipse(hdcMem,centerX-radius,centerY-radius,centerX+radius,centerY+radius);
				}
			}
		}

		if(MapShowGates != 0)
		{
			for(std::map<int,GATE_INFO>::iterator it= gGate.m_GateInfo.begin();it != gGate.m_GateInfo.end();it++)
			{
				if(it->second.Flag != 1 || map != it->second.Map)
				{
					continue;
				}

				rect.top = margim+it->second.X*3;
				rect.left = margim+it->second.Y*3;
				rect.bottom = rect.top+4;
				rect.right = rect.left+4;

				FillRect(hdcMem,&rect,m_brush[4]);
			}
		}

		DeleteObject((HGDIOBJ)m_brush);

		SetBkMode(hdcMem,TRANSPARENT);

		HDC hdc = GetDC(hDlg);

		BitBlt(hdc,margim,margim,255*3,255*3,hdcMem,margim,margim,SRCCOPY);

		ReleaseDC(hDlg,hdc);
	}
	else if(message == WM_COMMAND)
	{
		if(LOWORD(wParam) == IDC_MAPGATE_VIEW)
		{
			MapShowGates ^= 1;
		}
		else if(LOWORD(wParam) == IDC_INVASION_VIEW)
		{
			MapShowInvasion ^= 1;
		}
		else if(LOWORD(wParam) == IDC_MONSTER_VIEW)
		{
			MapShowMonster ^= 1;
		}
		else if(LOWORD(wParam) == IDC_NPC_VIEW)
		{
			MapShowNPC ^= 1;
		}
		else if(LOWORD(wParam) == IDC_PLAYER_VIEW)
		{
			MapShowPlayer ^= 1;
		}
		else if(LOWORD(wParam) == IDCANCEL)
		{
			DeleteObject(hBitmap);
			DeleteDC(hdcMem);
			EndDialog(hDlg,LOWORD(wParam));
			return 1;
		}
	}

	return 0;
}