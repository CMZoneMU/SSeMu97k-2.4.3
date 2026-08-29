#include "stdafx.h"
#include "Texture.h"
#include "CustomItem.h"
#include "Offset.h"
#include "Util.h"

int& TextureBegin = *(int*)0x0083A4104;

int& TextureCurrent = *(int*)0x083A4108;

TEXTURE m_Texture[MAX_TEXTURE];

void InitTexture() // OK
{
	// Remove JPG size limit
	MemorySet(0x005299E7,0x90,11);
	MemorySet(0x005299F8,0x90,12);
	SetByte(0x00529A18,0xEB); // Width
	SetByte(0x00529A31,0xEB); // Height

	// Remove TGA size limit
	MemorySet(0x00529E37,0x90,22);
	SetByte(0x00529E69,0xEB); // Width
	SetByte(0x00529E8B,0xEB); // Height

	SetTextures();

	SetCompleteHook(0xE8,0x00513950,&OpenBasicData);

	SetCompleteHook(0xE8,0x0051396E,&OpenBasicData);

	SetCompleteHook(0xE9,0x00505BD0,&SetMaxTextures);
}

void OpenBasicData(HDC hDC) // OK
{
	((void(*)(HDC))0x00510320)(hDC);

	TextureCurrent = 5500;

	for(int n=0;n<MAX_CUSTOM_ITEM;n++)
	{
		if(gCustomItem.m_CustomItemInfo[n].Index != -1)
		{
			LoadItemModel((gCustomItem.m_CustomItemInfo[n].ItemIndex+ITEM_BASE_MODEL),((gCustomItem.m_CustomItemInfo[n].ItemIndex >= GET_ITEM(7,0) && gCustomItem.m_CustomItemInfo[n].ItemIndex<GET_ITEM(12,0)) ? "Player\\" : "Item\\"),gCustomItem.m_CustomItemInfo[n].ModelName);
			LoadItemTexture((gCustomItem.m_CustomItemInfo[n].ItemIndex+ITEM_BASE_MODEL),((gCustomItem.m_CustomItemInfo[n].ItemIndex >= GET_ITEM(7,0) && gCustomItem.m_CustomItemInfo[n].ItemIndex<GET_ITEM(12,0)) ? "Player\\" : "Item\\"));
		}
	}
}

void SetMaxTextures(DWORD count) // OK
{
	TextureBegin = 0;
	TextureCurrent = count;
}

void SetTextures() // OK
{
	memset(m_Texture,0,sizeof(m_Texture));

	SetDword(0x0050F6CE+1,(DWORD)(&m_Texture));
	SetDword(0x004F89F8+3,(DWORD)(&m_Texture));
	SetDword(0x00505C16+3,(DWORD)(&m_Texture));
	SetDword(0x00505C6D+3,(DWORD)(&m_Texture));
	SetDword(0x00505DCE+3,(DWORD)(&m_Texture));
	SetDword(0x00529A43+3,(DWORD)(&m_Texture));
	SetDword(0x00529EA2+3,(DWORD)(&m_Texture));
	SetDword(0x0052A068+3,(DWORD)(&m_Texture));

	SetDword(0x0040FF39+2,(DWORD)(&m_Texture->Width));
	SetDword(0x00410917+2,(DWORD)(&m_Texture->Width));
	SetDword(0x00478C2C+2,(DWORD)(&m_Texture->Width));
	SetDword(0x0047970F+2,(DWORD)(&m_Texture->Width));
	SetDword(0x0047F4EF+2,(DWORD)(&m_Texture->Width));
	SetDword(0x0047F617+2,(DWORD)(&m_Texture->Width));
	SetDword(0x004F89F1+3,(DWORD)(&m_Texture->Width));
	
	SetDword(0x0040FF21+2,(DWORD)(&m_Texture->Height));
	SetDword(0x004107C4+2,(DWORD)(&m_Texture->Height));
	SetDword(0x004108FF+2,(DWORD)(&m_Texture->Height));
	SetDword(0x00410B9D+2,(DWORD)(&m_Texture->Height));
	SetDword(0x00478508+3,(DWORD)(&m_Texture->Height));
	SetDword(0x00478C40+2,(DWORD)(&m_Texture->Height));
	SetDword(0x00479700+2,(DWORD)(&m_Texture->Height));
	SetDword(0x0047F41B+2,(DWORD)(&m_Texture->Height));
	SetDword(0x0047F4D7+2,(DWORD)(&m_Texture->Height));
	SetDword(0x0047F5FF+2,(DWORD)(&m_Texture->Height));
	SetDword(0x004F8296+2,(DWORD)(&m_Texture->Height));
	
	SetDword(0x00409FAC+3,(DWORD)(&m_Texture->Component));
	SetDword(0x0040A03C+3,(DWORD)(&m_Texture->Component));
	SetDword(0x0040A797+3,(DWORD)(&m_Texture->Component));
	SetDword(0x0040A7EE+3,(DWORD)(&m_Texture->Component));
	SetDword(0x0040FF45+3,(DWORD)(&m_Texture->Component));
	SetDword(0x00441064+3,(DWORD)(&m_Texture->Component));
	SetDword(0x00441097+3,(DWORD)(&m_Texture->Component));
	SetDword(0x004416C3+3,(DWORD)(&m_Texture->Component));
	SetDword(0x004416FB+3,(DWORD)(&m_Texture->Component));
	SetDword(0x00478C35+2,(DWORD)(&m_Texture->Component));
	SetDword(0x0047955F+3,(DWORD)(&m_Texture->Component));
	SetDword(0x0047F4FB+3,(DWORD)(&m_Texture->Component));
	SetDword(0x00511F12+3,(DWORD)(&m_Texture->Component));
	
	SetDword(0x0047F4C0+1,(DWORD)(&m_Texture->Texture));
	SetDword(0x005114A0+3,(DWORD)(&m_Texture->Texture));
	
	SetDword(0x0040F704+2,(DWORD)(&m_Texture->Buffer));
	SetDword(0x0040FE8F+1,(DWORD)(&m_Texture->Buffer));
	SetDword(0x0040FF1B+2,(DWORD)(&m_Texture->Buffer));
	SetDword(0x0047F434+2,(DWORD)(&m_Texture->Buffer));
	SetDword(0x0047F4D1+2,(DWORD)(&m_Texture->Buffer));
	SetDword(0x004F8282+2,(DWORD)(&m_Texture->Buffer));
	SetDword(0x0052A05F+3,(DWORD)(&m_Texture->Buffer));
	
	SetDword(0x004F0103+2,(DWORD)(&m_Texture)+1936);
	SetDword(0x004F036A+2,(DWORD)(&m_Texture)+1936);
	
	SetDword(0x004F0112+2,(DWORD)(&m_Texture)+1940);
	SetDword(0x004F0378+2,(DWORD)(&m_Texture)+1940);
	
	SetDword(0x004F0281+3,(DWORD)(&m_Texture)+1944);
	SetDword(0x004F041D+3,(DWORD)(&m_Texture)+1944);
	
	SetDword(0x004F026A+1,(DWORD)(&m_Texture)+1948);
	SetDword(0x004F0406+1,(DWORD)(&m_Texture)+1948);
	
	SetDword(0x004F0123+2,(DWORD)(&m_Texture)+1956);
	SetDword(0x004F027B+2,(DWORD)(&m_Texture)+1956);
	SetDword(0x004F0389+2,(DWORD)(&m_Texture)+1956);
	SetDword(0x004F0417+2,(DWORD)(&m_Texture)+1956);
	
	SetDword(0x004F7DFE+3,(DWORD)(&m_Texture)+1960);
	
	SetDword(0x004C106C+3,(DWORD)(&m_Texture)+2000);
	
	SetDword(0x0050CB5A+1,(DWORD)(&m_Texture)+3640);
	
	SetDword(0x0050CB5F+2,(DWORD)(&m_Texture)+4200);
	SetDword(0x0050CBA5+2,(DWORD)(&m_Texture)+4200);
	
	SetDword(0x0050CBEF+2,(DWORD)(&m_Texture)+5432);
}

void LoadItemModel(int index,char* folder,char* name) // OK
{
	if(name[0] == 0)
	{
		return;
	}

	char path[MAX_PATH]={ 0 };

	wsprintf(path,"Data\\%s",folder);

	AccessModel(index,path,name,-1);
}

void LoadItemTexture(int index,char* folder) // OK
{
	OpenTexture(index,folder,GL_NEAREST,GL_TRUE);
}