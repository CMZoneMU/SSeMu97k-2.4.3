#include "stdafx.h"
#include "Skill.h"
#include "Effect.h"
#include "Offset.h"
#include "Protocol.h"
#include "Util.h"

void InitSkill() // OK
{
	SetCompleteHook(0xE9,0x00486136,&SkillCheckIndex1); // DeathStab
	
	SetCompleteHook(0xE9,0x00489769,&SkillCheckIndex2); // Blast

	SetCompleteHook(0xE9,0x0048C6F9,&SkillCheckIndex3); // TripleShot

	SetCompleteHook(0xE9,0x0049DBB1,&SkillCheckIndex4); // Penetration

	SetCompleteHook(0xE9,0x004A0153,&SkillCheckIndex5); // TwistingSlash

	SetCompleteHook(0xE9,0x004A132A,&SkillCheckIndex6); // FireSlash

	SetCompleteHook(0xE9,0x004A25C6,&SkillCheckIndex7); // PowerSlash

	SetCompleteHook(0xE9,0x004A45D4,&SkillCheckIndex8); // RagefulBlow

	SetCompleteHook(0xE9,0x004A6D45,&SkillCheckIndex9); // HellFire

	SetCompleteHook(0xE9,0x004A78BC,&SkillCheckIndex10); // Inferno

	SetCompleteHook(0xE9,0x004A886B,&SkillCheckIndex11); // TwisterEvilSpirit

	SetCompleteHook(0xE9,0x004A9425,&SkillCheckIndex12); // AquaBeam

	SetCompleteHook(0xE9,0x004AA038,&SkillCheckIndex13); // Flame
}

void __declspec(naked) SkillCheckIndex1() // OK
{
	static DWORD SkillCheckIndexAddress1 = 0x00486B85;
	static int index;

	_asm
	{
		Mov Dword Ptr Ds:[0x05826D10],0x2B
		Pushad
	}

	CGSkillAttackSend(CurrentSkill,*(short*)(CharactersClient+(MovementSkillTarget*916)+476));

	_asm
	{
		Popad
		Jmp[SkillCheckIndexAddress1]
	}
}

void __declspec(naked) SkillCheckIndex2() // OK
{
	static DWORD SkillCheckIndexAddress1 = 0x0048A11D;
	static DWORD c;
	static int x,y,dir,index;

	_asm
	{
		Mov Eax,Dword Ptr Ds:[Esp+0x0D64]
		Mov c,Eax
		Mov Dword Ptr Ds:[0x05826D10],Edx
		Pushad
	}

	x = (int)(*(float*)(c+788)/100.0f);

	y = (int)(*(float*)(c+792)/100.0f);

	dir = (int)(*(float*)(c+36)/360.0f*256.0f);

	if(CheckAttack())
	{
		index = *(short*)(CharactersClient+(SelectedCharacter*916)+476);
	}
	else
	{
		index = -1;
	}

	CGDurationSkillAttackSend(CurrentSkill,x,y,dir,0,0,index);

	_asm
	{
		Popad
		Jmp[SkillCheckIndexAddress1]
	}
}

void __declspec(naked) SkillCheckIndex3() // OK
{
	static DWORD SkillCheckIndexAddress1 = 0x0048D305;
	static DWORD c;
	static int x,y,dir,dis,angle,index;

	_asm
	{
		Mov Eax,Dword Ptr Ds:[Esp+0x1B64]
		Mov c,Eax
		Mov Eax,Dword Ptr Ds:[Esp+0x1B68]
		Mov Ecx,Dword Ptr Ds:[Esp+0x18]
		Xor Edx,Edx
		Mov Dl,Byte Ptr Ds:[Eax+Ecx+0x25]
		Mov Dword Ptr Ds:[0x05826D10],Edx
		Pushad
	}

	((void(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+904);

	x = (BYTE)(*(DWORD*)(c+904));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+904);

	((UINT(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+908);

	y = (BYTE)(*(DWORD*)(c+908));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+908);

	dir = (int)(*(float*)(c+36)/360.0f*256.0f);

	dis = GetDestValue(x,y,TargetX,TargetY);

	angle = (int)((*(float*)(c+36)+180.0f)/360.0f*256.0f);

	if(CheckAttack())
	{
		index = *(short*)(CharactersClient+(SelectedCharacter*916)+476);
	}
	else
	{
		index = -1;
	}

	CGDurationSkillAttackSend(CurrentSkill,x,y,dir,dis,angle,index);

	_asm
	{
		Popad
		Jmp[SkillCheckIndexAddress1]
	}
}

void __declspec(naked) SkillCheckIndex4() // OK
{
	static DWORD SkillCheckIndexAddress1 = 0x0049E5FD;
	static DWORD c;
	static int x,y,dir,index;

	_asm
	{
		Mov Edi,Dword Ptr Ds:[Esp+0x139F4]
		Mov c,Edi
		Mov Dword Ptr Ds:[0x05826D10],0x34
		Pushad
	}

	((void(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+904);

	x = (BYTE)(*(DWORD*)(c+904));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+904);

	((UINT(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+908);

	y = (BYTE)(*(DWORD*)(c+908));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+908);

	dir = (int)(*(float*)(c+36)/360.0f*256.0f);

	if(CheckAttack())
	{
		index = *(short*)(CharactersClient+(SelectedCharacter*916)+476);
	}

	CGDurationSkillAttackSend(CurrentSkill,x,y,dir,0,0,index);

	_asm
	{
		Popad
		Jmp[SkillCheckIndexAddress1]
	}
}

void __declspec(naked) SkillCheckIndex5() // OK
{
	static DWORD SkillCheckIndexAddress1 = 0x004A0AE2;
	static DWORD c;
	static int x,y,dir,index;

	_asm
	{
		Mov Eax,Dword Ptr Ds:[Esp+0x139F4]
		Mov c,Eax
		Mov Eax,Dword Ptr Ds:[Esp+0x40]
		Mov Dword Ptr Ds:[0x05826D10],Eax
		Pushad
	}

	((void(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+904);

	x = (BYTE)(*(DWORD*)(c+904));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+904);

	((UINT(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+908);

	y = (BYTE)(*(DWORD*)(c+908));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+908);

	dir = (int)(*(float*)(c+36)/360.0f*256.0f);

	if(CheckAttack())
	{
		index = *(short*)(CharactersClient+(SelectedCharacter*916)+476);
	}
	else
	{
		index = -1;
	}

	CGDurationSkillAttackSend(CurrentSkill,x,y,dir,0,0,index);

	_asm
	{
		Popad
		Jmp[SkillCheckIndexAddress1]
	}
}

void __declspec(naked) SkillCheckIndex6() // OK
{
	static DWORD SkillCheckIndexAddress1 = 0x004A1D5B;
	static DWORD c;
	static int x,y,dir,dis,index;

	_asm
	{
		Mov Edx,Dword Ptr Ds:[Esp+0x139F4]
		Mov c,Edx
		Mov Edx,Dword Ptr Ds:[Esp+0x40]
		Mov Dword Ptr Ds:[0x05826D10],Edx
		Pushad
	}

	((void(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+904);

	x = (BYTE)(*(DWORD*)(c+904));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+904);

	((UINT(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+908);

	y = (BYTE)(*(DWORD*)(c+908));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+908);

	dir = (int)(*(float*)(c+36)/360.0f*256.0f);

	dis = GetDestValue(x,y,TargetX,TargetY);

	if(CheckAttack())
	{
		index = *(short*)(CharactersClient+(SelectedCharacter*916)+476);
	}
	else
	{
		index = -1;
	}

	CGDurationSkillAttackSend(CurrentSkill,x,y,dir,dis,0,index);

	_asm
	{
		Popad
		Jmp[SkillCheckIndexAddress1]
	}
}

void __declspec(naked) SkillCheckIndex7() // OK
{
	static DWORD SkillCheckIndexAddress1 = 0x004A2FF9;
	static DWORD c;
	static int x,y,dis,dir,angle,index;

	_asm
	{
		Mov Edx,Dword Ptr Ds:[Esp+0x139F4]
		Mov c,Edx
		Mov Edx,Dword Ptr Ds:[Esp+0x40]
		Mov Dword Ptr Ds:[0x05826D10],Edx
		Pushad
	}

	((void(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+904);

	x = (BYTE)(*(DWORD*)(c+904));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+904);

	((UINT(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+908);

	y = (BYTE)(*(DWORD*)(c+908));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+908);

	dis = GetDestValue(x,y,TargetX,TargetY);

	dir = (int)(*(float*)(c+36)/360.0f*256.0f);

	angle = (int)((*(float*)(c+36)+180.0f)/360.0f*256.0f);

	if(CheckAttack())
	{
		index = *(short*)(CharactersClient+(SelectedCharacter*916)+476);
	}
	else
	{
		index = -1;
	}

	CGDurationSkillAttackSend(CurrentSkill,x,y,dir,dis,angle,index);

	_asm
	{
		Popad
		Jmp[SkillCheckIndexAddress1]
	}
}

void __declspec(naked) SkillCheckIndex8() // OK
{
	static DWORD SkillCheckIndexAddress1 = 0x004A508E;
	static DWORD c;
	static int x,y,dir,index;

	_asm
	{
		Mov Eax,Dword Ptr Ds:[Esp+0x139F4]
		Mov c,Eax
		Mov Eax,Dword Ptr Ds:[Esp+0x40]
		Mov Dword Ptr Ds:[0x05826D10],Eax
		Pushad
	}

	((void(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+904);

	x = (BYTE)(*(DWORD*)(c+904));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+904);

	((UINT(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+908);

	y = (BYTE)(*(DWORD*)(c+908));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+908);

	dir = (int)(*(float*)(c+36)/360.0f*256.0f);

	if(CheckAttack())
	{
		index = *(short*)(CharactersClient+(SelectedCharacter*916)+476);
	}
	else
	{
		index = -1;
	}

	CGDurationSkillAttackSend(CurrentSkill,x,y,dir,0,0,index);

	_asm
	{
		Popad
		Jmp[SkillCheckIndexAddress1]
	}
}

void __declspec(naked) SkillCheckIndex9() // OK
{
	static DWORD SkillCheckIndexAddress1 = 0x004A7813;
	static DWORD c;
	static int x,y,dir,index;

	_asm
	{
		Mov Eax,Dword Ptr Ds:[Esp+0x139F4]
		Mov c,Eax
		Mov Eax,Dword Ptr Ds:[Esp+0x40]
		Mov Dword Ptr Ds:[0x05826D10],Eax
		Pushad
	}

	((void(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+904);

	x = (BYTE)(*(DWORD*)(c+904));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+904);

	((UINT(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+908);

	y = (BYTE)(*(DWORD*)(c+908));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+908);

	dir = (int)(*(float*)(c+36)/360.0f*256.0f);

	if(CheckAttack())
	{
		index = *(short*)(CharactersClient+(SelectedCharacter*916)+476);
	}
	else
	{
		index = -1;
	}

	CGDurationSkillAttackSend(CurrentSkill,x,y,dir,0,0,index);

	_asm
	{
		Popad
		Jmp[SkillCheckIndexAddress1]
	}
}

void __declspec(naked) SkillCheckIndex10() // OK
{
	static DWORD SkillCheckIndexAddress1 = 0x004A83CC;
	static DWORD c;
	static int x,y,dir,index;

	_asm
	{
		Mov Eax,Dword Ptr Ds:[Esp+0x139F4]
		Mov c,Eax
		Mov Eax,Dword Ptr Ds:[Esp+0x40]
		Mov Dword Ptr Ds:[0x05826D10],Eax
		Pushad
	}

	((void(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+904);

	x = (BYTE)(*(DWORD*)(c+904));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+904);

	((UINT(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+908);

	y = (BYTE)(*(DWORD*)(c+908));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+908);

	dir = (int)(*(float*)(c+36)/360.0f*256.0f);

	if(CheckAttack())
	{
		index = *(short*)(CharactersClient+(SelectedCharacter*916)+476);
	}
	else
	{
		index = -1;
	}

	CGDurationSkillAttackSend(CurrentSkill,x,y,dir,0,0,index);

	_asm
	{
		Popad
		Jmp[SkillCheckIndexAddress1]
	}
}

void __declspec(naked) SkillCheckIndex11() // OK
{
	static DWORD SkillCheckIndexAddress1 = 0x004A9383;
	static DWORD c;
	static int x,y,dir,index;

	_asm
	{
		Mov Eax,Dword Ptr Ds:[Esp+0x139F4]
		Mov c,Eax
		Mov Eax,Dword Ptr Ds:[Esp+0x40]
		Mov Dword Ptr Ds:[0x05826D10],Eax
		Pushad
	}

	((void(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+904);

	x = (BYTE)(*(DWORD*)(c+904));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+904);

	((UINT(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+908);

	y = (BYTE)(*(DWORD*)(c+908));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+908);

	dir = (int)(*(float*)(c+36)/360.0f*256.0f);

	if(CheckAttack())
	{
		index = *(short*)(CharactersClient+(SelectedCharacter*916)+476);
	}
	else
	{
		index = -1;
	}

	CGDurationSkillAttackSend(CurrentSkill,x,y,dir,0,0,index);

	_asm
	{
		Popad
		Jmp[SkillCheckIndexAddress1]
	}
}

void __declspec(naked) SkillCheckIndex12() // OK
{
	static DWORD SkillCheckIndexAddress1 = 0x004A9F91;
	static DWORD c;
	static int x,y,dir,index;

	_asm
	{
		Mov Eax,Dword Ptr Ds:[Esp+0x139F4]
		Mov c,Eax
		Mov Eax,Dword Ptr Ds:[Esp+0x40]
		Mov Dword Ptr Ds:[0x05826D10],Eax
		Pushad
	}

	((void(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+904);

	x = (BYTE)(*(DWORD*)(c+904));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+904);

	((UINT(__thiscall*)(DWORD,DWORD))0x0043D3E0)(0x055C9BC8,c+908);

	y = (BYTE)(*(DWORD*)(c+908));

	((void(__thiscall*)(DWORD,DWORD))0x004233E0)(0x055C9BC8,c+908);

	dir = (int)(*(float*)(c+36)/360.0f*256.0f);

	if(CheckAttack())
	{
		index = *(short*)(CharactersClient+(SelectedCharacter*916)+476);
	}
	else
	{
		index = -1;
	}

	CGDurationSkillAttackSend(CurrentSkill,x,y,dir,0,0,index);

	_asm
	{
		Popad
		Jmp[SkillCheckIndexAddress1]
	}
}

void __declspec(naked) SkillCheckIndex13() // OK
{
	static DWORD SkillCheckIndexAddress1 = 0x004AAADC;
	static DWORD c;
	static int x,y,dir,index;

	_asm
	{
		Mov Edx,Dword Ptr Ds:[Esp+0x139F4]
		Mov c,Edx
		Mov Edx,Dword Ptr Ds:[Esp+0x40]
		Mov Dword Ptr Ds:[0x05826D10],Edx
		Pushad
	}

	x = (int)(*(float*)(c+788)/100.0f);

	y = (int)(*(float*)(c+792)/100.0f);

	dir = (int)(*(float*)(c+36)/360.0f*256.0f);

	if(CheckAttack())
	{
		index = *(short*)(CharactersClient+(SelectedCharacter*916)+476);
	}
	else
	{
		index = -1;
	}

	CGDurationSkillAttackSend(CurrentSkill,x,y,dir,0,0,index);

	_asm
	{
		Popad
		Jmp[SkillCheckIndexAddress1]
	}
}

void CGSkillAttackSend(int skill,int index) // OK
{
	PMSG_SKILL_ATTACK_SEND pMsg;

	pMsg.header.setE(0x19,sizeof(pMsg));

	pMsg.skill = skill;

	pMsg.index[0] = SET_NUMBERHB(index);

	pMsg.index[1] = SET_NUMBERLB(index);

	DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CGDurationSkillAttackSend(int skill,int x,int y,int dir,int dis,int angle,int index) // OK
{
	PMSG_DURATION_SKILL_ATTACK_SEND pMsg;

	pMsg.header.setE(0x1E,sizeof(pMsg));

	pMsg.skill = skill;
	
	pMsg.x = x;

	pMsg.y = y;

	pMsg.dir = dir;

	pMsg.dis = dis;

	pMsg.angle = angle;

	pMsg.index[0] = SET_NUMBERHB(index);

	pMsg.index[1] = SET_NUMBERLB(index);

	DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void GCDurationSkillAttackRecv(PMSG_DURATION_SKILL_ATTACK_RECV* lpMsg) // OK
{
	int aIndex = FindCharacterIndex(MAKE_NUMBERW(lpMsg->index[0], lpMsg->index[1]));

	if (aIndex >= 0 && aIndex < MAX_MAIN_VIEWPORT)
	{
		DWORD c = CharactersClient + (aIndex * 916);

		*(float *)(c + 0x24) = (lpMsg->dir / 256.0f) * 360.0f;
	}
}

BYTE GetDestValue(int x,int y,int tx,int ty) // OK
{
	int DestX = tx-x;
	int DestY = ty-y;

	if(DestX < -8)
	{
		DestX = -8;
	}

	if(DestX > 7)
	{
		DestX = 7;
	}

	if(DestY < -8)
	{
		DestY = -8;
	}

	if(DestY > 7)
	{
		DestY = 7;
	}

	BYTE byValue1 = ((BYTE)(DestX+8))<<4;

	BYTE byValue2 = ((BYTE)(DestY+8))&0xF;

	return (byValue1|byValue2);
}