// LuaASyncQueue.h: interface for the CLuaASyncQueue class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_ASYNC_QUEUE_SIZE 100

struct LUA_ASYNC_QUEUE_INFO
{
	char label[256];
	char query[256];
	char param[256];
};

class CLuaASyncQueue
{
public:
	CLuaASyncQueue();
	virtual ~CLuaASyncQueue();
	void ClearQueue();
	bool AddToQueue(const LUA_ASYNC_QUEUE_INFO& lpInfo);
	bool GetFromQueue(LUA_ASYNC_QUEUE_INFO& lpInfo);
private:
	std::mutex m_mutex;
	std::queue<LUA_ASYNC_QUEUE_INFO> m_QueueInfo;
};