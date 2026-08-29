// ScriptLoader.cpp: implementation of the CScriptLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScriptLoader.h"
#include "LuaFunction.h"
#include "Log.h"
#include "QueryManager.h"
#include "ServerInfo.h"
#include "Util.h"

CScriptLoader gScriptLoader;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScriptLoader::CScriptLoader()
{
	this->m_luaState = nullptr;

	this->m_LuaServerQueueSemaphore = 0;

	this->m_LuaServerQueueThread = 0;
}

CScriptLoader::~CScriptLoader()
{
	this->Clean();
}

void CScriptLoader::Init()
{
	if((this->m_LuaServerQueueSemaphore=CreateSemaphore(0,0,MAX_ASYNC_QUEUE_SIZE,0)) == 0)
	{
		LogAdd(LOG_RED,"[ScriptLoader] CreateSemaphore() failed with error: %d",GetLastError());
		return;
	}

	if((this->m_LuaServerQueueThread=CreateThread(0,0,(LPTHREAD_START_ROUTINE)this->LuaServerQueueThread,this,0,0)) == 0)
	{
		LogAdd(LOG_RED,"[ScriptLoader] CreateThread() failed with error: %d",GetLastError());
		return;
	}

	this->m_RunningFlag = true;

	LogAdd(LOG_BLACK,"[ScriptLoader] Lua async system started");
}

void CScriptLoader::Clean()
{
	this->m_RunningFlag = false;

	this->OnShutScript();

	if(this->m_luaState != nullptr)
    {
        lua_close(this->m_luaState);
        this->m_luaState = nullptr;
    }

	if(this->m_LuaServerQueueThread != 0) 
	{
		ReleaseSemaphore(this->m_LuaServerQueueSemaphore,1,0);
		WaitForSingleObject(this->m_LuaServerQueueThread,INFINITE);
		CloseHandle(this->m_LuaServerQueueThread);
		this->m_LuaServerQueueThread = 0;
	}

	if(this->m_LuaServerQueueSemaphore != 0)
	{
		CloseHandle(this->m_LuaServerQueueSemaphore);
		this->m_LuaServerQueueSemaphore = 0;
	}

	this->m_LuaServerQueue.ClearQueue();
}

void CScriptLoader::Load(char* path)
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState != nullptr) 
	{
		lua_close(this->m_luaState);
		this->m_luaState = nullptr;
	}

	lua_State* lua = luaL_newstate();

	luaL_openlibs(lua);
	lua_pushcclosure(lua,LuaRequire,0);
	lua_setglobal(lua,"require");
	lua_gc(lua,LUA_GCCOLLECT,0);

	InitLuaFunction(lua);

	if(luaL_loadfile(lua,path) != 0)
    {
		LogAdd(LOG_RED,"[ScriptLoader] Could not load '%s'. %s",path,lua_tostring(lua,-1));
		gLog.Output(LOG_SCRIPT,"Error: %s",lua_tostring(lua,-1));
		lua_close(lua);
        return;
    }

	if(lua_pcall(lua,0,0,0) != 0)
    {
		LogAdd(LOG_RED,"[ScriptLoader] Error in Lua-file. %s",lua_tostring(lua,-1));
		gLog.Output(LOG_SCRIPT,"Error: %s",lua_tostring(lua,-1));
		lua_close(lua);
        return;
    }

	this->m_luaState = lua;
}

void CScriptLoader::LuaError(char* name)
{
	gLog.Output(LOG_SCRIPT,"Error in %s: %s",name,lua_tostring(this->m_luaState,-1));
    
	lua_pop(this->m_luaState,1);
}

void CScriptLoader::AddQueueInfo(LUA_ASYNC_QUEUE_INFO& info)
{
	if(this->m_LuaServerQueue.AddToQueue(info))
	{
		ReleaseSemaphore(this->m_LuaServerQueueSemaphore,1,0);
	}
}

void CScriptLoader::OnReadScript()
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState == nullptr)
	{
		return;
	}

	lua_getglobal(this->m_luaState,"BridgeFunction_OnReadScript");

	if(lua_isfunction(this->m_luaState,-1) == 0)
    {
		this->LuaError("OnReadScript");
		return;
    }

	if(lua_pcall(this->m_luaState,0,0,0) != 0)
	{
		this->LuaError("OnReadScript");
		return;
	}
}

void CScriptLoader::OnShutScript()
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState == nullptr)
	{
		return;
	}

	lua_getglobal(this->m_luaState,"BridgeFunction_OnShutScript");

	if(lua_isfunction(this->m_luaState,-1) == 0)
    {
		this->LuaError("OnShutScript");
		return;
    }

	if(lua_pcall(this->m_luaState,0,0,0) != 0)
	{
		this->LuaError("OnShutScript");
		return;
	}
}

void CScriptLoader::OnTimerThread()
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState == nullptr)
	{
		return;
	}

	lua_getglobal(this->m_luaState,"BridgeFunction_OnTimerThread");

	if(lua_isfunction(this->m_luaState,-1) == 0)
    {
		this->LuaError("OnTimerThread");
		return;
    }

	if(lua_pcall(this->m_luaState,0,0,0) != 0)
	{
		this->LuaError("OnTimerThread");
		return;
	}
}

int CScriptLoader::OnCommandManager(int aIndex,int code,char* arg)
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState == nullptr)
	{
		return 0;
	}

	lua_getglobal(this->m_luaState,"BridgeFunction_OnCommandManager");

	if(lua_isfunction(this->m_luaState,-1) == 0)
    {
		this->LuaError("OnCommandManager");
		return 0;
    }

	lua_pushinteger(this->m_luaState,aIndex);

	lua_pushinteger(this->m_luaState,code);

	lua_pushstring(this->m_luaState,arg);

	if(lua_pcall(this->m_luaState,3,1,0) != 0)
	{
		this->LuaError("OnCommandManager");
		return 0;
	}

	if(lua_isnumber(this->m_luaState,-1) == 0)
	{
		this->LuaError("OnCommandManager");
		return 0;
	}

	int value = lua_tointeger(this->m_luaState,-1);
	
	lua_pop(this->m_luaState,1);

	return value;
}

void CScriptLoader::OnCommandDone(int aIndex,int code)
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState == nullptr)
	{
		return;
	}

	lua_getglobal(this->m_luaState,"BridgeFunction_OnCommandDone");

	if(lua_isfunction(this->m_luaState,-1) == 0)
    {
		this->LuaError("OnCommandDone");
		return;
    }

	lua_pushinteger(this->m_luaState,aIndex);

	lua_pushinteger(this->m_luaState,code);

	if(lua_pcall(this->m_luaState,2,0,0) != 0)
	{
		this->LuaError("OnCommandDone");
		return;
	}
}

void CScriptLoader::OnCharacterEntry(int aIndex)
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState == nullptr)
	{
		return;
	}

	lua_getglobal(this->m_luaState,"BridgeFunction_OnCharacterEntry");

	if(lua_isfunction(this->m_luaState,-1) == 0)
    {
		this->LuaError("OnCharacterEntry");
		return;
    }

	lua_pushinteger(this->m_luaState,aIndex);

	if(lua_pcall(this->m_luaState,1,0,0) != 0)
	{
		this->LuaError("OnCharacterEntry");
		return;
	}
}

void CScriptLoader::OnCharacterClose(int aIndex)
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState == nullptr)
	{
		return;
	}

	lua_getglobal(this->m_luaState,"BridgeFunction_OnCharacterClose");

	if(lua_isfunction(this->m_luaState,-1) == 0)
    {
		this->LuaError("OnCharacterClose");
		return;
    }

	lua_pushinteger(this->m_luaState,aIndex);

	if(lua_pcall(this->m_luaState,1,0,0) != 0)
	{
		this->LuaError("OnCharacterClose");
		return;
	}
}

int CScriptLoader::OnNpcTalk(int aIndex,int bIndex)
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState == nullptr)
	{
		return 0;
	}

	lua_getglobal(this->m_luaState,"BridgeFunction_OnNpcTalk");

	if(lua_isfunction(this->m_luaState,-1) == 0)
    {
		this->LuaError("OnNpcTalk");
		return 0;
    }

	lua_pushinteger(this->m_luaState,aIndex);

	lua_pushinteger(this->m_luaState,bIndex);

	if(lua_pcall(this->m_luaState,2,1,0) != 0)
	{
		this->LuaError("OnNpcTalk");
		return 0;
	}

	if(lua_isnumber(this->m_luaState,-1) == 0)
	{
		this->LuaError("OnNpcTalk");
		return 0;
	}

	int value = lua_tointeger(this->m_luaState,-1);
	
	lua_pop(this->m_luaState,1);

	return value;
}

void CScriptLoader::OnMonsterDie(int aIndex,int bIndex)
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState == nullptr)
	{
		return;
	}

	lua_getglobal(this->m_luaState,"BridgeFunction_OnMonsterDie");

	if(lua_isfunction(this->m_luaState,-1) == 0)
    {
		this->LuaError("OnMonsterDie");
		return;
    }

	lua_pushinteger(this->m_luaState,aIndex);

	lua_pushinteger(this->m_luaState,bIndex);

	if(lua_pcall(this->m_luaState,2,0,0) != 0)
	{
		this->LuaError("OnMonsterDie");
		return;
	}
}

void CScriptLoader::OnUserDie(int aIndex,int bIndex)
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState == nullptr)
	{
		return;
	}

	lua_getglobal(this->m_luaState,"BridgeFunction_OnUserDie");

	if(lua_isfunction(this->m_luaState,-1) == 0)
    {
		this->LuaError("OnUserDie");
		return;
    }

	lua_pushinteger(this->m_luaState,aIndex);

	lua_pushinteger(this->m_luaState,bIndex);

	if(lua_pcall(this->m_luaState,2,0,0) != 0)
	{
		this->LuaError("OnUserDie");
		return;
	}
}

void CScriptLoader::OnUserMove(int aIndex,int MapIndex)
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState == nullptr)
	{
		return;
	}

	lua_getglobal(this->m_luaState,"BridgeFunction_OnUserMove");

	if(lua_isfunction(this->m_luaState,-1) == 0)
    {
		this->LuaError("OnUserMove");
		return;
    }

	lua_pushinteger(this->m_luaState,aIndex);

	lua_pushinteger(this->m_luaState,MapIndex);

	if(lua_pcall(this->m_luaState,2,0,0) != 0)
	{
		this->LuaError("OnUserMove");
		return;
	}
}

void CScriptLoader::OnUserRespawn(int aIndex,int KillerType)
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState == nullptr)
	{
		return;
	}

	lua_getglobal(this->m_luaState,"BridgeFunction_OnUserRespawn");

	if(lua_isfunction(this->m_luaState,-1) == 0)
    {
		this->LuaError("OnUserRespawn");
		return;
    }

	lua_pushinteger(this->m_luaState,aIndex);

	lua_pushinteger(this->m_luaState,KillerType);

	if(lua_pcall(this->m_luaState,2,0,0) != 0)
	{
		this->LuaError("OnUserRespawn");
		return;
	}
}

int CScriptLoader::OnCheckUserTarget(int aIndex,int bIndex)
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState == nullptr)
	{
		return 1;
	}

	lua_getglobal(this->m_luaState,"BridgeFunction_OnCheckUserTarget");

	if(lua_isfunction(this->m_luaState,-1) == 0)
    {
		this->LuaError("OnCheckUserTarget");
		return 1;
    }

	lua_pushinteger(this->m_luaState,aIndex);

	lua_pushinteger(this->m_luaState,bIndex);

	if(lua_pcall(this->m_luaState,2,1,0) != 0)
	{
		this->LuaError("OnCheckUserTarget");
		return 1;
	}

	if(lua_isnumber(this->m_luaState,-1) == 0)
	{
		this->LuaError("OnCheckUserTarget");
		return 1;
	}

	int value = lua_tointeger(this->m_luaState,-1);
	
	lua_pop(this->m_luaState,1);

	return value;
}

int CScriptLoader::OnCheckUserKiller(int aIndex,int bIndex)
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState == nullptr)
	{
		return 1;
	}

	lua_getglobal(this->m_luaState,"BridgeFunction_OnCheckUserKiller");

	if(lua_isfunction(this->m_luaState,-1) == 0)
    {
		this->LuaError("OnCheckUserKiller");
		return 1;
    }

	lua_pushinteger(this->m_luaState,aIndex);

	lua_pushinteger(this->m_luaState,bIndex);

	if(lua_pcall(this->m_luaState,2,1,0) != 0)
	{
		this->LuaError("OnCheckUserKiller");
		return 1;
	}

	if(lua_isnumber(this->m_luaState,-1) == 0)
	{
		this->LuaError("OnCheckUserKiller");
		return 1;
	}

	int value = lua_tointeger(this->m_luaState,-1);
	
	lua_pop(this->m_luaState,1);

	return value;
}

int CScriptLoader::OnUserItemPick(int aIndex,int slot)
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState == nullptr)
	{
		return 1;
	}

	lua_getglobal(this->m_luaState,"BridgeFunction_OnUserItemPick");

	if(lua_isfunction(this->m_luaState,-1) == 0)
    {
		this->LuaError("OnUserItemPick");
		return 1;
    }

	lua_pushinteger(this->m_luaState,aIndex);

	lua_pushinteger(this->m_luaState,slot);

	if(lua_pcall(this->m_luaState,2,1,0) != 0)
	{
		this->LuaError("OnUserItemPick");
		return 1;
	}

	if(lua_isnumber(this->m_luaState,-1) == 0)
	{
		this->LuaError("OnUserItemPick");
		return 1;
	}

	int value = lua_tointeger(this->m_luaState,-1);
	
	lua_pop(this->m_luaState,1);

	return value;
}

int CScriptLoader::OnUserItemDrop(int aIndex,int slot,int x,int y)
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState == nullptr)
	{
		return 1;
	}

	lua_getglobal(this->m_luaState,"BridgeFunction_OnUserItemDrop");

	if(lua_isfunction(this->m_luaState,-1) == 0)
    {
		this->LuaError("OnUserItemDrop");
		return 1;
    }

	lua_pushinteger(this->m_luaState,aIndex);

	lua_pushinteger(this->m_luaState,slot);

	lua_pushinteger(this->m_luaState,x);

	lua_pushinteger(this->m_luaState,y);

	if(lua_pcall(this->m_luaState,4,1,0) != 0)
	{
		this->LuaError("OnUserItemDrop");
		return 1;
	}

	if(lua_isnumber(this->m_luaState,-1) == 0)
	{
		this->LuaError("OnUserItemDrop");
		return 1;
	}

	int value = lua_tointeger(this->m_luaState,-1);
	
	lua_pop(this->m_luaState,1);

	return value;
}

int CScriptLoader::OnUserItemMove(int aIndex,int aFlag,int aSlot,int bFlag,int bSlot)
{
	std::unique_lock<std::mutex> lock(this->m_mutex);

	if(this->m_luaState == nullptr)
	{
		return 1;
	}

	lua_getglobal(this->m_luaState,"BridgeFunction_OnUserItemMove");

	if(lua_isfunction(this->m_luaState,-1) == 0)
    {
		this->LuaError("OnUserItemMove");
		return 1;
    }

	lua_pushinteger(this->m_luaState,aIndex);

	lua_pushinteger(this->m_luaState,aFlag);

	lua_pushinteger(this->m_luaState,aSlot);

	lua_pushinteger(this->m_luaState,bFlag);

	lua_pushinteger(this->m_luaState,bSlot);

	if(lua_pcall(this->m_luaState,5,1,0) != 0)
	{
		this->LuaError("OnUserItemMove");
		return 1;
	}

	if(lua_isnumber(this->m_luaState,-1) == 0)
	{
		this->LuaError("OnUserItemMove");
		return 1;
	}

	int value = lua_tointeger(this->m_luaState,-1);
	
	lua_pop(this->m_luaState,1);

	return value;
}

DWORD WINAPI CScriptLoader::LuaServerQueueThread(CScriptLoader* lpScriptLoader)
{
	while(lpScriptLoader->m_RunningFlag)
	{
		if(WaitForSingleObject(lpScriptLoader->m_LuaServerQueueSemaphore,INFINITE) == WAIT_FAILED)
		{
			LogAdd(LOG_RED,"[SocketTCP] WaitForSingleObject() failed with error: %d",GetLastError());
			break;
		}

		LUA_ASYNC_QUEUE_INFO QueueInfo;

		if(lpScriptLoader->m_LuaServerQueue.GetFromQueue(QueueInfo) != 0)
		{
			if(gQueryManager.GetStatus() == SQL_NULL_HANDLE)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}

			int result = ((!gQueryManager.ExecQuery(QueueInfo.query)|| gQueryManager.Fetch() == SQL_NO_DATA) ? 0 : 1);

			gQueryManager.Close();

			std::unique_lock<std::mutex> lock(lpScriptLoader->m_mutex);

			if(lpScriptLoader->m_luaState == nullptr)
			{
				continue;
			}

			lua_getglobal(lpScriptLoader->m_luaState,"BridgeFunction_OnSQLAsyncResult");

			if(lua_isfunction(lpScriptLoader->m_luaState,-1) == 0)
			{
				lpScriptLoader->LuaError("OnSQLAsyncResult");
				continue;
			}

			lua_pushstring(lpScriptLoader->m_luaState,QueueInfo.label);

			lua_pushstring(lpScriptLoader->m_luaState,QueueInfo.param);

			lua_pushinteger(lpScriptLoader->m_luaState,result);

			if(lua_pcall(lpScriptLoader->m_luaState,3,0,0) != 0)
			{
				lpScriptLoader->LuaError("OnSQLAsyncResult");
				continue;
			}
		}
	}

	return 0;
}
