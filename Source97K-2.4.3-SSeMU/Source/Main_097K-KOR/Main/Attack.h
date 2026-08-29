#pragma once

#include "Protocol.h"

struct PMSG_CUSTOM_ATTACK_STATUS_RECV
{
	PSBMSG_HEAD header; // C1:F3:EB
	int Started;
};

struct PMSG_CUSTOM_ATTACK_TOGGLE_SEND
{
	PSBMSG_HEAD header;// C1:F3:0A
	int index;
};

void AutoAttackToggle();
void AttackDrawInterface();
void GCCustomAttackStatusRecv(PMSG_CUSTOM_ATTACK_STATUS_RECV* lpMsg);

extern int CustomAttack;