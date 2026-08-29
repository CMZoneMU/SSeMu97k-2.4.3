#pragma once

#include "EffectManager.h"
#include "Item.h"
#include "Protocol.h"
#include "SkillManager.h"
#include "User.h"

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_CHARACTER_LIST_RECV
{
	PWMSG_HEAD header; // C2:01
	WORD index;
	char account[11];
	BYTE MoveCnt;
	BYTE ExtClass;
	BYTE count;
};

struct SDHP_CHARACTER_LIST
{
	BYTE slot;
	char name[11];
	WORD level;
	BYTE Class;
	BYTE CtlCode;
	BYTE Inventory[60];
};

struct SDHP_CHARACTER_CREATE_RECV
{
	PBMSG_HEAD header; // C1:02
	WORD index;
	char account[11];
	char name[11];
	BYTE result;
	BYTE slot;
	BYTE Class;
	BYTE equipment[24];
	WORD level;
};

struct SDHP_CHARACTER_DELETE_RECV
{
	PBMSG_HEAD header; // C1:03
	WORD index;
	char account[11];
	BYTE result;
};

struct SDHP_CHARACTER_INFO_RECV
{
	PWMSG_HEAD header; // C2:04
	WORD index;
	char account[11];
	char name[11];
	BYTE result;
	BYTE Class;
	WORD Level;
	DWORD LevelUpPoint;
	DWORD Experience;
	DWORD Money;
	DWORD Strength;
	DWORD Dexterity;
	DWORD Vitality;
	DWORD Energy;
	DWORD Life;
	DWORD MaxLife;
	DWORD Mana;
	DWORD MaxMana;
	DWORD BP;
	DWORD MaxBP;
	BYTE Inventory[INVENTORY_SIZE][16];
	BYTE Skill[MAX_SKILL_LIST][3];
	BYTE Map;
	BYTE X;
	BYTE Y;
	BYTE Dir;
	DWORD PKCount;
	DWORD PKLevel;
	DWORD PKTime;
	BYTE CtlCode;
	BYTE Quest[50];
	DWORD ChatLimitTime;
	WORD FruitPoint;
	BYTE Effect[MAX_EFFECT_LIST][13];
	DWORD Reset;
	DWORD MasterReset;
	DWORD IsNewChar;
	WORD Married;
	char MarryName[11];
	WORD BCCount;
	WORD CCCount;
	WORD DSCount;
};

struct SDHP_CREATE_ITEM_RECV
{
	PBMSG_HEAD header; // C1:07
	WORD index;
	char account[11];
	BYTE X;
	BYTE Y;
	BYTE Map;
	DWORD Serial;
	WORD ItemIndex;
	BYTE Level;
	BYTE Dur;
	BYTE Option1;
	BYTE Option2;
	BYTE Option3;
	BYTE NewOption;
	WORD LootIndex;
	BYTE SetOption;
	DWORD Duration;
};

struct SDHP_OPTION_DATA_RECV
{
	PBMSG_HEAD header; // C1:08
	WORD index;
	char account[11];
	char name[11];
	BYTE SkillKey[10];
	BYTE GameOption;
	BYTE QKey;
	BYTE WKey;
	BYTE EKey;
	BYTE ChatWindow;
};

struct SDHP_PET_ITEM_INFO_RECV
{
	PWMSG_HEAD header; // C2:09
	WORD index;
	char account[11];
	BYTE type;
	BYTE count;
};

struct SDHP_PET_ITEM_INFO1
{
	BYTE slot;
	DWORD serial;
	BYTE level;
	DWORD experience;
};

struct SDHP_CHARACTER_NAME_CHECK_RECV
{
	PBMSG_HEAD header; // C1:0A
	WORD index;
	char account[11];
	char name[11];
	BYTE result;
};

struct SDHP_CHARACTER_NAME_CHANGE_RECV
{
	PBMSG_HEAD header; // C1:0B
	WORD index;
	char account[11];
	char OldName[11];
	char NewName[11];
	BYTE result;
};

struct SDHP_GLOBAL_POST_RECV
{
	PBMSG_HEAD header; // C1:20
	BYTE type;
	BYTE code;
	char name[11];
	char message[60];
};

struct SDHP_GLOBAL_NOTICE_RECV
{
	PBMSG_HEAD header; // C1:21
	BYTE type;
	BYTE count;
	BYTE opacity;
	WORD delay;
	DWORD color;
	BYTE speed;
	char message[128];
};

struct SDHP_SNS_DATA_RECV
{
	PWMSG_HEAD header; // C1:24
	WORD index;
	char account[11];
	char name[11];
	BYTE result;
	BYTE data[256];
};

struct SDHP_GLOBAL_WHISPER_RECV
{
	PBMSG_HEAD header; // C1:72
	WORD index;
	char account[11];
	char name[11];
	BYTE result;
	char TargetName[11];
	char message[60];
};

struct SDHP_GLOBAL_WHISPER_ECHO_RECV
{
	PBMSG_HEAD header; // C1:73
	WORD index;
	char account[11];
	char name[11];
	char SourceName[11];
	char message[60];
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_DATA_SERVER_INFO_SEND
{
	PBMSG_HEAD header; // C1:00
	char ServerName[32];
	WORD ServerPort;
	WORD ServerCode;
};

struct SDHP_CHARACTER_LIST_SEND
{
	PBMSG_HEAD header; // C1:01
	WORD index;
	char account[11];
};

struct SDHP_CHARACTER_CREATE_SEND
{
	PBMSG_HEAD header; // C1:02
	WORD index;
	char account[11];
	char name[11];
	BYTE Class;
};

struct SDHP_CHARACTER_DELETE_SEND
{
	PBMSG_HEAD header; // C1:03
	WORD index;
	char account[11];
	char name[11];
	BYTE guild;
	char GuildName[9];
};

struct SDHP_CHARACTER_INFO_SEND
{
	PBMSG_HEAD header; // C1:04
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_CREATE_ITEM_SEND
{
	PBMSG_HEAD header; // C1:07
	WORD index;
	char account[11];
	BYTE X;
	BYTE Y;
	BYTE Map;
	WORD ItemIndex;
	BYTE Level;
	BYTE Dur;
	BYTE Option1;
	BYTE Option2;
	BYTE Option3;
	BYTE NewOption;
	WORD LootIndex;
	BYTE SetOption;
	DWORD Duration;
};

struct SDHP_OPTION_DATA_SEND
{
	PBMSG_HEAD header; // C1:08
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_PET_ITEM_INFO_SEND
{
	PWMSG_HEAD header; // C2:09
	WORD index;
	char account[11];
	BYTE type;
	BYTE count;
};

struct SDHP_PET_ITEM_INFO2
{
	BYTE slot;
	DWORD serial;
};

struct SDHP_GLOBAL_POST_SEND
{
	PBMSG_HEAD header; // C1:20
	BYTE type;
	BYTE code;
	char name[11];
	char message[60];
};

struct SDHP_GLOBAL_NOTICE_SEND
{
	PBMSG_HEAD header; // C1:21
	BYTE type;
	BYTE count;
	BYTE opacity;
	WORD delay;
	DWORD color;
	BYTE speed;
	char message[128];
};

struct SDHP_SNS_DATA_SEND
{
	PBMSG_HEAD header; // C1:24
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_CHARACTER_INFO_SAVE_SEND
{
	PWMSG_HEAD header; // C2:30
	WORD index;
	char account[11];
	char name[11];
	WORD Level;
	BYTE Class;
	DWORD LevelUpPoint;
	DWORD Experience;
	DWORD Money;
	DWORD Strength;
	DWORD Dexterity;
	DWORD Vitality;
	DWORD Energy;
	DWORD Life;
	DWORD MaxLife;
	DWORD Mana;
	DWORD MaxMana;
	DWORD BP;
	DWORD MaxBP;
	BYTE Inventory[INVENTORY_SIZE][16];
	BYTE Skill[MAX_SKILL_LIST][3];
	BYTE Map;
	BYTE X;
	BYTE Y;
	BYTE Dir;
	DWORD PKCount;
	DWORD PKLevel;
	DWORD PKTime;
	BYTE Quest[50];
	DWORD ChatLimitTime;
	WORD FruitPoint;
	BYTE Effect[MAX_EFFECT_LIST][13];
	WORD Married;
	char MarryName[11];
	WORD BCCount;
	WORD CCCount;
	WORD DSCount;
};

struct SDHP_INVENTORY_ITEM_SAVE_SEND
{
	PWMSG_HEAD header; // C2:31
	WORD index;
	char account[11];
	char name[11];
	BYTE Inventory[INVENTORY_SIZE][16];
};

struct SDHP_OPTION_DATA_SAVE_SEND
{
	PBMSG_HEAD header; // C1:33
	WORD index;
	char account[11];
	char name[11];
	BYTE SkillKey[10];
	BYTE GameOption;
	BYTE QKey;
	BYTE WKey;
	BYTE EKey;
	BYTE ChatWindow;
};

struct SDHP_PET_ITEM_INFO_SAVE_SEND
{
	PWMSG_HEAD header; // C2:34
	WORD index;
	char account[11];
	BYTE count;
};

struct SDHP_PET_ITEM_INFO_SAVE
{
	DWORD serial;
	BYTE level;
	DWORD experience;
};

struct SDHP_RESET_INFO_SAVE_SEND
{
	PBMSG_HEAD header; // C1:39
	WORD index;
	char account[11];
	char name[11];
	UINT Reset;
	UINT ResetDay;
	UINT ResetWek;
	UINT ResetMon;
};

struct SDHP_MASTER_RESET_INFO_SAVE_SEND
{
	PBMSG_HEAD header; // C1:3A
	WORD index;
	char account[11];
	char name[11];
	UINT Reset;
	UINT MasterReset;
	UINT MasterResetDay;
	UINT MasterResetWek;
	UINT MasterResetMon;
};

struct SDHP_RANKING_DUEL_SAVE_SEND
{
	PBMSG_HEAD header; // C1:3C
	WORD index;
	char account[11];
	char name[11];
	DWORD WinScore;
	DWORD LoseScore;
};

struct SDHP_RANKING_BLOOD_CASTLE_SAVE_SEND
{
	PBMSG_HEAD header; // C1:3D
	WORD index;
	char account[11];
	char name[11];
	DWORD score;
};

struct SDHP_RANKING_CHAOS_CASTLE_SAVE_SEND
{
	PBMSG_HEAD header; // C1:3E
	WORD index;
	char account[11];
	char name[11];
	DWORD score;
};

struct SDHP_RANKING_DEVIL_SQUARE_SAVE_SEND
{
	PBMSG_HEAD header; // C1:3F
	WORD index;
	char account[11];
	char name[11];
	DWORD score;
};

struct SDHP_RANKING_ILLUSION_TEMPLE_SAVE_SEND
{
	PBMSG_HEAD header; // C1:40
	WORD index;
	char account[11];
	char name[11];
	DWORD score;
};

struct SDHP_CREATION_CARD_SAVE_SEND
{
	PBMSG_HEAD header; // C1:42
	WORD index;
	char account[11];
	BYTE ExtClass;
};

struct SDHP_CONNECT_CHARACTER_SEND
{
	PBMSG_HEAD header; // C1:70
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_DISCONNECT_CHARACTER_SEND
{
	PBMSG_HEAD header; // C1:71
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_GLOBAL_WHISPER_SEND
{
	PBMSG_HEAD header; // C1:72
	WORD index;
	char account[11];
	char name[11];
	char TargetName[11];
	char message[60];
};

//**********************************************//
//**********************************************//
//**********************************************//

void DataServerProtocolCore(BYTE head,BYTE* lpMsg,int size);
void DGCharacterListRecv(SDHP_CHARACTER_LIST_RECV* lpMsg);
void DGCharacterCreateRecv(SDHP_CHARACTER_CREATE_RECV* lpMsg);
void DGCharacterDeleteRecv(SDHP_CHARACTER_DELETE_RECV* lpMsg);
void DGCharacterInfoRecv(SDHP_CHARACTER_INFO_RECV* lpMsg);
void DGCreateItemRecv(SDHP_CREATE_ITEM_RECV* lpMsg);
void DGPetItemInfoRecv(SDHP_PET_ITEM_INFO_RECV* lpMsg);
void DGOptionDataRecv(SDHP_OPTION_DATA_RECV* lpMsg);
void DGGlobalPostRecv(SDHP_GLOBAL_POST_RECV* lpMsg);
void DGGlobalNoticeRecv(SDHP_GLOBAL_NOTICE_RECV* lpMsg);
void DGGlobalWhisperRecv(SDHP_GLOBAL_WHISPER_RECV* lpMsg);
void DGGlobalWhisperEchoRecv(SDHP_GLOBAL_WHISPER_ECHO_RECV* lpMsg);
void GDServerInfoSend();
void GDCharacterListSend(int aIndex);
void GDCharacterCreateSend(int aIndex,char* name,BYTE Class);
void GDCharacterDeleteSend(int aIndex,char* name);
void GDCharacterInfoSend(int aIndex,char* name);
void GDCreateItemSend(int aIndex,BYTE map,BYTE x,BYTE y,int index,BYTE level,BYTE dur,BYTE Option1,BYTE Option2,BYTE Option3,int LootIndex,BYTE NewOption,BYTE SetOption,DWORD duration);
void GDOptionDataSend(int aIndex);
void GDPetItemInfoSend(int aIndex,int type);
void GDGlobalPostSend(BYTE type,char* name,char* message);
void GDGlobalNoticeSend(BYTE type,BYTE count,BYTE opacity,WORD delay,DWORD color,BYTE speed,char* message,...);
void GDCharacterInfoSaveSend(int aIndex);
void GDInventoryItemSaveSend(int aIndex);
void GDOptionDataSaveSend(int aIndex,BYTE* SkillKey,BYTE GameOption,BYTE QKey,BYTE WKey,BYTE EKey,BYTE ChatWindow);
void GDPetItemInfoSaveSend(int aIndex,int type);
void GDResetInfoSaveSend(int aIndex,int ResetDay,int ResetWek,int ResetMon);
void GDMasterResetInfoSaveSend(int aIndex,int MasterResetDay,int MasterResetWek,int MasterResetMon);
void GDRankingDuelSaveSend(int aIndex,DWORD WinScore,DWORD LoseScore);
void GDRankingBloodCastleSaveSend(int aIndex,DWORD Score);
void GDRankingChaosCastleSaveSend(int aIndex,DWORD Score);
void GDRankingDevilSquareSaveSend(int aIndex,DWORD Score);
void GDConnectCharacterSend(int aIndex);
void GDDisconnectCharacterSend(int aIndex);
void GDGlobalWhisperSend(int aIndex,char* TargetName,char* message);