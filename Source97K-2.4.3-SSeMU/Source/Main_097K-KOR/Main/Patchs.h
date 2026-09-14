#pragma once

void InitPatchs();

void CalcFPS();

DWORD WINAPI FrameWait(DWORD dwStartTime);
void HookFrameWait();

void ReduceRam(LPVOID lpThreadParameter);

void FixShopNpcClose();

void IgnoreRandomStuck(DWORD c, DWORD Damage);

void FixChasingAttackMovement();

void DecBMD();

int OpenTerrainMapping(char* FileName);

int OpenTerrainAttribute(char* FileName);

int OpenObjectsEnc(char* FileName);

extern char WindowName[128];
extern int g_IncreaseFPSSwitch;