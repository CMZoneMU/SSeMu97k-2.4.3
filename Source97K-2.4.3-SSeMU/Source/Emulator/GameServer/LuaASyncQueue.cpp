// LuaASyncQueue.cpp: implementation of the CLuaASyncQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LuaASyncQueue.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLuaASyncQueue::CLuaASyncQueue()
{

}

CLuaASyncQueue::~CLuaASyncQueue()
{
	this->ClearQueue();
}

void CLuaASyncQueue::ClearQueue()
{
	std::lock_guard<std::mutex> lock(this->m_mutex);

	std::queue<LUA_ASYNC_QUEUE_INFO>().swap(this->m_QueueInfo);
}

bool CLuaASyncQueue::AddToQueue(const LUA_ASYNC_QUEUE_INFO& lpInfo)
{
	std::lock_guard<std::mutex> lock(this->m_mutex);

	if(this->m_QueueInfo.size() < MAX_ASYNC_QUEUE_SIZE)
	{
		this->m_QueueInfo.push(lpInfo);

		return true;
	}

	return false;
}

bool CLuaASyncQueue::GetFromQueue(LUA_ASYNC_QUEUE_INFO& lpInfo)
{
	std::lock_guard<std::mutex> lock(this->m_mutex);

	if(!this->m_QueueInfo.empty())
	{
		lpInfo = this->m_QueueInfo.front();

		this->m_QueueInfo.pop();

		return true;
	}

	return false;
}
