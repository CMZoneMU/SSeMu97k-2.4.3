// Script.h: interface for the CScript class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "CriticalSection.h"

#define LOG_SCRIPT_ERROR(fmt, ...) \
    LogAdd(LOG_RED,fmt,__VA_ARGS__); \
    gLog.Output(LOG_SCRIPT,fmt,__VA_ARGS__)

class CScript
{
public:
	CScript();
	virtual ~CScript();
	void Load(char* path);
	void Execute(const std::string& function, const char* format, ...);
private:
	lua_State* m_luaState;
	CCriticalSection m_critical;
};
