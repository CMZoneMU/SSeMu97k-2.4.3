// NpcTalk.h: interface for the CNpcTalk class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"
#include "User.h"

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_NPC_TALK_RECV
{
	PBMSG_HEAD header; // C1:30
	BYTE index[2];
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_NPC_TALK_SEND
{
	PBMSG_HEAD header; // C3:30
	BYTE result;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CNpcTalk
{
public:
	CNpcTalk();
	virtual ~CNpcTalk();
	bool NpcTalk(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcTrainer(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcAngelKing(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcAngelMessenger(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcServerTransfer(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcGoldenArcher(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcCharon(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcChaosGoblin(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcWarehouse(LPOBJ lpNpc,LPOBJ lpObj);
	void NpcGuildMaster(LPOBJ lpNpc,LPOBJ lpObj);
	void CGNpcTalkRecv(PMSG_NPC_TALK_RECV* lpMsg,int aIndex);
	void CGNpcTalkCloseRecv(int aIndex);
};

extern CNpcTalk gNpcTalk;
