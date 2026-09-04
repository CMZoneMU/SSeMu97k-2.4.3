// GuildManager.cpp: implementation of the CGuildManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildManager.h"
#include "Guild.h"
#include "Viewport.h"
#include "Util.h"

CGuildManager gGuildManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildManager::CGuildManager() // OK
{
	this->m_GuildList.clear();
}

CGuildManager::~CGuildManager() // OK
{
	for(std::map<int,GUILD_INFO*>::iterator it = this->m_GuildList.begin(); it != this->m_GuildList.end(); ++it)
    {
        delete it->second;
    }

    this->m_GuildList.clear();
}

void CGuildManager::AddGuild(int index,char* name,BYTE* mark,char* master,char* notice,int score) // OK
{
	if(this->GetGuild(name) != 0)
	{
		return;
	}

	GUILD_INFO* lpGuild = new GUILD_INFO();

	lpGuild->Index = index;

	lpGuild->Score = score;

	memcpy(lpGuild->Name,name,sizeof(lpGuild->Name));
	
	memcpy(lpGuild->Master,master,sizeof(lpGuild->Master));

	memcpy(lpGuild->Notice,notice,sizeof(lpGuild->Notice));

	memcpy(lpGuild->Mark,mark,sizeof(lpGuild->Mark));

	memcpy(lpGuild->Member[0].Name,master,sizeof(lpGuild->Member[0].Name));

	this->m_GuildList[lpGuild->Index] = lpGuild;
}

void CGuildManager::DelGuild(int index) // OK
{
	std::map<int,GUILD_INFO*>::iterator it = this->m_GuildList.find(index);

	if(it != this->m_GuildList.end()) 
	{
		delete it->second;
		this->m_GuildList.erase(it);
	}
}

GUILD_INFO* CGuildManager::GetGuild(int index) // OK
{
	std::map<int,GUILD_INFO*>::iterator it = this->m_GuildList.find(index);

	if(it != this->m_GuildList.end()) 
	{
		return it->second;
	}

	return 0;
}

GUILD_INFO* CGuildManager::GetGuild(char* name) // OK
{
	for(std::map<int,GUILD_INFO*>::iterator it = this->m_GuildList.begin(); it != this->m_GuildList.end(); it++)
	{
		if(_stricmp(it->second->Name,name) == 0)
		{
			return it->second;
		}
	}

	return 0;
}

GUILD_MEMBER_INFO* CGuildManager::GetMemberInfo(char* member) // OK
{
	for(std::map<int,GUILD_INFO*>::iterator it = this->m_GuildList.begin(); it != this->m_GuildList.end(); it++)
	{
		for(int n=0;n < MAX_GUILD_MEMBER;n++)
		{
			if(_stricmp(it->second->Member[n].Name,member) == 0)
			{
				return &it->second->Member[n];
			}
		}
	}

    return 0;
}

int CGuildManager::AddMember(char* name,char* member,int index,int server) // OK
{
	GUILD_INFO* pGuild = this->GetGuild(name);

	if(pGuild == 0)
	{
		return 0;
	};

	if(_stricmp(member,pGuild->Master) == 0)
	{
		strcpy_s(pGuild->Member[0].Name,member);
		
		pGuild->Member[0].Server = server;

		if(index != -1)
		{
			pGuild->Member[0].Index = index;
		}

		return 1;
	}

	GUILD_MEMBER_INFO* lpMember = this->GetMemberInfo(member);

	if(lpMember != 0)
	{
		lpMember->Server = server;

		if(index != -1)
		{
			lpMember->Index = index;
		}

		return 1;
	}

	for(int n=1;n < MAX_GUILD_MEMBER;n++)
	{
		if(pGuild->Member[n].IsEmpty() != 0)
		{
			pGuild->Member[n].Clear();

			strcpy_s(pGuild->Member[n].Name,member);
		
			pGuild->Member[n].Server = server;

			if(index != -1)
			{
				pGuild->Member[n].Index = index;
			}

			return 1;
		}
	}

	return 0;
}

void CGuildManager::DelMember(char* member) // OK
{
	GUILD_MEMBER_INFO* lpMember = this->GetMemberInfo(member);

	if(lpMember != 0)
	{
		lpMember->Clear();
	}
}

void CGuildManager::ConnectUser(char* name,char* member,int index,int server) // OK
{
	GUILD_INFO* lpGuild = this->GetGuild(name);

	if(lpGuild == 0)
	{
		return;
	}

	if(_stricmp(member,lpGuild->Master) == 0)
	{
		lpGuild->WarDeclare = 0;
		lpGuild->WarState = 0;
		lpGuild->WarScore = 0;
	}

	GUILD_MEMBER_INFO* lpMember = this->GetMemberInfo(member);

	if(lpMember != 0)
	{
		lpMember->Index = index;
		lpMember->Server = server;
	}
	else
	{
		this->AddMember(name,member,index,server);
	}
}

void CGuildManager::SetServer(char* member,int server) // OK
{
	GUILD_MEMBER_INFO* lpMember = this->GetMemberInfo(member);

	if(lpMember != 0)
	{
		lpMember->Server = server;
	}
}