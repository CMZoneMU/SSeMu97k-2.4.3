#pragma once

#include "Protocol.h"

struct PMSG_DURATION_SKILL_ATTACK_RECV
{
	PBMSG_HEAD header; // C3:1E
	BYTE skill;
	BYTE index[2];
	BYTE x;
	BYTE y;
	BYTE dir;
};

struct PMSG_SKILL_ATTACK_SEND
{
	PBMSG_HEAD header; // C3:19
	BYTE skill;
	BYTE index[2];
};

struct PMSG_DURATION_SKILL_ATTACK_SEND
{
	PBMSG_HEAD header; // C3:1E
	BYTE skill;
	BYTE x;
	BYTE y;
	BYTE dir;
	BYTE dis;
	BYTE angle;
	BYTE index[2];
};

void InitSkill();
void SkillCheckIndex1();
void SkillCheckIndex2();
void SkillCheckIndex3();
void SkillCheckIndex4();
void SkillCheckIndex5();
void SkillCheckIndex6();
void SkillCheckIndex7();
void SkillCheckIndex8();
void SkillCheckIndex9();
void SkillCheckIndex10();
void SkillCheckIndex11();
void SkillCheckIndex12();
void SkillCheckIndex13();
void CGSkillAttackSend(int skill,int index);
void CGDurationSkillAttackSend(int skill,int x,int y,int dir,int dis,int angle,int index);
void GCDurationSkillAttackRecv(PMSG_DURATION_SKILL_ATTACK_RECV* lpMsg);
BYTE GetDestValue(int x,int y,int tx,int ty);