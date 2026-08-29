// ScriptLoader.h: interface for the CScriptLoader class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "CriticalSection.h"
#include "LuaASyncQueue.h"

class CScriptLoader
{
public:
	CScriptLoader();
	virtual ~CScriptLoader();
public:
	void Init();
	void Clean();
	void Load(char* path);
	void LuaError(char* name);
	void AddQueueInfo(LUA_ASYNC_QUEUE_INFO& info);
	void OnReadScript();
	void OnShutScript();
	void OnTimerThread();
	int OnCommandManager(int aIndex,int code,char* arg);
	void OnCommandDone(int aIndex,int code);
	void OnCharacterEntry(int aIndex);
	void OnCharacterClose(int aIndex);
	int OnNpcTalk(int aIndex,int bIndex);
	void OnMonsterDie(int aIndex,int bIndex);
	void OnUserDie(int aIndex,int bIndex);
	void OnUserMove(int aIndex,int MapIndex);
	void OnUserRespawn(int aIndex,int KillerType);
	int OnCheckUserTarget(int aIndex,int bIndex);
	int OnCheckUserKiller(int aIndex,int bIndex);
	int OnUserItemPick(int aIndex,int slot);
	int OnUserItemDrop(int aIndex,int slot,int x,int y);
	int OnUserItemMove(int aIndex,int aFlag,int aSlot,int bFlag,int bSlot);
	static DWORD WINAPI LuaServerQueueThread(CScriptLoader* lpScriptLoader);
private:
	std::mutex m_mutex;
	lua_State* m_luaState;
	CLuaASyncQueue m_LuaServerQueue;
	HANDLE m_LuaServerQueueSemaphore;
	HANDLE m_LuaServerQueueThread;
	std::atomic<bool> m_RunningFlag = true;
};

extern CScriptLoader gScriptLoader;