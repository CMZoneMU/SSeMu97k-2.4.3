#pragma once

#define MAX_MAIN_VIEWPORT 400

struct NEW_HEALTH_BAR
{
	WORD index;
	BYTE type;
	BYTE rate;
};

void HealthBarToggle();
void ClearNewHealthBar();
void InsertNewHealthBar(WORD index,BYTE type,BYTE rate);
NEW_HEALTH_BAR* GetNewHealthBar(WORD index,BYTE type);
void DrawNewHealthBar();

extern bool MonsterHPBarMode;