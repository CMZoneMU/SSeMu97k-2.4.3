// Protect.h: interface for the CProtect class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "CustomEffect.h"
#include "CustomFog.h"
#include "CustomItem.h"
#include "CustomMap.h"
#include "CustomMessage.h"
#include "CustomMonster.h"
#include "CustomTooltip.h"
#include "SkillAttackSpeed.h"

struct MAIN_FILE_INFO
{
	char CustomerName[32];
	BYTE LauncherStart;
	char LauncherMutex[32];
	char IpAddress[32];
	WORD IpAddressPort;
	char ClientVersion[8];
	char ClientSerial[17];
	char WindowName[32];
	char ScreenShotPath[50];
	char ClientName[32];
	DWORD ClientNameCRC;
	char PluginName[5][32];
	DWORD PluginNameCRC[5];
	BYTE AttackAnimationValue[5];
	BYTE KeyCodeAutoAttackSwitch;
	BYTE KeyCodeHealthBarSwitch;
	BYTE KeyCodeCamera3DSwitch;
	BYTE KeyCodeCamera3DRestore;
	BYTE KeyCodeTrayModeSwitch;
	DWORD ReconnectTime;
	BYTE AdvancedOptionMenuSwitch;
	BYTE GoldenArcherRenewalSwitch;
	BYTE ItemMoveRightClicSwitch;
	BYTE WindowEventTimeSwitch;
	int m_BloodCastleEntryLevelCommon[7][2];
	int m_BloodCastleEntryLevelSpecial[7][2];
	int m_DevilSquareEntryLevelCommon[4][2];
	int m_DevilSquareEntryLevelSpecial[4][2];
	int m_ChaosCastleEntryLevelCommon[6][2];
	int m_ChaosCastleEntryLevelSpecial[6][2];
	CUSTOM_EFFECT_INFO CustomEffectInfo[MAX_CUSTOM_EFFECT];
	CUSTOM_FOG_INFO CustomFogInfo[MAX_CUSTOM_FOG];
	CUSTOM_ITEM_INFO CustomItemInfo[MAX_CUSTOM_ITEM];
	CUSTOM_MAP_INFO CustomMapInfo[MAX_CUSTOM_MAP];
	CUSTOM_MESSAGE_INFO EngCustomMessageInfo[MAX_CUSTOM_MESSAGE];
	CUSTOM_MESSAGE_INFO PorCustomMessageInfo[MAX_CUSTOM_MESSAGE];
	CUSTOM_MESSAGE_INFO SpnCustomMessageInfo[MAX_CUSTOM_MESSAGE];
	CUSTOM_MONSTER_INFO CustomMonsterInfo[MAX_CUSTOM_MONSTER];
	CUSTOM_TOOLTIP_INFO CustomTooltipInfo[MAX_CUSTOM_TOOLTIP];
	SKILL_ATTACK_SPEED_INFO SkillAttackSpeedInfo[MAX_ATTACK_SPEED];
};

class CProtect
{
public:
	CProtect();
	virtual ~CProtect();
	bool ReadMainFile(char* name);
	void CheckLauncher();
	void CheckInstance();
	void CheckClientFile();
	void CheckPluginFile();
public:
	MAIN_FILE_INFO m_MainInfo;
	DWORD m_ClientFileCRC;
};

extern CProtect gProtect;