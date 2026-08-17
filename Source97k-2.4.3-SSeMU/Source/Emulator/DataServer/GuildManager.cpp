// GuildManager.cpp: implementation of the CGuildManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildManager.h"
#include "Guild.h"
#include "QueryManager.h"
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

void CGuildManager::Init() // OK
{
	if(gQueryManager.ExecQuery("SELECT * FROM Guild") != 0)
	{
		GUILD_INFO* lpGuild;

		while(gQueryManager.Fetch() != SQL_NO_DATA)
		{
			lpGuild = new GUILD_INFO();

			lpGuild->Index = gQueryManager.GetAsInteger("Number");

			lpGuild->Score = gQueryManager.GetAsInteger("G_Score");

			gQueryManager.GetAsString("G_Name",lpGuild->Name,sizeof(lpGuild->Name));

			gQueryManager.GetAsString("G_Master",lpGuild->Master,sizeof(lpGuild->Master));

			gQueryManager.GetAsString("G_Notice",lpGuild->Notice,sizeof(lpGuild->Notice));

			gQueryManager.GetAsBinary("G_Mark",lpGuild->Mark,sizeof(lpGuild->Mark));

			lpGuild->Member[0].Clear();

			memcpy(lpGuild->Member[0].Name,lpGuild->Master,sizeof(lpGuild->Member[0].Name));

			this->m_GuildList[lpGuild->Index] = lpGuild;
		}
	}

	gQueryManager.Close();

	if(gQueryManager.ExecQuery("SELECT * FROM GuildMember") != 0)
	{
		while(gQueryManager.Fetch() != SQL_NO_DATA)
		{
			char GuildName[9] = {0};

			char MemberName[11] = {0};

			gQueryManager.GetAsString("G_Name",GuildName,sizeof(GuildName));

			gQueryManager.GetAsString("Name",MemberName,sizeof(MemberName));

			GUILD_INFO* lpGuild = this->GetGuild(GuildName);

			if(lpGuild != 0)
			{
				if(strcmp(lpGuild->Master,MemberName) != 0)
				{
					for(int n=1;n < MAX_GUILD_MEMBER;n++)
					{
						if(lpGuild->Member[n].IsEmpty() != 0)
						{
							lpGuild->Member[n].Clear();
							memcpy(lpGuild->Member[n].Name,MemberName,sizeof(lpGuild->Member[0].Name));
							lpGuild->Member[n].Server = -1;
							break;
						}
					}
				}
			}
		}
	}

	gQueryManager.Close();
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
		if(it->second->Name[0] == name[0])
		{
			if(_stricmp(it->second->Name,name) == 0)
			{
				return it->second;
			}
		}
	}

	return 0;
}

GUILD_INFO* CGuildManager::GetGuildMember(char* member) // OK
{
	for(std::map<int,GUILD_INFO*>::iterator it = this->m_GuildList.begin(); it != this->m_GuildList.end(); it++)
	{
		for(int n=0;n < MAX_GUILD_MEMBER;n++)
		{
			if(it->second->Member[n].Name[0] == member[0])
			{
				if(strcmp(it->second->Member[n].Name,member) == 0)
				{
					return it->second;
				}
			}
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
			if(it->second->Member[n].Name[0] == member[0])
			{
				if(strcmp(it->second->Member[n].Name,member) == 0)
				{
					return &it->second->Member[n];
				}
			}
		}
	}

    return 0;
}

BYTE CGuildManager::CreateGuild(char* name,char* master,BYTE* mark) // OK
{
	if(this->GetGuild(name) != 0)
	{
		return 0;
	}

	if(CheckTextSyntax(name,strlen(name)) == 0)
	{
		return 5;
	}

	if(gQueryManager.ExecQuery("WZ_GuildCreate '%s','%s'",name,master) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		return 6;
	}
	else
	{
		if(gQueryManager.GetResult(0) == 0)
		{
			gQueryManager.Close();

			gQueryManager.BindParameterAsBinary(1,mark,32);

			gQueryManager.ExecQuery("UPDATE Guild SET G_Mark=? WHERE G_Name='%s'",name);

			gQueryManager.Close();

			GUILD_INFO* lpGuild = new GUILD_INFO();

			gQueryManager.ExecQuery("SELECT Number FROM Guild WHERE G_Name='%s'",name);

			gQueryManager.Fetch();

			lpGuild->Index = gQueryManager.GetAsInteger("Number");

			gQueryManager.Close();

			memcpy(lpGuild->Name,name,sizeof(lpGuild->Name));

			memcpy(lpGuild->Master,master,sizeof(lpGuild->Master));

			memcpy(lpGuild->Mark,mark,sizeof(lpGuild->Mark));

			memcpy(lpGuild->Member[0].Name,master,sizeof(lpGuild->Member[0].Name));

			this->m_GuildList[lpGuild->Index] = lpGuild;

			return 1;
		}
		else
		{
			gQueryManager.Close();

			return 6;
		}
	}
}

BYTE CGuildManager::RemoveGuild(char* name) // OK
{
	GUILD_INFO* lpGuild = this->GetGuild(name);

	if(lpGuild == 0)
	{
		return 3;
	}

	this->DelGuild(lpGuild->Index);

	gQueryManager.ExecQuery("DELETE FROM Guild WHERE G_Name='%s'",name);

	gQueryManager.Close();

	gQueryManager.ExecQuery("DELETE FROM GuildMember WHERE G_Name='%s'",name);

	gQueryManager.Close();

	return 1;
}

BYTE CGuildManager::AddGuildMember(char* name,char* member,int server) // OK
{
	GUILD_INFO* lpGuild = this->GetGuild(name);

	if(lpGuild == 0)
	{
		return 0;
	}

	if(this->GetMemberInfo(member) != 0)
	{
		return 3;
	}

	if(gQueryManager.ExecQuery("SELECT * FROM GuildMember WHERE Name='%s'",member) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		for(int n=1;n < MAX_GUILD_MEMBER;n++)
		{
			if(lpGuild->Member[n].IsEmpty() != 0)
			{
				lpGuild->Member[n].Clear();
				strcpy_s(lpGuild->Member[n].Name,member);
				lpGuild->Member[n].Server = server;
				
				gQueryManager.ExecQuery("INSERT INTO GuildMember (Name,G_Name) VALUES ('%s','%s')",member,name);

				gQueryManager.Close();

				return 1;
			}
		}

		return 2;
	}
	else
	{
		gQueryManager.Close();

		return 5;
	}
}

BYTE CGuildManager::DelGuildMember(char* member) // OK
{
	GUILD_MEMBER_INFO* lpMember = this->GetMemberInfo(member);

	if(lpMember == 0)
	{
		return 3;
	}

	lpMember->Clear();

	gQueryManager.ExecQuery("DELETE FROM GuildMember WHERE Name='%s'",member);

	gQueryManager.Close();

	return 1;
}

bool CGuildManager::SetGuildScore(char* name,int score) // OK
{
	GUILD_INFO* lpGuild = this->GetGuild(name);

	if(lpGuild == 0)
	{
		return 0;
	}

	if(gQueryManager.ExecQuery("UPDATE Guild SET G_Score=%d WHERE G_Name='%s'",score,name) == 0)
	{
		gQueryManager.Close();

		return 0;
	}
	else
	{
		gQueryManager.Close();

		lpGuild->Score = score;

		return 1;
	}
}

bool CGuildManager::SetGuildNotice(char* name,char* notice) // OK
{
	GUILD_INFO* lpGuild = this->GetGuild(name);

	if(lpGuild == 0)
	{
		return 0;
	}

	gQueryManager.BindParameterAsString(1,notice,60);

	if(gQueryManager.ExecQuery("UPDATE Guild SET G_Notice=? WHERE G_Name='%s'",name) == 0)
	{
		gQueryManager.Close();

		return 0;
	}
	else
	{
		gQueryManager.Close();

		memcpy(lpGuild->Notice,notice,sizeof(lpGuild->Notice));

		return 1;
	}
}