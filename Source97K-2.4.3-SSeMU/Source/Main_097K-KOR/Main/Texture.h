#pragma once

#define MAX_TEXTURE 12000

struct TEXTURE
{
	char Name[32];
	float Width;
	float Height;
	DWORD Component;
	GLuint Texture;
	DWORD Ref;
	BYTE* Buffer;
};

void InitTexture();
void OpenBasicData(HDC hDC);
void SetMaxTextures(DWORD count);
void SetTextures();
void LoadItemModel(int index,char* folder,char* name);
void LoadItemTexture(int index,char* folder);