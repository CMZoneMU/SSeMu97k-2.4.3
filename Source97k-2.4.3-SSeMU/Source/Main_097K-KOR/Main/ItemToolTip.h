#pragma once

#define TextNum *(int*)(0x07EAA154)
#define TextListColor (&*(int*)0x07E91708)
#define TextBold (&*(int*)0x07EA7B10)
#define TextList(x) (char*)(0x07E90798+(100*x))

void InitItemToolTip();
void RenderItemInfo(int sx,int sy,DWORD ip,bool sell,int Inventype);
void RenderTipTextList(int x,int y,int count,int tab,int sort,int point);
void AddTipText(char* text,int color,int bold);
void AddTipText(char* text,int color,int bold,int slot);