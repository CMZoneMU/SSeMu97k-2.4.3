#include "stdafx.h"
#include "Effect.h"
#include "Offset.h"
#include "Util.h"

void InitEffect()
{
	SetCompleteHook(0xE9,0x0043BDE0,&RegisterBuff);

	SetCompleteHook(0xE9,0x0043C070,&UnRegisterBuff);
}

void RegisterBuff(eEffectNumber buff,DWORD o) // OK
{
	short Type = *(short*)(o + 2);

	float Position[3],Angle[3],Light[3];

	VectorCopy((float*)o + 16,Position);
	VectorCopy((float*)o + 28,Angle);
	VectorCopy((float*)o + 232,Light);

	if(FindEffect(buff,STATE_POISON) != 0)
	{
		InsertEffect(o,STATE_POISON);
	}

	if(FindEffect(buff,STATE_FREEZE) != 0)
	{
		InsertEffect(o,STATE_FREEZE);
	}

	if(FindEffect(buff,STATE_ATTACK) != 0)
	{
		InsertEffect(o,STATE_ATTACK);
	}

	if(FindEffect(buff,STATE_DEFENSE) != 0)
	{
		InsertEffect(o,STATE_DEFENSE);

		if(Type == MODEL_PLAYER)
		{
			DeleteJoint(MODEL_SPEARSKILL,o,4);

			for(int n = 0; n < 5; n++)
			{
				CreateJoint(MODEL_SPEARSKILL,Position,Position,Angle,4,o,20.0,-1,0);
			}
		}
	}

	if(FindEffect(buff,STATE_ADD_LIFE) != 0)
	{
		InsertEffect(o,STATE_ADD_LIFE);

		if(Type == MODEL_PLAYER)
		{
			DeleteEffect(BITMAP_LIGHT,o,1);
			CreateEffect(BITMAP_LIGHT,Position,Angle,Light,1,o,-1,0,0);
		}
	}

	if(FindEffect(buff,STATE_HARDEN) != 0)
	{
		InsertEffect(o,STATE_HARDEN);

		VectorCopy((float*)o + 28,Angle);

		DeleteEffect(MODEL_ICE,o,1);
		CreateEffect(MODEL_ICE,Position,Angle,Light,1,o,-1,0,0);
			
		Angle[2] += 180.f;
		CreateEffect(MODEL_ICE,Position,Angle,Light,2,o,-1,0,0);

		VectorCopy((float*)o + 28, Angle);
	}

	if(FindEffect(buff,STATE_REDUCE_DEFENSE) != 0)
	{
		InsertEffect(o,STATE_REDUCE_DEFENSE);

		Vector(1.f,1.f,1.f,Light);

		DeleteEffect(BITMAP_SKULL,o,0);
		CreateEffect(BITMAP_SKULL,Position,Angle,Light,0,o,-1,0,0);

		pPlayBuffer(104,o,0);
	}

	if(FindEffect(buff,STATE_REDUCE_ATTACKDAMAGE) != 0)
	{
		InsertEffect(o,STATE_REDUCE_ATTACKDAMAGE);
	}

	if(FindEffect(buff,STATE_REDUCE_MAGICDEFENSE) != 0) 
	{
		InsertEffect(o,STATE_REDUCE_MAGICDEFENSE);

		if(Type == MODEL_PLAYER)
		{
			pPlayBuffer(103,0,0);
			DeleteJoint(MODEL_SPEARSKILL,o,0);

			for(int n = 0; n < 5; n++)
			{
				CreateJoint(MODEL_SPEARSKILL,Position,Position,Angle,0,o,50.0,-1,0);
			}
		}
	}
}

void UnRegisterBuff(eEffectNumber buff,DWORD o) // OK
{
	WORD Type = *(WORD*)(o + 2);

	if(FindEffect(buff,STATE_POISON) != 0)
	{
		RemoveEffect(o,STATE_POISON);
	}

	if(FindEffect(buff,STATE_FREEZE) != 0)
	{
		RemoveEffect(o,STATE_FREEZE);
	}

	if(FindEffect(buff,STATE_ATTACK) != 0)
	{
		RemoveEffect(o,STATE_ATTACK);
	}

	if(FindEffect(buff,STATE_DEFENSE) != 0)
	{
		RemoveEffect(o,STATE_DEFENSE);
		
		DeleteJoint(MODEL_SPEARSKILL,o,4);
	}

	if(FindEffect(buff,STATE_ADD_LIFE) != 0)
	{
		RemoveEffect(o,STATE_ADD_LIFE);
		
		DeleteEffect(BITMAP_LIGHT,o,1);
	}

	if(FindEffect(buff,STATE_HARDEN) != 0)
	{
		RemoveEffect(o,STATE_HARDEN);
	}

	if(FindEffect(buff,STATE_REDUCE_DEFENSE) != 0)
	{
		RemoveEffect(o,STATE_REDUCE_DEFENSE);
		
		DeleteEffect(BITMAP_SKULL,o,0);
	}

	if(FindEffect(buff,STATE_REDUCE_ATTACKDAMAGE) != 0)
	{
		RemoveEffect(o,STATE_REDUCE_ATTACKDAMAGE);
	}

	if(FindEffect(buff,STATE_REDUCE_MAGICDEFENSE) != 0) 
	{
		RemoveEffect(o,STATE_REDUCE_MAGICDEFENSE);
	
		if(Type == MODEL_PLAYER)
		{
			DeleteJoint(MODEL_SPEARSKILL,o,0);
		}
	}
}

void InsertEffect(DWORD o,eEffectNumber buff) // OK
{
	*(DWORD *)(o + 120) |= buff;
}

void RemoveEffect(DWORD o,eEffectNumber buff) // OK
{
	*(DWORD *)(o + 120) &= ~buff;
}