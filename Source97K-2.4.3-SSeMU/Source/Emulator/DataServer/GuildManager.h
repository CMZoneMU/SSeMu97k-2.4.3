// GuildManager.h: interface for the CGuildManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_GUILD_MEMBER 40

struct GUILD_MEMBER_INFO
{
	void Clear()
	{
		memset(this->Name,0,sizeof(this->Name));

		this->Server = -1;
	}

	bool IsEmpty()
	{
		return Name[0] == '\0';
	}

	char Name[11];
	WORD Server;
};

struct GUILD_INFO
{
	GUILD_INFO()
	{
		this->Clear();
	}

	void Clear()
	{
		this->Index = 0;

		this->Score = 0;

		memset(this->Name,0,sizeof(this->Name));

		memset(this->Master,0,sizeof(this->Master));

		memset(this->Notice,0,sizeof(this->Notice));

		memset(this->Mark,0,sizeof(this->Mark));

		for(int n=0;n < MAX_GUILD_MEMBER;n++){this->Member[n].Clear();}
	}

	DWORD Index;
	DWORD Score;
	char Name[9];
	char Master[11];
	char Notice[60];
	BYTE Mark[32];
	GUILD_MEMBER_INFO Member[MAX_GUILD_MEMBER];
};

class CGuildManager
{
public:
	CGuildManager();
	virtual ~CGuildManager();
	void Init();
	void DelGuild(int index);
	GUILD_INFO* GetGuild(int index);
	GUILD_INFO* GetGuild(char* name);
	GUILD_INFO* GetGuildMember(char* member);
	GUILD_MEMBER_INFO* GetMemberInfo(char* member);
	BYTE CreateGuild(char* name,char* master,BYTE* mark);
	BYTE RemoveGuild(char* name);
	BYTE AddGuildMember(char* name,char* member,int server);
	BYTE DelGuildMember(char* member);
	bool SetGuildScore(char* name,int score);
	bool SetGuildNotice(char* name,char* notice);
private:
	std::map<int,GUILD_INFO*> m_GuildList;
};

extern CGuildManager gGuildManager;