#include "stdafx.h"
#include "Font.h"
#include "Offset.h"
#include "Util.h"

CFont gFont;

CFont::CFont()
{
	GetPrivateProfileString("Font", "FontName", "Tahoma", this->MyFontFaceName, sizeof(this->MyFontFaceName), ".\\Config.ini");

	int iFontHeight = GetPrivateProfileInt("Font", "FontHeight", 13, ".\\Config.ini");

	FontHeight = (iFontHeight > 25) ? 25 : ((iFontHeight < 1) ? 13 : iFontHeight);

	this->Bold = GetPrivateProfileInt("Font", "FontBold", 0, ".\\Config.ini");

	this->Italic = GetPrivateProfileInt("Font", "FontItalic", 0, ".\\Config.ini");

	this->Charset = GetPrivateProfileInt("Font", "FontCharset", DEFAULT_CHARSET, ".\\Config.ini");

	this->Width = GetPrivateProfileInt("Font", "FontWidth", 0, ".\\Config.ini");

	this->UnderLine = GetPrivateProfileInt("Font", "FontUnderline", 0, ".\\Config.ini");

	this->Quality = GetPrivateProfileInt("Font", "FontQuality", NONANTIALIASED_QUALITY, ".\\Config.ini");

	this->StrikeOut = GetPrivateProfileInt("Font", "FontStrikeOut", 0, ".\\Config.ini");

	this->AllFonts.clear();

	this->CurrentFontNumber = 0;
}

CFont::~CFont()
{
	WritePrivateProfileString("Font", "FontName", this->MyFontFaceName, ".\\Config.ini");

	char Text[33] = { 0 };

	wsprintf(Text, "%d", FontHeight);
	WritePrivateProfileString("Font", "FontHeight", Text, ".\\Config.ini");

	wsprintf(Text, "%d", this->Bold);
	WritePrivateProfileString("Font", "FontBold", Text, ".\\Config.ini");

	wsprintf(Text, "%d", this->Italic);
	WritePrivateProfileString("Font", "FontItalic", Text, ".\\Config.ini");

	wsprintf(Text, "%d", this->Charset);
	WritePrivateProfileString("Font", "FontCharset", Text, ".\\Config.ini");

	wsprintf(Text, "%d", this->Width);
	WritePrivateProfileString("Font", "FontWidth", Text, ".\\Config.ini");

	wsprintf(Text, "%d", this->UnderLine);
	WritePrivateProfileString("Font", "FontUnderline", Text, ".\\Config.ini");

	wsprintf(Text, "%d", this->Quality);
	WritePrivateProfileString("Font", "FontQuality", Text, ".\\Config.ini");

	wsprintf(Text, "%d", this->StrikeOut);
	WritePrivateProfileString("Font", "FontStrikeOut", Text, ".\\Config.ini");
}

void CFont::Init()
{
	SetCompleteHook(0xE9, 0x0041F012, 0x0041F08C); // Skip Setting Hardcoded FontHeight

	SetCompleteHook(0xE8, 0x0041F0CD, &this->MyCreateFont);
	SetByte(0x0041F0CD + 5, 0x90);

	SetCompleteHook(0xE8, 0x0041F10E, &this->MyCreateFontBold);
	SetByte(0x0041F10E + 5, 0x90);

	SetCompleteHook(0xE8, 0x0041F151, &this->MyCreateFontBig);
	SetByte(0x0041F151 + 5, 0x90);
}

HFONT CFont::MyCreateFont(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName)
{
	gFont.GetFonts();

	return CreateFont(FontHeight, gFont.Width, 0, 0, (gFont.Bold) ? FW_BOLD : FW_NORMAL, gFont.Italic, gFont.UnderLine, gFont.StrikeOut, gFont.Charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, gFont.Quality, DEFAULT_PITCH, gFont.MyFontFaceName);
}

HFONT CFont::MyCreateFontBold(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName)
{
	return CreateFont(FontHeight, gFont.Width, 0, 0, FW_BOLD, gFont.Italic, gFont.UnderLine, gFont.StrikeOut, gFont.Charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, gFont.Quality, DEFAULT_PITCH, gFont.MyFontFaceName);
}

HFONT CFont::MyCreateFontBig(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName)
{
	return CreateFont(FontHeight * 2, gFont.Width, 0, 0, FW_BOLD, gFont.Italic, gFont.UnderLine, gFont.StrikeOut, gFont.Charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, gFont.Quality, DEFAULT_PITCH, gFont.MyFontFaceName);
}

void CFont::ReloadFont()
{
	if (g_hFont)
	{
		DeleteObject((HGDIOBJ)g_hFont);
	}
	g_hFont = CreateFont(FontHeight, this->Width, 0, 0, (this->Bold) ? FW_BOLD : FW_NORMAL, this->Italic, this->UnderLine, this->StrikeOut, this->Charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, this->Quality, DEFAULT_PITCH, this->MyFontFaceName);

	if (g_hFontBold)
	{
		DeleteObject((HGDIOBJ)g_hFontBold);
	}
	g_hFontBold = CreateFont(FontHeight, this->Width, 0, 0, FW_BOLD, this->Italic, this->UnderLine, this->StrikeOut, this->Charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, this->Quality, DEFAULT_PITCH, this->MyFontFaceName);

	if (g_hFontBig)
	{
		DeleteObject((HGDIOBJ)g_hFontBig);
	}
	g_hFontBig = CreateFont(FontHeight * 2, this->Width, 0, 0, FW_BOLD, this->Italic, this->UnderLine, this->StrikeOut, this->Charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, this->Quality, DEFAULT_PITCH, this->MyFontFaceName);

	OpenFont();

	char Text[33] = { 0 };
	WritePrivateProfileString("Font", "FontName", this->MyFontFaceName, ".\\Config.ini");
	wsprintf(Text, "%d", FontHeight);
	WritePrivateProfileString("Font", "FontHeight", Text, ".\\Config.ini");
	wsprintf(Text, "%d", this->Bold);
	WritePrivateProfileString("Font", "FontBold", Text, ".\\Config.ini");
	wsprintf(Text, "%d", this->Italic);
	WritePrivateProfileString("Font", "FontItalic", Text, ".\\Config.ini");
}

void CFont::GetFonts()
{
	std::string str(this->MyFontFaceName);

	std::set<std::string> uniqueFonts;
	if (!str.empty())
	{
		uniqueFonts.insert(str);
	}

	HDC hDC = g_hDC;
	bool releaseDC = false;
	if (hDC == NULL)
	{
		hDC = GetDC(g_hWnd);
		if (hDC == NULL)
		{
			hDC = GetDC(NULL);
		}
		releaseDC = (hDC != NULL);
	}

	if (hDC != NULL)
	{
		LOGFONT lf = { 0 };
		lf.lfCharSet = (BYTE)this->Charset;
		lf.lfFaceName[0] = '\0';
		lf.lfPitchAndFamily = DEFAULT_PITCH;

		EnumFontFamiliesEx(hDC, &lf, &this->MyEnumFontFamExProc, reinterpret_cast<LPARAM>(&uniqueFonts), 0);

		if (releaseDC && g_hWnd != NULL)
		{
			ReleaseDC(g_hWnd, hDC);
		}
		else if (releaseDC)
		{
			ReleaseDC(NULL, hDC);
		}
	}

	if (uniqueFonts.empty())
	{
		uniqueFonts.insert("Tahoma");
		uniqueFonts.insert("Arial");
		uniqueFonts.insert("Verdana");
		uniqueFonts.insert("Segoe UI");
		uniqueFonts.insert("Courier New");
		uniqueFonts.insert("Comic Sans MS");
	}

	this->AllFonts.clear();

	if (!str.empty())
	{
		uniqueFonts.erase(str);
	}

	this->AllFonts.assign(uniqueFonts.begin(), uniqueFonts.end());

	if (!str.empty())
	{
		this->AllFonts.insert(this->AllFonts.begin(), str);
	}

	this->CurrentFontNumber = 0;
	for (size_t i = 0; i < this->AllFonts.size(); ++i)
	{
		if (_stricmp(this->MyFontFaceName, this->AllFonts[i].c_str()) == 0)
		{
			this->CurrentFontNumber = (int)i;
			break;
		}
	}
}

void CFont::Sync()
{
	if (this->AllFonts.empty())
	{
		this->GetFonts();
	}

	for (size_t i = 0; i < this->AllFonts.size(); ++i)
	{
		if (_stricmp(this->MyFontFaceName, this->AllFonts[i].c_str()) == 0)
		{
			this->CurrentFontNumber = (int)i;
			break;
		}
	}
}

int CALLBACK CFont::MyEnumFontFamExProc(const LOGFONT* lpelfe, const TEXTMETRIC* lpntme, DWORD FontType, LPARAM lParam)
{
	std::set<std::string>* uniqueFonts = reinterpret_cast<std::set<std::string>*>(lParam);

	std::string str = lpelfe->lfFaceName;

	uniqueFonts->insert(str);

	return 1;
}

void InitFont()
{
	gFont.Init();
}

void ReloadFont()
{
	gFont.ReloadFont();
}