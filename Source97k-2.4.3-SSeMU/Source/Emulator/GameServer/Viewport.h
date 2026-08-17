// Viewport.h: interface for the CViewport class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "ItemManager.h"
#include "Protocol.h"

enum eViewportState
{
	VIEWPORT_NONE = 0,
	VIEWPORT_SEND = 1,
	VIEWPORT_WAIT = 2,
	VIEWPORT_DESTROY = 3,
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_VIEWPORT_DESTROY_SEND
{
	PBMSG_HEAD header; // C1:14
	BYTE count;
};

struct PMSG_VIEWPORT_DESTROY_ITEM_SEND
{
	PWMSG_HEAD header; // C2:21
	BYTE count;
};

struct PMSG_VIEWPORT_DESTROY
{
	BYTE index[2];
};

struct PMSG_VIEWPORT_SEND
{
	PWMSG_HEAD header; // C2:[12:13:1F:20:45:5A:5B]
	BYTE count;
};

struct PMSG_VIEWPORT_PLAYER
{
	BYTE index[2];
	BYTE x;
	BYTE y;
	#if(GAMESERVER_UPDATE>=99)
	BYTE CharSet[13];
	#else
	BYTE CharSet[11];
	#endif
	WORD ViewSkillState;
	char name[10];
	BYTE tx;
	BYTE ty;
	BYTE DirAndPkLevel;
};

struct PMSG_VIEWPORT_MONSTER
{
	BYTE index[2];
	#if(GAMESERVER_UPDATE>=99)
	BYTE type[2];
	#else
	BYTE type;
	#endif
	WORD ViewSkillState;
	BYTE x;
	BYTE y;
	BYTE tx;
	BYTE ty;
	BYTE DirAndPkLevel;
};

struct PMSG_VIEWPORT_SUMMON
{
	BYTE index[2];
	#if(GAMESERVER_UPDATE>=99)
	BYTE type[2];
	#else
	BYTE type;
	#endif
	WORD ViewSkillState;
	BYTE x;
	BYTE y;
	BYTE tx;
	BYTE ty;
	BYTE DirAndPkLevel;
	char name[10];
};

struct PMSG_VIEWPORT_ITEM
{
	BYTE index[2];
	BYTE x;
	BYTE y;
	BYTE ItemInfo[5];
};

struct PMSG_VIEWPORT_CHANGE
{
	BYTE index[2];
	BYTE x;
	BYTE y;
	#if(GAMESERVER_UPDATE>=99)
	BYTE skin[2];
	#else
	BYTE skin;
	#endif
	WORD ViewSkillState;
	char name[10];
	BYTE tx;
	BYTE ty;
	BYTE DirAndPkLevel;
	#if(GAMESERVER_UPDATE>=99)
	BYTE CharSet[13];
	#else
	BYTE CharSet[11];
	#endif
};

struct PMSG_VIEWPORT_GUILD
{
	BYTE index[2];
	char name[8];
	BYTE Mark[32];
};

struct PMSG_VIEWPORT_GUILD_MEMBER
{
	BYTE index[2];
	BYTE number[2];
};

struct PMSG_GUILD_VIEWPORT_DELETE_SEND
{
	PBMSG_HEAD header; // C1:5D
	BYTE index[2];
};

//**********************************************//
//**********************************************//
//**********************************************//

class CViewport
{
public:
	CViewport();
	virtual ~CViewport();
	bool CheckViewportObjectPosition(int aIndex,int map,int x,int y,int view);
	bool CheckViewportObject1(int aIndex,int bIndex,int type);
	bool CheckViewportObject2(int aIndex,int bIndex,int type);
	bool CheckViewportObjectItem(int aIndex,int bIndex,int type);
	bool AddViewportObject1(int aIndex,int bIndex,int type);
	bool AddViewportObject2(int aIndex,int bIndex,int type);
	bool AddViewportObjectItem(int aIndex,int bIndex,int type);
	void DestroyViewportPlayer1(int aIndex);
	void DestroyViewportPlayer2(int aIndex);
	void DestroyViewportMonster1(int aIndex);
	void DestroyViewportMonster2(int aIndex);
	void DestroyViewportItem(int aIndex);
	void DestroyViewportGuild(int aIndex);
	void CreateViewportPlayer(int aIndex);
	void CreateViewportMonster(int aIndex);
	void CreateViewportItem(int aIndex);
	void GCViewportDestroySend(int aIndex);
	void GCViewportDestroyItemSend(int aIndex);
	void GCViewportPlayerSend(int aIndex);
	void GCViewportMonsterSend(int aIndex);
	void GCViewportSummonSend(int aIndex);
	void GCViewportItemSend(int aIndex);
	void GCViewportChangeSend(int aIndex);
	void GCViewportGuildSend(int aIndex);
	void GCViewportGuildMemberSend(int aIndex);
	void GCViewportSimpleDestroySend(LPOBJ lpObj);
	void GCViewportSimplePlayerSend(LPOBJ lpObj);
	void GCViewportSimpleMonsterSend(LPOBJ lpObj);
	void GCViewportSimpleSummonSend(LPOBJ lpObj);
	void GCViewportSimpleChangeSend(LPOBJ lpObj);
	void GCViewportSimpleGuildSend(LPOBJ lpObj);
	void GCViewportSimpleGuildMemberSend(LPOBJ lpObj);
};

extern CViewport gViewport;