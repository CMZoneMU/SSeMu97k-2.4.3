#pragma once

void InitServerList();
void PrintServerName1(char* a,char* b,char* c,DWORD d);
void PrintServerName2(char* a,char* b,char* c,DWORD d);
void ServerBoxTexture(int a, float b, float c, float d, float e, float f, float g, float h, float i, bool j, bool k);
void ServerBoxText(int a, int b, char* c, int d, int e, SIZE* f);
extern int ServerCode;
extern char ServerName[32][400];