#pragma once

int ReadFontFile(char* path);
void InitFont();
void ReloadFont();
HFONT FontNormal();
HFONT FontBold();
HFONT FontBig();

extern int FontSize;