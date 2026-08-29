#pragma once

#define Vector(a,b,c,d) {(d)[0]=a;(d)[1]=b;(d)[2]=c;}
#define VectorCopy(a,b) {(b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];}

#define FindEffect(x,y) ((((x)&(y))==(y))?1:0)

#define DeleteEffect ((bool(__cdecl*)(int,DWORD,int))0x00460D20)
#define CreateJoint ((void(__cdecl*)(int,float*,float*,float*,int,DWORD,float,short,WORD))0x0046D840)
#define DeleteJoint ((void(__cdecl*)(int,DWORD,int))0x0046FE00)

#define CreateEffect ((void(__cdecl*)(int,float*,float*,float*,int,DWORD,short,BYTE,float))0x00460DC0)
#define pPlayBuffer ((int(__cdecl*)(int,int,int))0x00404BC0)

enum eModelList
{
	MODEL_ICE = 190,
	MODEL_SPEARSKILL = 266,
	MODEL_PLAYER = 390,
};

enum eBitmapList
{
	BITMAP_LIGHT = 1150,
	BITMAP_SKULL = 1274,
	BITMAP_SHINY = 1230,
};

enum eEffectNumber
{
	STATE_POISON = 0x1,
	STATE_FREEZE = 0x2,
	STATE_ATTACK = 0x4,
	STATE_DEFENSE = 0x8,
	STATE_ADD_LIFE = 0x10,
	STATE_HARDEN = 0x20,
	STATE_REDUCE_DEFENSE = 0x40,
	STATE_REDUCE_ATTACKDAMAGE = 0x80,
	STATE_REDUCE_MAGICDEFENSE = 0x100,
};

void InitEffect();
void RegisterBuff(eEffectNumber buff,DWORD o);
void UnRegisterBuff(eEffectNumber buff,DWORD o);
void InsertEffect(DWORD o,eEffectNumber buff);
void RemoveEffect(DWORD o,eEffectNumber buff);