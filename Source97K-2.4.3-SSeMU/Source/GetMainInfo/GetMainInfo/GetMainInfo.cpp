#include "stdafx.h"
#include "CCRC32.H"
#include "CustomEffect.h"
#include "CustomFog.h"
#include "CustomItem.h"
#include "CustomMap.h"
#include "CustomMessage.h"
#include "CustomMonster.h"
#include "CustomTooltip.h"
#include "EventEntryLevel.h"
#include "SkillAttackSpeed.h"
#include "ThemidaSDK.h"

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

int _tmain(int argc, _TCHAR* argv[]) // OK
{
	CLEAR_START

	ENCODE_START

	printf("=================================================\n");
	printf("===    SSeMU Emulator Powered by SetecSoft    ===\n");
	printf("===          https://www.ssemu.com.ar         ===\n");
	printf("=================================================\n\n");

	MAIN_FILE_INFO info;

	memset(&info,0,sizeof(info));

	GetPrivateProfileString("MainInfo","CustomerName","",info.CustomerName,sizeof(info.CustomerName),".\\MainInfo.ini");
	info.LauncherStart = GetPrivateProfileInt("MainInfo","LauncherStart",0,".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo","LauncherMutex","",info.LauncherMutex,sizeof(info.LauncherMutex),".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo","IpAddress","",info.IpAddress,sizeof(info.IpAddress),".\\MainInfo.ini");
	info.IpAddressPort = GetPrivateProfileInt("MainInfo","IpAddressPort",44405,".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo","ClientVersion","",info.ClientVersion,sizeof(info.ClientVersion),".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo","ClientSerial","",info.ClientSerial,sizeof(info.ClientSerial),".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo","WindowName","",info.WindowName,sizeof(info.WindowName),".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo","ScreenShotPath","",info.ScreenShotPath,sizeof(info.ScreenShotPath),".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo","ClientName","",info.ClientName,sizeof(info.ClientName),".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo","PluginName1","",info.PluginName[0],sizeof(info.PluginName[0]),".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo","PluginName2","",info.PluginName[1],sizeof(info.PluginName[1]),".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo","PluginName3","",info.PluginName[2],sizeof(info.PluginName[2]),".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo","PluginName4","",info.PluginName[3],sizeof(info.PluginName[3]),".\\MainInfo.ini");
	GetPrivateProfileString("MainInfo","PluginName5","",info.PluginName[4],sizeof(info.PluginName[4]),".\\MainInfo.ini");
	info.AttackAnimationValue[0] = GetPrivateProfileInt("AnimationInfo","DWAttackAnimationValue",15,".\\MainInfo.ini");
	info.AttackAnimationValue[1] = GetPrivateProfileInt("AnimationInfo","DKAttackAnimationValue",15,".\\MainInfo.ini");
	info.AttackAnimationValue[2] = GetPrivateProfileInt("AnimationInfo","FEAttackAnimationValue",15,".\\MainInfo.ini");
	info.AttackAnimationValue[3] = GetPrivateProfileInt("AnimationInfo","MGAttackAnimationValue",15,".\\MainInfo.ini");
	info.AttackAnimationValue[4] = GetPrivateProfileInt("AnimationInfo","DLAttackAnimationValue",15,".\\MainInfo.ini");
	info.KeyCodeAutoAttackSwitch = GetPrivateProfileInt("KeyInfo","KeyCodeAutoAttackSwitch",119,".\\MainInfo.ini");
	info.KeyCodeHealthBarSwitch = GetPrivateProfileInt("KeyInfo","KeyCodeHealthBarSwitch",120,".\\MainInfo.ini");
	info.KeyCodeCamera3DSwitch = GetPrivateProfileInt("KeyInfo","KeyCodeCamera3DSwitch",121,".\\MainInfo.ini");
	info.KeyCodeCamera3DRestore = GetPrivateProfileInt("KeyInfo","KeyCodeCamera3DRestore",122,".\\MainInfo.ini");
	info.KeyCodeTrayModeSwitch = GetPrivateProfileInt("KeyInfo","KeyCodeTrayModeSwitch",123,".\\MainInfo.ini");
	info.ReconnectTime = GetPrivateProfileInt("ReconnectInfo","ReconnectTime",300000,".\\MainInfo.ini");
	info.AdvancedOptionMenuSwitch = GetPrivateProfileInt("CustomInfo","AdvancedOptionMenuSwitch",0,".\\MainInfo.ini");
	info.GoldenArcherRenewalSwitch = GetPrivateProfileInt("CustomInfo","GoldenArcherRenewalSwitch",0,".\\MainInfo.ini");
	info.ItemMoveRightClicSwitch = GetPrivateProfileInt("CustomInfo","ItemMoveRightClicSwitch",0,".\\MainInfo.ini");
	info.WindowEventTimeSwitch = GetPrivateProfileInt("CustomInfo","WindowEventTimeSwitch",0,".\\MainInfo.ini");

	if(gCustomEffect.Load(".\\Data\\CustomEffect.txt") == 0) { system("pause"); return 0; }
	if(gCustomFog.Load(".\\Data\\CustomFog.txt") == 0) { system("pause"); return 0; }
	if(gCustomItem.Load(".\\Data\\CustomItem.txt") == 0) { system("pause"); return 0; }
	if(gCustomMap.Load(".\\Data\\CustomMap.txt") == 0) { system("pause"); return 0; }
	if(gCustomMessage.Load(".\\Data\\CustomMessage.txt") == 0) { system("pause"); return 0; }
	if(gCustomMonster.Load(".\\Data\\CustomMonster.txt") == 0) { system("pause"); return 0; }
	if(gCustomTooltip.Load(".\\Data\\CustomTooltip.txt") == 0) { system("pause"); return 0; }
	if(gEventEntryLevel.Load(".\\Data\\EventEntryLevel.txt") == 0) { system("pause"); return 0; }
	if(gSkillAttackSpeed.Load(".\\Data\\SkillAttackSpeed.txt") == 0) { system("pause"); return 0; }

	memcpy(info.m_BloodCastleEntryLevelCommon,gEventEntryLevel.m_BloodCastleEntryLevelCommon,sizeof(info.m_BloodCastleEntryLevelCommon));
	memcpy(info.m_BloodCastleEntryLevelSpecial,gEventEntryLevel.m_BloodCastleEntryLevelSpecial,sizeof(info.m_BloodCastleEntryLevelSpecial));
	memcpy(info.m_DevilSquareEntryLevelCommon,gEventEntryLevel.m_DevilSquareEntryLevelCommon,sizeof(info.m_DevilSquareEntryLevelCommon));
	memcpy(info.m_DevilSquareEntryLevelSpecial,gEventEntryLevel.m_DevilSquareEntryLevelSpecial,sizeof(info.m_DevilSquareEntryLevelSpecial));
	memcpy(info.m_ChaosCastleEntryLevelCommon,gEventEntryLevel.m_ChaosCastleEntryLevelCommon,sizeof(info.m_ChaosCastleEntryLevelCommon));
	memcpy(info.m_ChaosCastleEntryLevelSpecial,gEventEntryLevel.m_ChaosCastleEntryLevelSpecial,sizeof(info.m_ChaosCastleEntryLevelSpecial));

	memcpy(info.CustomEffectInfo,gCustomEffect.m_CustomEffectInfo,sizeof(info.CustomEffectInfo));
	memcpy(info.CustomFogInfo,gCustomFog.m_CustomFogInfo,sizeof(info.CustomFogInfo));
	memcpy(info.CustomItemInfo,gCustomItem.m_CustomItemInfo,sizeof(info.CustomItemInfo));
	memcpy(info.CustomMapInfo,gCustomMap.m_CustomMapInfo,sizeof(info.CustomMapInfo));
	memcpy(info.EngCustomMessageInfo,gCustomMessage.m_EngCustomMessageInfo,sizeof(info.EngCustomMessageInfo));
	memcpy(info.PorCustomMessageInfo,gCustomMessage.m_PorCustomMessageInfo,sizeof(info.PorCustomMessageInfo));
	memcpy(info.SpnCustomMessageInfo,gCustomMessage.m_SpnCustomMessageInfo,sizeof(info.SpnCustomMessageInfo));
	memcpy(info.CustomMonsterInfo,gCustomMonster.m_CustomMonsterInfo,sizeof(info.CustomMonsterInfo));
	memcpy(info.CustomTooltipInfo,gCustomTooltip.m_CustomTooltipInfo,sizeof(info.CustomTooltipInfo));
	memcpy(info.SkillAttackSpeedInfo,gSkillAttackSpeed.m_SkillAttackSpeedInfo,sizeof(info.SkillAttackSpeedInfo));

	CCRC32 CRC32;

	char buff[256];

	wsprintf(buff,".\\Path\\%s",info.ClientName);

	if(CRC32.FileCRC(buff,&info.ClientNameCRC,1024) == 0)
	{
		info.ClientNameCRC = 0;
	}

	for(int i=0;i < 3;i++)
	{
		wsprintf(buff,".\\Path\\%s",info.PluginName[i]);

		if(CRC32.FileCRC(buff,&info.PluginNameCRC[i],1024) == 0)
		{
			info.PluginNameCRC[i] = 0;
		}
	}

	for(int n=0;n < sizeof(MAIN_FILE_INFO);n++)
	{
		/*((BYTE*)&this->m_MainInfo)[n] += (BYTE)(0x95^HIBYTE(n));
		((BYTE*)&this->m_MainInfo)[n] ^= (BYTE)(0xCA^LOBYTE(n));*/
		((BYTE*)&info)[n] ^= (BYTE)(0xCA ^ LOBYTE(n));
		((BYTE*)&info)[n] -= (BYTE)(0x95 ^ HIBYTE(n));
	}

	HANDLE file = CreateFile(".\\Path\\ServerInfo.sse",GENERIC_WRITE,FILE_SHARE_READ,0,CREATE_ALWAYS,FILE_ATTRIBUTE_ARCHIVE,0);

	if(file == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	DWORD OutSize = 0;

	if(WriteFile(file,&info,sizeof(MAIN_FILE_INFO),&OutSize,0) == 0)
	{
		CloseHandle(file);
		return 0;
	}

	CloseHandle(file);

	ENCODE_END

	CLEAR_END

	printf("\n[.\\Path\\ServerInfo.sse] Generated successfully...\n\n");
	system("pause");

	return 0;
}