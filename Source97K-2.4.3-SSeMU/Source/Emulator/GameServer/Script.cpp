// Script.cpp: implementation of the CScript class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Script.h"
#include "Log.h"
#include "LuaFunction.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScript::CScript()
{
	this->m_luaState = luaL_newstate();

	luaL_openlibs(this->m_luaState);
	lua_gc(this->m_luaState,LUA_GCCOLLECT,0);

	InitLuaFunction(this->m_luaState);
}

CScript::~CScript()
{
	if(this->m_luaState != nullptr)
	{
		lua_close(this->m_luaState);
		this->m_luaState = nullptr;
	}
}

void CScript::Load(char* path)
{
	this->m_critical.lock();

	if(luaL_loadfile(this->m_luaState,path) != 0)
    {
		LogAdd(LOG_RED,"[Script] Could not load '%s'. %s",path,lua_tostring(this->m_luaState,-1));
		gLog.Output(LOG_SCRIPT,"[Script] Could not load '%s'. %s",path,lua_tostring(this->m_luaState,-1));
        lua_pop(this->m_luaState, 1);
		this->m_critical.unlock();
		return;
    }

	if(lua_pcall(this->m_luaState,0,LUA_MULTRET,0) != 0)
    {
		LogAdd(LOG_RED,"[Script] Error in Lua-file. %s",lua_tostring(this->m_luaState,-1));
		gLog.Output(LOG_SCRIPT,"[Script] Error in Lua-file. %s",lua_tostring(this->m_luaState,-1));
        lua_pop(this->m_luaState,1);
        this->m_critical.unlock();
		return;
    }

	this->m_critical.unlock();
}

void CScript::Execute(const std::string& function,const char* format,...)
{
    this->m_critical.lock();

    if(this->m_luaState == nullptr)
    {
        LOG_SCRIPT_ERROR("[Script] Lua state is nullptr, cannot execute function `%s`", function.c_str());
        return;
    }

    va_list args;
    va_start(args,format);

    lua_getglobal(this->m_luaState,function.c_str());

    int inCount = 0;

    while(*format && *format != '>')
    {
        switch(*format++)
        {
            case 'i': {
                int val = va_arg(args,int);
                lua_pushinteger(this->m_luaState,val);
                inCount++;
                break;
            }
            case 's': {
                const char* val = va_arg(args,const char*);
                lua_pushstring(this->m_luaState,val);
                inCount++;
                break;
            }
            default:
                LOG_SCRIPT_ERROR("[Script] Unknown format specifier in input for function `%s`: '%c'",function.c_str(),*(format-1));
                va_end(args);
                this->m_critical.unlock();
                return;
        }
    }

    if(*format == '>') format++;

    int outCount = strlen(format);

    if(lua_pcall(this->m_luaState,inCount,outCount,0) != 0)
    {
        LOG_SCRIPT_ERROR("[Script] Error running Lua function `%s`: %s",function.c_str(),lua_tostring(this->m_luaState,-1));
        lua_pop(this->m_luaState,1);
        va_end(args);
        this->m_critical.unlock();
        return;
    }

    int retIndex = -outCount;
    while(*format)
    {
        switch(*format++)
        {
            case 'i': 
            {
                if(!lua_isnumber(this->m_luaState,retIndex))
                {
                    LOG_SCRIPT_ERROR("[Script] Return value type mismatch for function `%s`: expected int",function.c_str());
                    lua_pop(this->m_luaState,outCount);
                    va_end(args);
                    this->m_critical.unlock();
                    return;
                }
                int* retVal = va_arg(args,int*);
                *retVal = (int)lua_tointeger(this->m_luaState,retIndex);
                break;
            }
            case 's': 
            {
                if(!lua_isstring(this->m_luaState,retIndex))
                {
                    LOG_SCRIPT_ERROR("[Script] Return value type mismatch for function `%s`: expected string",function.c_str());
                    lua_pop(this->m_luaState,outCount);
                    va_end(args);
                    this->m_critical.unlock();
                    return;
                }
                const char** retVal = va_arg(args,const char**);
                *retVal = lua_tostring(this->m_luaState,retIndex);
                break;
            }
            default:
                LOG_SCRIPT_ERROR("[Script] Unknown format specifier in output for function `%s`: '%c'",function.c_str(),*(format-1));
                lua_pop(this->m_luaState,outCount);
                va_end(args);
                this->m_critical.unlock();
                return;
        }
        retIndex++;
    }

    if(outCount > 0)
    {
        lua_pop(this->m_luaState,outCount);
    }

    va_end(args);
    this->m_critical.unlock();
}
