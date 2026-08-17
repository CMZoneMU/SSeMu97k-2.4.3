#pragma once

#include "Protocol.h"

struct PMSG_GOLDEN_ARCHER_COUNT_RECV
{
	PBMSG_HEAD header; // C1:94
	BYTE type;
	DWORD count;
	WORD code[3];
};

struct PMSG_GOLDEN_ARCHER_REWARD_SEND
{
	PBMSG_HEAD header; // C1:96
	BYTE type;
};

void InitGoldenArcher();
void DrawGoldenArcher();
void ControlGoldenArcher();
void GoldenArcherButtonPush();
void GoldenArcherCountRecv(PMSG_GOLDEN_ARCHER_COUNT_RECV* lpMsg);