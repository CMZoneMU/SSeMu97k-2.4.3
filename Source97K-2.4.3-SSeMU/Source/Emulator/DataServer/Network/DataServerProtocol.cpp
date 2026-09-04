#include "stdafx.h"
#include "DataServerProtocol.h"
#include "BadSyntax.h"
#include "CharacterManager.h"
#include "CommandManager.h"
#include "CustomPick.h"
#include "Guild.h"
#include "GuildManager.h"
#include "GoldenArcher.h"
#include "ItemManager.h"
#include "PeriodicItem.h"
#include "QueryManager.h"
#include "Quest.h"
#include "ServerManager.h"
#include "SocketManager.h"
#include "Util.h"
#include "Warehouse.h"

void DataServerProtocolCore(int index,BYTE head,BYTE* lpMsg,int size) // OK
{
	switch(head)
	{
		case 0x00:
			GDServerInfoRecv((SDHP_SERVER_INFO_RECV*)lpMsg,index);
			break;
		case 0x01:
			GDCharacterListRecv((SDHP_CHARACTER_LIST_RECV*)lpMsg,index);
			break;
		case 0x02:
			GDCharacterCreateRecv((SDHP_CHARACTER_CREATE_RECV*)lpMsg,index);
			break;
		case 0x03:
			GDCharacterDeleteRecv((SDHP_CHARACTER_DELETE_RECV*)lpMsg,index);
			break;
		case 0x04:
			GDCharacterInfoRecv((SDHP_CHARACTER_INFO_RECV*)lpMsg,index);
			break;
		case 0x05:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gWarehouse.GDWarehouseItemRecv((SDHP_WAREHOUSE_ITEM_RECV*)lpMsg,index);
					break;
				case 0x30:
					gWarehouse.GDWarehouseItemSaveRecv((SDHP_WAREHOUSE_ITEM_SAVE_RECV*)lpMsg);
					break;
			}
			break;
		case 0x07:
			GDCreateItemRecv((SDHP_CREATE_ITEM_RECV*)lpMsg,index);
			break;
		case 0x08:
			GDOptionDataRecv((SDHP_OPTION_DATA_RECV*)lpMsg,index);
			break;
		case 0x0C:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gQuest.GDQuestKillCountRecv((SDHP_QUEST_KILL_COUNT_RECV*)lpMsg,index);
					break;
				case 0x30:
					gQuest.GDQuestKillCountSaveRecv((SDHP_QUEST_KILL_COUNT_SAVE_RECV*)lpMsg);
					break;
			}
			break;
		case 0x0F:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gCommandManager.GDCommandResetRecv((SDHP_COMMAND_RESET_RECV*)lpMsg,index);
					break;
				case 0x01:
					gCommandManager.GDCommandMasterResetRecv((SDHP_COMMAND_MASTER_RESET_RECV*)lpMsg,index);
					break;
				case 0x02:
					gCommandManager.GDCommandDivorceRecv((SDHP_COMMAND_DIVORCE_RECV*)lpMsg,index);
					break;
				case 0x04:
					gCommandManager.GDCommandRenameRecv((SDHP_COMMAND_RENAME_RECV*)lpMsg,index);
					break;
				case 0x05:
					gCommandManager.GDCommandBanAccountRecv((SDHP_COMMAND_BAN_ACCOUNT_RECV*)lpMsg,index);
					break;
				case 0x06:
					gCommandManager.GDCommandBanCharacterRecv((SDHP_COMMAND_BAN_CHARACTER_RECV*)lpMsg,index);
					break;
			}
			break;
		case 0x13:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gCustomPick.GDCustomPickRecv((SDHP_CUSTOM_PICK_RECV*)lpMsg,index);
					break;
				case 0x30:
					gCustomPick.GDCustomPickSaveRecv((SDHP_CUSTOM_PICK_SAVE_RECV*)lpMsg);
					break;
			}
			break;
		case 0x20:
			GDGlobalPostRecv((SDHP_GLOBAL_POST_RECV*)lpMsg,index);
			break;
		case 0x21:
			GDGlobalNoticeRecv((SDHP_GLOBAL_NOTICE_RECV*)lpMsg,index);
			break;
		case 0x2A:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gPeriodicItem.GDPeriodicItemRecv((SDHP_PERIODIC_ITEM_RECV*)lpMsg,index);
					break;
				case 0x30:
					gPeriodicItem.GDPeriodicItemSaveRecv((SDHP_PERIODIC_ITEM_SAVE_RECV*)lpMsg);
					break;
			}
			break;
		case 0x30:
			GDCharacterInfoSaveRecv((SDHP_CHARACTER_INFO_SAVE_RECV*)lpMsg);
			break;
		case 0x31:
			GDInventoryItemSaveRecv((SDHP_INVENTORY_ITEM_SAVE_RECV*)lpMsg);
			break;
		case 0x33:
			GDOptionDataSaveRecv((SDHP_OPTION_DATA_SAVE_RECV*)lpMsg);
			break;
		case 0x39:
			GDResetInfoSaveRecv((SDHP_RESET_INFO_SAVE_RECV*)lpMsg);
			break;
		case 0x3A:
			GDMasterResetInfoSaveRecv((SDHP_MASTER_RESET_INFO_SAVE_RECV*)lpMsg);
			break;
		case 0x3C:
			GDRankingDuelSaveRecv((SDHP_RANKING_DUEL_SAVE_RECV*)lpMsg);
			break;
		case 0x3D:
			GDRankingBloodCastleSaveRecv((SDHP_RANKING_BLOOD_CASTLE_SAVE_RECV*)lpMsg);
			break;
		case 0x3E:
			GDRankingChaosCastleSaveRecv((SDHP_RANKING_CHAOS_CASTLE_SAVE_RECV*)lpMsg);
			break;
		case 0x3F:
			GDRankingDevilSquareSaveRecv((SDHP_RANKING_DEVIL_SQUARE_SAVE_RECV*)lpMsg);
			break;
		case 0x40:
			GDRankingIllusionTempleSaveRecv((SDHP_RANKING_ILLUSION_TEMPLE_SAVE_RECV*)lpMsg);
			break;
		case 0x42:
			GDCreationCardSaveRecv((SDHP_CREATION_CARD_SAVE_RECV*)lpMsg);
			break;
		case 0x50:
			GDCustomMonsterKilLCountRecv((SDHP_MONSTER_KILL_COUNT_RECV*)lpMsg, index);
			break;
		case 0x70:
			GDConnectCharacterRecv((SDHP_CONNECT_CHARACTER_RECV*)lpMsg,index);
			break;
		case 0x71:
			GDDisconnectCharacterRecv((SDHP_DISCONNECT_CHARACTER_RECV*)lpMsg,index);
			break;
		case 0x72:
			GDGlobalWhisperRecv((SDHP_GLOBAL_WHISPER_RECV*)lpMsg,index);
			break;
		case 0x94:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gGoldenArcher.GDGoldenArcherCountRecv((SDHP_GOLDEN_ARCHER_COUNT_RECV*)lpMsg,index);
					break;
				case 0x01:
					gGoldenArcher.GDGoldenArcherRewardRecv((SDHP_GOLDEN_ARCHER_REWARD_RECV*)lpMsg,index);
					break;
				case 0x30:
					gGoldenArcher.GDGoldenArcherAddCountSaveRecv((SDHP_GOLDEN_ARCHER_ADD_COUNT_SAVE_RECV*)lpMsg);
					break;
			}
			break;
		case 0xA0:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gGuild.GDGuildCreateRecv((SDHP_GUILD_CREATE_RECV*)lpMsg,index);
					break;
				case 0x01:
					gGuild.GDGuildDeleteRecv((SDHP_GUILD_DELETE_RECV*)lpMsg,index);
					break;
				case 0x02:
					gGuild.GDGuildMemberAddRecv((SDHP_GUILD_MEMBER_ADD_RECV*)lpMsg,index);
					break;
				case 0x03:
					gGuild.GDGuildMemberDelRecv((SDHP_GUILD_MEMBER_DEL_RECV*)lpMsg,index);
					break;
				case 0x04:
					gGuild.GDGuildNoticeRecv((SDHP_GUILD_NOTICE_RECV*)lpMsg);
					break;
				case 0x05:
					gGuild.GDGuildPostRecv((SDHP_GUILD_POST_RECV*)lpMsg);
					break;
				case 0x06:
					gGuild.GDGuildScoreRecv((SDHP_GUILD_SCORE_RECV*)lpMsg);
					break;
			}
			break;
	}
}

void GDServerInfoRecv(SDHP_SERVER_INFO_RECV* lpMsg,int index) // OK
{
	gServerManager[index].SetServerInfo(lpMsg->ServerName,lpMsg->ServerPort,lpMsg->ServerCode);
}

void GDCharacterListRecv(SDHP_CHARACTER_LIST_RECV* lpMsg,int index) // OK
{
	BYTE send[2048];

	SDHP_CHARACTER_LIST_SEND pMsg;

	pMsg.header.set(0x01,0);

	int size = sizeof(pMsg);

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	if(gQueryManager.ExecQuery("SELECT Id FROM AccountCharacter WHERE Id='%s'",lpMsg->account) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("INSERT INTO AccountCharacter (Id) VALUES ('%s')",lpMsg->account);
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
	}

	char CharacterName[5][11];

	memset(CharacterName,0,sizeof(CharacterName));

	gQueryManager.ExecQuery("SELECT * FROM AccountCharacter WHERE Id='%s'",lpMsg->account);

	gQueryManager.Fetch();

	pMsg.MoveCnt = (BYTE)gQueryManager.GetAsInteger("MoveCnt");

	pMsg.ExtClass = (BYTE)gQueryManager.GetAsInteger("ExtClass");

	gQueryManager.GetAsString("GameID1",CharacterName[0],sizeof(CharacterName[0]));

	gQueryManager.GetAsString("GameID2",CharacterName[1],sizeof(CharacterName[1]));

	gQueryManager.GetAsString("GameID3",CharacterName[2],sizeof(CharacterName[2]));

	gQueryManager.GetAsString("GameID4",CharacterName[3],sizeof(CharacterName[3]));

	gQueryManager.GetAsString("GameID5",CharacterName[4],sizeof(CharacterName[4]));

	gQueryManager.Close();

	pMsg.count = 0;

	SDHP_CHARACTER_LIST info;

	for(int n=0;n < 5;n++)
	{
		if(CharacterName[n][0] == 0)
		{
			continue;
		}

		if(gQueryManager.ExecQuery("SELECT cLevel,Class,Inventory,CtlCode FROM Character WHERE AccountID='%s' AND Name='%s'",lpMsg->account,CharacterName[n]) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();
		}
		else
		{
			info.slot = n;

			memcpy(info.name,CharacterName[n],sizeof(info.name));

			info.level = (WORD)gQueryManager.GetAsInteger("cLevel");

			info.Class = (BYTE)gQueryManager.GetAsInteger("Class");

			BYTE Inventory[12][16];

			gQueryManager.GetAsBinary("Inventory",Inventory[0],sizeof(Inventory));

			info.CtlCode = (BYTE)gQueryManager.GetAsInteger("CtlCode");

			gQueryManager.Close();

			memset(info.Inventory,0xFF,sizeof(info.Inventory));

			for(int i=0;i < 12;i++)
			{
				if(Inventory[i][0] == 0xFF && (Inventory[i][7] & 0x80) == 0x80 && (Inventory[i][9] & 0xF0) == 0xF0)
				{
					info.Inventory[0+(i*5)] = 0xFF;
					info.Inventory[1+(i*5)] = 0xFF;
					info.Inventory[2+(i*5)] = 0xFF;
					info.Inventory[3+(i*5)] = 0xFF;
					info.Inventory[4+(i*5)] = 0xFF;
				}
				else
				{
					info.Inventory[0+(i*5)] = Inventory[i][0];
					info.Inventory[1+(i*5)] = Inventory[i][1];
					info.Inventory[2+(i*5)] = Inventory[i][7];
					info.Inventory[3+(i*5)] = Inventory[i][8];
					info.Inventory[4+(i*5)] = Inventory[i][9];
				}
			}

			memcpy(&send[size],&info,sizeof(info));
			size += sizeof(info);

			pMsg.count++;
		}
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));

	gSocketManager.DataSend(index,send,size);
}

void GDCharacterCreateRecv(SDHP_CHARACTER_CREATE_RECV* lpMsg,int index) // OK
{
	SDHP_CHARACTER_CREATE_SEND pMsg;

	pMsg.header.set(0x02,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	if(CheckTextSyntax(lpMsg->name,sizeof(lpMsg->name)) == 0 || gBadSyntax.CheckSyntax(lpMsg->name) == 0)
	{
		pMsg.result = 0;
	}
	else
	{
		pMsg.result = 1;
	}

	pMsg.slot = 0;

	pMsg.Class = lpMsg->Class;

	memset(pMsg.equipment,0xFF,sizeof(pMsg.equipment));

	pMsg.level = 1;

	char CharacterName[5][11] = {0};

	if(pMsg.result == 0 || gQueryManager.ExecQuery("SELECT * FROM AccountCharacter WHERE Id='%s'",lpMsg->account) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.result = 0;
	}
	else
	{
		gQueryManager.GetAsString("GameID1",CharacterName[0],sizeof(CharacterName[0]));
		gQueryManager.GetAsString("GameID2",CharacterName[1],sizeof(CharacterName[1]));
		gQueryManager.GetAsString("GameID3",CharacterName[2],sizeof(CharacterName[2]));
		gQueryManager.GetAsString("GameID4",CharacterName[3],sizeof(CharacterName[3]));
		gQueryManager.GetAsString("GameID5",CharacterName[4],sizeof(CharacterName[4]));
		gQueryManager.Close();

		if(GetCharacterSlot(CharacterName,"",&pMsg.slot) == 0)
		{
			pMsg.result = 2;
		}
		else
		{
			if(gQueryManager.ExecQuery("EXEC WZ_CreateCharacter '%s','%s','%d'",lpMsg->account,lpMsg->name,lpMsg->Class) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
			{
				gQueryManager.Close();

				pMsg.result = 0;
			}
			else
			{
				pMsg.result = gQueryManager.GetResult(0);

				gQueryManager.Close();
			}

			if(pMsg.result == 1)
			{
				gQueryManager.ExecQuery("UPDATE AccountCharacter SET GameID%d='%s' WHERE Id='%s'",(pMsg.slot+1),lpMsg->name,lpMsg->account);
				gQueryManager.Close();
			}
		}
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
}

void GDCharacterDeleteRecv(SDHP_CHARACTER_DELETE_RECV* lpMsg,int index) // OK
{
	SDHP_CHARACTER_DELETE_SEND pMsg;

	pMsg.header.set(0x03,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	if(CheckTextSyntax(lpMsg->name,sizeof(lpMsg->name)) == 0)
	{
		pMsg.result = 0;
	}
	else
	{
		pMsg.result = 1;
	}

	if(pMsg.result == 0 || gQueryManager.ExecQuery("EXEC WZ_DeleteCharacter '%s','%s'",lpMsg->account,lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.result = 0;
	}
	else
	{
		pMsg.result = gQueryManager.GetResult(0);

		gQueryManager.Close();

		if(pMsg.result == 1)
		{
			char CharacterName[5][11] = {0};

			if(gQueryManager.ExecQuery("SELECT * FROM AccountCharacter WHERE Id='%s'",lpMsg->account) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
			{
				gQueryManager.Close();

				pMsg.result = 1;
			}
			else
			{
				gQueryManager.GetAsString("GameID1",CharacterName[0],sizeof(CharacterName[0]));
				gQueryManager.GetAsString("GameID2",CharacterName[1],sizeof(CharacterName[1]));
				gQueryManager.GetAsString("GameID3",CharacterName[2],sizeof(CharacterName[2]));
				gQueryManager.GetAsString("GameID4",CharacterName[3],sizeof(CharacterName[3]));
				gQueryManager.GetAsString("GameID5",CharacterName[4],sizeof(CharacterName[4]));
				gQueryManager.Close();

				BYTE slot;

				if(GetCharacterSlot(CharacterName,lpMsg->name,&slot) != 0)
				{
					gQueryManager.ExecQuery("UPDATE AccountCharacter SET GameID%d=NULL WHERE Id='%s'",(slot+1),lpMsg->account);
					gQueryManager.Close();
				}
			}
		}
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
}

void GDCharacterInfoRecv(SDHP_CHARACTER_INFO_RECV* lpMsg,int index) // OK
{
	SDHP_CHARACTER_INFO_SEND pMsg;

	pMsg.header.set(0x04,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	pMsg.result = ((CheckTextSyntax(lpMsg->name,sizeof(lpMsg->name))==0)?0:1);

	if(pMsg.result == 0 || gQueryManager.ExecQuery("SELECT * FROM Character WHERE AccountID='%s' AND Name='%s'",lpMsg->account,lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.result = 0;
	}
	else
	{
		pMsg.Level = (WORD)gQueryManager.GetAsInteger("cLevel");

		pMsg.Class = (BYTE)gQueryManager.GetAsInteger("Class");

		pMsg.LevelUpPoint = gQueryManager.GetAsInteger("LevelUpPoint");

		pMsg.Experience = gQueryManager.GetAsInteger("Experience");

		pMsg.Strength = gQueryManager.GetAsInteger("Strength");

		pMsg.Dexterity = gQueryManager.GetAsInteger("Dexterity");

		pMsg.Vitality = gQueryManager.GetAsInteger("Vitality");

		pMsg.Energy = gQueryManager.GetAsInteger("Energy");

		gQueryManager.GetAsBinary("Inventory",pMsg.Inventory[0],sizeof(pMsg.Inventory));

		gQueryManager.GetAsBinary("MagicList",pMsg.Skill[0],sizeof(pMsg.Skill));

		pMsg.Money = gQueryManager.GetAsInteger("Money");

		pMsg.Life = (DWORD)gQueryManager.GetAsFloat("Life");

		pMsg.MaxLife = (DWORD)gQueryManager.GetAsFloat("MaxLife");

		pMsg.Mana = (DWORD)gQueryManager.GetAsFloat("Mana");

		pMsg.MaxMana = (DWORD)gQueryManager.GetAsFloat("MaxMana");

		pMsg.BP = (DWORD)gQueryManager.GetAsFloat("BP");

		pMsg.MaxBP = (DWORD)gQueryManager.GetAsFloat("MaxBP");

		pMsg.Map = (BYTE)gQueryManager.GetAsInteger("MapNumber");

		pMsg.X = (BYTE)gQueryManager.GetAsInteger("MapPosX");

		pMsg.Y = (BYTE)gQueryManager.GetAsInteger("MapPosY");

		pMsg.Dir = (BYTE)gQueryManager.GetAsInteger("MapDir");

		pMsg.PKCount = gQueryManager.GetAsInteger("PkCount");

		pMsg.PKLevel = gQueryManager.GetAsInteger("PkLevel");

		pMsg.PKTime = gQueryManager.GetAsInteger("PkTime");

		pMsg.CtlCode = (BYTE)gQueryManager.GetAsInteger("CtlCode");

		gQueryManager.GetAsBinary("Quest",pMsg.Quest,sizeof(pMsg.Quest));

		int ChatLimitTime = gQueryManager.GetAsInteger("ChatLimitTime");

		pMsg.ChatLimitTime = (ChatLimitTime < 0) ? 0 : ChatLimitTime;

		gQueryManager.GetAsBinary("EffectList",pMsg.Effect[0],sizeof(pMsg.Effect));

		pMsg.FruitPoint = (WORD)gQueryManager.GetAsInteger("FruitAddPoint");

		gQueryManager.Close();

		gQueryManager.ExecQuery("EXEC WZ_GetResetInfo '%s','%s'",lpMsg->account,lpMsg->name);

		gQueryManager.Fetch();

		pMsg.Reset = gQueryManager.GetAsInteger("Reset");

		gQueryManager.Close();

		gQueryManager.ExecQuery("EXEC WZ_GetMasterResetInfo '%s','%s'",lpMsg->account,lpMsg->name);

		gQueryManager.Fetch();

		pMsg.MasterReset = gQueryManager.GetAsInteger("MasterReset");

		gQueryManager.Close();

		gQueryManager.ExecQuery("EXEC WZ_GetEventEntryInfo '%s','%s'",lpMsg->account,lpMsg->name);

		gQueryManager.Fetch();

		pMsg.BCCount = gQueryManager.GetAsInteger("BCCount");

		pMsg.CCCount = gQueryManager.GetAsInteger("CCCount");

		pMsg.DSCount = gQueryManager.GetAsInteger("DSCount");

		gQueryManager.Close();

		gQueryManager.ExecQuery("UPDATE AccountCharacter SET GameIDC='%s' WHERE Id='%s'",lpMsg->name,lpMsg->account);

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
}

void GDCreateItemRecv(SDHP_CREATE_ITEM_RECV* lpMsg,int index) // OK
{
	SDHP_CREATE_ITEM_SEND pMsg;

	pMsg.header.set(0x07,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	pMsg.X = lpMsg->X;

	pMsg.Y = lpMsg->Y;

	pMsg.Map = lpMsg->Map;

	pMsg.Serial = gItemManager.GetItemSerial();

	pMsg.ItemIndex = lpMsg->ItemIndex;

	pMsg.Level = lpMsg->Level;

	pMsg.Dur = lpMsg->Dur;

	pMsg.Option1 = lpMsg->Option1;

	pMsg.Option2 = lpMsg->Option2;

	pMsg.Option3 = lpMsg->Option3;

	pMsg.NewOption = lpMsg->NewOption;

	pMsg.LootIndex = lpMsg->LootIndex;

	pMsg.SetOption = lpMsg->SetOption;

	pMsg.Duration = lpMsg->Duration;

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
}

void GDOptionDataRecv(SDHP_OPTION_DATA_RECV* lpMsg,int index) // OK
{
	SDHP_OPTION_DATA_SEND pMsg;

	pMsg.header.set(0x08,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	if(gQueryManager.ExecQuery("SELECT * FROM OptionData WHERE Name='%s'",lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		memset(pMsg.SkillKey,0xFF,sizeof(pMsg.SkillKey));

		pMsg.GameOption = 0xFF;
		pMsg.QKey = 0xFF;
		pMsg.WKey = 0xFF;
		pMsg.EKey = 0xFF;
		pMsg.ChatWindow = 0x06;
	}
	else
	{
		gQueryManager.GetAsBinary("SkillKey",pMsg.SkillKey,sizeof(pMsg.SkillKey));

		pMsg.GameOption = (BYTE)gQueryManager.GetAsInteger("GameOption");
		pMsg.QKey = (BYTE)gQueryManager.GetAsInteger("Qkey");
		pMsg.WKey = (BYTE)gQueryManager.GetAsInteger("Wkey");
		pMsg.EKey = (BYTE)gQueryManager.GetAsInteger("Ekey");
		pMsg.ChatWindow = (BYTE)gQueryManager.GetAsInteger("ChatWindow");

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
}

void GDPetItemInfoRecv(SDHP_PET_ITEM_INFO_RECV* lpMsg,int index) // OK
{

}

void GDGlobalPostRecv(SDHP_GLOBAL_POST_RECV* lpMsg,int index) // OK
{
	SDHP_GLOBAL_POST_SEND pMsg;

	pMsg.header.set(0x20,sizeof(pMsg));

	pMsg.type = lpMsg->type;

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	memcpy(pMsg.message,lpMsg->message,sizeof(pMsg.message));

	for(int n=0;n < MAX_SERVER;n++)
	{
		if(gServerManager[n].CheckState() != SERVER_OFFLINE)
		{
			gSocketManager.DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
		}
	}
}

void GDGlobalNoticeRecv(SDHP_GLOBAL_NOTICE_RECV* lpMsg,int index) // OK
{
	SDHP_GLOBAL_NOTICE_SEND pMsg;

	pMsg.header.set(0x21,sizeof(pMsg));

	pMsg.type = lpMsg->type;

	pMsg.count = lpMsg->count;

	pMsg.opacity = lpMsg->opacity;

	pMsg.delay = lpMsg->delay;

	pMsg.color = lpMsg->color;

	pMsg.speed = lpMsg->speed;

	memcpy(pMsg.message,lpMsg->message,sizeof(pMsg.message));

	for(int n=0;n < MAX_SERVER;n++)
	{
		if(gServerManager[n].CheckState() != SERVER_OFFLINE)
		{
			gSocketManager.DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
		}
	}
}

void GDCustomMonsterKilLCountRecv(SDHP_MONSTER_KILL_COUNT_RECV* lpMsg, int index) // OK
{
	SDHP_MONSTER_KILL_COUNT_SEND pMsg;

	pMsg.header.set(0x50,sizeof(pMsg));

	pMsg.Index = lpMsg->Index;

	memcpy(pMsg.Account, lpMsg->Account, sizeof(pMsg.Account));

	memcpy(pMsg.Name, lpMsg->Name, sizeof(pMsg.Name));

	pMsg.MonsterClass = lpMsg->MonsterClass;

	if (gQueryManager.ExecQuery("SELECT * FROM MonsterKillCount WHERE Name='%s' and MonsterClass='%d'", lpMsg->Name, lpMsg->MonsterClass) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		pMsg.Count = 1;

		gQueryManager.Close();
		gQueryManager.ExecQuery("INSERT INTO MonsterKillCount (Name,MonsterClass,MonsterKillCount) VALUES ('%s',%d,%d)", lpMsg->Name, lpMsg->MonsterClass, pMsg.Count);
		gQueryManager.Close();
	}
	else
	{
		pMsg.Count = (gQueryManager.GetAsInteger("MonsterKillCount") + 1);

		gQueryManager.Close();
		gQueryManager.ExecQuery("UPDATE MonsterKillCount SET MonsterKillCount=%d WHERE Name='%s' and MonsterClass='%d'", pMsg.Count, lpMsg->Name, lpMsg->MonsterClass);
		gQueryManager.Close();
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
}

void GDCharacterInfoSaveRecv(SDHP_CHARACTER_INFO_SAVE_RECV* lpMsg) // OK
{
	gQueryManager.BindParameterAsBinary(1,lpMsg->Inventory[0],sizeof(lpMsg->Inventory));
	gQueryManager.BindParameterAsBinary(2,lpMsg->Skill[0],sizeof(lpMsg->Skill));
	gQueryManager.BindParameterAsBinary(3,lpMsg->Quest,sizeof(lpMsg->Quest));
	gQueryManager.BindParameterAsBinary(4,lpMsg->Effect[0],sizeof(lpMsg->Effect));

	gQueryManager.ExecQuery("UPDATE Character SET cLevel=%d,Class=%d,LevelUpPoint=%d,Experience=%d,Strength=%d,Dexterity=%d,Vitality=%d,Energy=%d,Inventory=?,MagicList=?,Money=%d,Life=%f,MaxLife=%f,Mana=%f,MaxMana=%f,BP=%f,MaxBP=%f,MapNumber=%d,MapPosX=%d,MapPosY=%d,MapDir=%d,PkCount=%d,PkLevel=%d,PkTime=%d,Quest=?,ChatLimitTime=%d,EffectList=?,FruitAddPoint=%d WHERE AccountID='%s' AND Name='%s'", lpMsg->Level, lpMsg->Class, lpMsg->LevelUpPoint, lpMsg->Experience, lpMsg->Strength, lpMsg->Dexterity, lpMsg->Vitality, lpMsg->Energy, lpMsg->Money, (float)lpMsg->Life, (float)lpMsg->MaxLife, (float)lpMsg->Mana, (float)lpMsg->MaxMana, (float)lpMsg->BP, (float)lpMsg->MaxBP, lpMsg->Map, lpMsg->X, lpMsg->Y, lpMsg->Dir, lpMsg->PKCount, lpMsg->PKLevel, lpMsg->PKTime, lpMsg->ChatLimitTime, lpMsg->FruitPoint, lpMsg->account, lpMsg->name);
	gQueryManager.Close();

	gQueryManager.ExecQuery("EXEC WZ_SetEventEntryInfo '%s','%s','%d','%d','%d'",lpMsg->account,lpMsg->name,lpMsg->BCCount,lpMsg->CCCount,lpMsg->DSCount);
	gQueryManager.Fetch();
	gQueryManager.Close();
}

void GDInventoryItemSaveRecv(SDHP_INVENTORY_ITEM_SAVE_RECV* lpMsg) // OK
{
	gQueryManager.BindParameterAsBinary(1,lpMsg->Inventory[0],sizeof(lpMsg->Inventory));
	gQueryManager.ExecQuery("UPDATE Character SET Inventory=? WHERE AccountID='%s' AND Name='%s'",lpMsg->account,lpMsg->name);
	gQueryManager.Close();
}

void GDOptionDataSaveRecv(SDHP_OPTION_DATA_SAVE_RECV* lpMsg) // OK
{
	if(gQueryManager.ExecQuery("SELECT Name FROM OptionData WHERE Name='%s'",lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.BindParameterAsBinary(1, lpMsg->SkillKey, sizeof(lpMsg->SkillKey));

		gQueryManager.ExecQuery("INSERT INTO OptionData (Name,SkillKey,GameOption,Qkey,Wkey,Ekey,ChatWindow) VALUES ('%s',?,%d,%d,%d,%d,%d)",lpMsg->name,lpMsg->GameOption,lpMsg->QKey,lpMsg->WKey,lpMsg->EKey,lpMsg->ChatWindow);

		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
		gQueryManager.BindParameterAsBinary(1,lpMsg->SkillKey,sizeof(lpMsg->SkillKey));

		gQueryManager.ExecQuery("UPDATE OptionData SET SkillKey=?,GameOption=%d,Qkey=%d,Wkey=%d,Ekey=%d,ChatWindow=%d WHERE Name='%s'",lpMsg->GameOption,lpMsg->QKey,lpMsg->WKey,lpMsg->EKey,lpMsg->ChatWindow,lpMsg->name);

		gQueryManager.Close();
	}
}

void GDPetItemInfoSaveRecv(SDHP_PET_ITEM_INFO_SAVE_RECV* lpMsg) // OK
{
	
}

void GDResetInfoSaveRecv(SDHP_RESET_INFO_SAVE_RECV* lpMsg) // OK
{
	gQueryManager.ExecQuery("EXEC WZ_SetResetInfo '%s','%s','%d','%d','%d','%d'",lpMsg->account,lpMsg->name,lpMsg->Reset,lpMsg->ResetDay,lpMsg->ResetWek,lpMsg->ResetMon);
	gQueryManager.Fetch();
	gQueryManager.Close();
}

void GDMasterResetInfoSaveRecv(SDHP_MASTER_RESET_INFO_SAVE_RECV* lpMsg) // OK
{
	gQueryManager.ExecQuery("EXEC WZ_SetMasterResetInfo '%s','%s','%d','%d','%d','%d','%d'",lpMsg->account,lpMsg->name,lpMsg->Reset,lpMsg->MasterReset,lpMsg->MasterResetDay,lpMsg->MasterResetWek,lpMsg->MasterResetMon);
	gQueryManager.Fetch();
	gQueryManager.Close();
}

void GDRankingDuelSaveRecv(SDHP_RANKING_DUEL_SAVE_RECV* lpMsg) // OK
{
	if(gQueryManager.ExecQuery("SELECT Name FROM RankingDuel WHERE Name='%s'",lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("INSERT INTO RankingDuel (Name,WinScore,LoseScore) VALUES ('%s',%d,%d)",lpMsg->name,lpMsg->WinScore,lpMsg->LoseScore);
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("UPDATE RankingDuel SET WinScore=WinScore+%d,LoseScore=LoseScore+%d WHERE Name='%s'",lpMsg->WinScore,lpMsg->LoseScore,lpMsg->name);
		gQueryManager.Close();
	}
}

void GDRankingBloodCastleSaveRecv(SDHP_RANKING_BLOOD_CASTLE_SAVE_RECV* lpMsg) // OK
{
	if(gQueryManager.ExecQuery("SELECT Name FROM RankingBloodCastle WHERE Name='%s'",lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("INSERT INTO RankingBloodCastle (Name,Score) VALUES ('%s',%d)",lpMsg->name,lpMsg->score);
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("UPDATE RankingBloodCastle SET Score=Score+%d WHERE Name='%s'",lpMsg->score,lpMsg->name);
		gQueryManager.Close();
	}
}

void GDRankingChaosCastleSaveRecv(SDHP_RANKING_CHAOS_CASTLE_SAVE_RECV* lpMsg) // OK
{
	if(gQueryManager.ExecQuery("SELECT Name FROM RankingChaosCastle WHERE Name='%s'",lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("INSERT INTO RankingChaosCastle (Name,Score) VALUES ('%s',%d)",lpMsg->name,lpMsg->score);
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("UPDATE RankingChaosCastle SET Score=Score+%d WHERE Name='%s'",lpMsg->score,lpMsg->name);
		gQueryManager.Close();
	}
}

void GDRankingDevilSquareSaveRecv(SDHP_RANKING_DEVIL_SQUARE_SAVE_RECV* lpMsg) // OK
{
	if(gQueryManager.ExecQuery("SELECT Name FROM RankingDevilSquare WHERE Name='%s'",lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("INSERT INTO RankingDevilSquare (Name,Score) VALUES ('%s',%d)",lpMsg->name,lpMsg->score);
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("UPDATE RankingDevilSquare SET Score=Score+%d WHERE Name='%s'",lpMsg->score,lpMsg->name);
		gQueryManager.Close();
	}
}

void GDRankingIllusionTempleSaveRecv(SDHP_RANKING_ILLUSION_TEMPLE_SAVE_RECV* lpMsg) // OK
{
	if(gQueryManager.ExecQuery("SELECT Name FROM RankingIllusionTemple WHERE Name='%s'",lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("INSERT INTO RankingIllusionTemple (Name,Score) VALUES ('%s',%d)",lpMsg->name,lpMsg->score);
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("UPDATE RankingIllusionTemple SET Score=Score+%d WHERE Name='%s'",lpMsg->score,lpMsg->name);
		gQueryManager.Close();
	}
}

void GDCreationCardSaveRecv(SDHP_CREATION_CARD_SAVE_RECV* lpMsg) // OK
{
	gQueryManager.ExecQuery("UPDATE AccountCharacter SET ExtClass=%d WHERE Id='%s'",lpMsg->ExtClass,lpMsg->account);
	gQueryManager.Close();
}

void GDConnectCharacterRecv(SDHP_CONNECT_CHARACTER_RECV* lpMsg,int index) // OK
{
	CHARACTER_INFO CharacterInfo;

	strcpy_s(CharacterInfo.Name,lpMsg->name);

	strcpy_s(CharacterInfo.Account,lpMsg->account);

	CharacterInfo.UserIndex = lpMsg->index;

	CharacterInfo.GameServerCode = gServerManager[index].m_ServerCode;

	gCharacterManager.InsertCharacterInfo(CharacterInfo);

	gGuild.MemberOnlineOffline(lpMsg->name,gServerManager[index].m_ServerCode);
}

void GDDisconnectCharacterRecv(SDHP_DISCONNECT_CHARACTER_RECV* lpMsg,int index) // OK
{
	CHARACTER_INFO CharacterInfo;

	if(gCharacterManager.GetCharacterInfo(&CharacterInfo,lpMsg->name) == 0)
	{
		return;
	}

	if(CharacterInfo.UserIndex != lpMsg->index)
	{
		return;
	}

	if(CharacterInfo.GameServerCode != gServerManager[index].m_ServerCode)
	{
		return;
	}

	gCharacterManager.RemoveCharacterInfo(CharacterInfo);

	gGuild.MemberOnlineOffline(lpMsg->name,-1);
}

void GDGlobalWhisperRecv(SDHP_GLOBAL_WHISPER_RECV* lpMsg,int index) // OK
{
	SDHP_GLOBAL_WHISPER_SEND pMsg;

	pMsg.header.set(0x72,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	CHARACTER_INFO CharacterInfo;

	if(gCharacterManager.GetCharacterInfo(&CharacterInfo,lpMsg->TargetName) == 0)
	{
		pMsg.result = 0;
	}
	else
	{
		pMsg.result = 1;
		DGGlobalWhisperEchoSend(CharacterInfo.GameServerCode,CharacterInfo.UserIndex,CharacterInfo.Account,CharacterInfo.Name,lpMsg->name,lpMsg->message);
	}

	memcpy(pMsg.TargetName,lpMsg->TargetName,sizeof(pMsg.TargetName));

	memcpy(pMsg.message,lpMsg->message,sizeof(pMsg.message));

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
}

void DGGlobalWhisperEchoSend(WORD ServerCode,WORD index,char* account,char* name,char* SourceName,char* message) // OK
{
	SDHP_GLOBAL_WHISPER_ECHO_SEND pMsg;

	pMsg.header.set(0x73,sizeof(pMsg));

	pMsg.index = index;

	memcpy(pMsg.account,account,sizeof(pMsg.account));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	memcpy(pMsg.SourceName,SourceName,sizeof(pMsg.SourceName));

	memcpy(pMsg.message,message,sizeof(pMsg.message));

	CServerManager* lpServerManager = FindServerByCode(ServerCode);

	if(lpServerManager != 0)
	{
		gSocketManager.DataSend(lpServerManager->m_index,(BYTE*)&pMsg,pMsg.header.size);
	}
}