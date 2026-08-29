// MapPath.cpp: implementation of the CMapPath class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapPath.h"
#include "Map.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapPath::CMapPath() // OK
{
	this->m_PathDir[0] = -1;
	this->m_PathDir[1] = -1;
	this->m_PathDir[2] = 0;
	this->m_PathDir[3] = -1;
	this->m_PathDir[4] = 1;
	this->m_PathDir[5] = -1;
	this->m_PathDir[6] = 1;
	this->m_PathDir[7] = 0;
	this->m_PathDir[8] = 1;
	this->m_PathDir[9] = 1;
	this->m_PathDir[10] = 0;
	this->m_PathDir[11] = 1;
	this->m_PathDir[12] = -1;
	this->m_PathDir[13] = 1;
	this->m_PathDir[14] = -1;
	this->m_PathDir[15] = 0;

	this->m_HitAttr = 0;
}

CMapPath::~CMapPath() // OK
{
	if (this->m_HitAttr != 0)
	{
		delete[] this->m_HitAttr;
		this->m_HitAttr = 0;
	}
}

int CMapPath::GetDist(int x1, int y1, int x2, int y2) // OK
{
	int distx = x2 - x1;
	int disty = y2 - y1;
	return (distx*distx) + (disty*disty);
}

int CMapPath::VerifyThatOnPath(int x, int y) // OK
{
	for (int i = 0; i < this->m_NumPath; i++)
	{
		if (x == this->m_PathX[i] && y == this->m_PathY[i])
		{
			return i;
		}
	}
	return -1;
}

bool CMapPath::CanWeMoveForward(int x, int y) // OK
{
	int which = x + y * this->m_width;

	if (x >= this->m_width || x < 0 || y >= this->m_height || y < 0)
	{
		return false;
	}

	if (this->m_MapAttr[which] > 1 || this->m_HitAttr[which])
	{
		this->m_HitAttr[which] = 1;
		return false;
	}

	return true;
}

bool CMapPath::CanWeMoveForward2(int x, int y) // OK
{
	int which = x + y * this->m_width;

	if (x >= this->m_width || x < 0 || y >= this->m_height || y < 0)
	{
		return false;
	}

	if (this->m_MapAttr[which] >= 1 || this->m_HitAttr[which])
	{
		this->m_HitAttr[which] = 1;
		return false;
	}

	return true;
}

bool CMapPath::CanWeMoveForward3(int x, int y) // OK
{
	int which = x + y * this->m_width;

	if (x >= this->m_width || x < 0 || y >= this->m_height || y < 0)
	{
		return false;
	}

	if (this->m_MapAttr[which] >= 1 || this->m_HitAttr[which])
	{
		this->m_HitAttr[which] = 1;
		return false;
	}

	return true;
}

int CMapPath::IsThisSpotOK(int x, int y) // OK
{
	int pos = x + y * this->m_width;

	if (x < 0 || x >= this->m_width || y < 0 || y >= this->m_height)
	{
		return 0;
	}

	if (this->m_HitAttr[pos])
	{
		return 0;
	}

	return 1;
}

int CMapPath::FindNextDir(int sx, int sy, int dx, int dy, int dirstart, bool First, bool ErrorCheck) // OK
{
	int MinDist = 10000000, ldir;
	int WhichDir = 0;

	if (First)
	{
		for (int i = 0; i < 8; i++)
		{
			ldir = i % 8;
			int newx = sx + this->m_PathDir[ldir * 2], newy = sy + this->m_PathDir[ldir * 2 + 1];
			int dist = GetDist(newx, newy, dx, dy);

			if (MinDist > dist && IsThisSpotOK(newx, newy))
			{
				MinDist = dist; WhichDir = ldir;
			}
		}
	}

	else
	{
		for (int i = dirstart + 8 - 1; i <= dirstart + 8 + 1; i++)
		{
			ldir = i % 8;
			int newx = sx + this->m_PathDir[ldir * 2], newy = sy + this->m_PathDir[ldir * 2 + 1];
			int dist = GetDist(newx, newy, dx, dy);

			if (MinDist > dist && IsThisSpotOK(newx, newy))
			{
				MinDist = dist; WhichDir = ldir;
			}
		}

		if (MinDist == 10000000)
		{
			for (int i = dirstart + 2; i <= dirstart + 6; i++)
			{
				ldir = i % 8;
				int newx = sx + this->m_PathDir[ldir * 2], newy = sy + this->m_PathDir[ldir * 2 + 1];
				int dist = GetDist(newx, newy, dx, dy);

				if (MinDist > dist && IsThisSpotOK(newx, newy))
				{
					MinDist = dist; WhichDir = ldir;
				}
			}
		}
	}

	this->m_LastDir = dirstart;

	int LastDist = GetDist(sx, sy, sx + this->m_PathDir[this->m_LastDir * 2], sy + this->m_PathDir[this->m_LastDir * 2 + 1]);

	if (LastDist > MinDist - 6 && LastDist < MinDist + 6 && MinDist >= 25)
	{
		WhichDir = this->m_LastDir;
	}

	if (MinDist == 10000000)
	{
		return -1;
	}

	if (CanWeMoveForward(sx + this->m_PathDir[WhichDir * 2], sy + this->m_PathDir[WhichDir * 2 + 1]))
	{
		int p = VerifyThatOnPath(sx, sy);

		if (p != -1)
		{
			this->m_HitAttr[sx + sy * this->m_width] = 1;
			this->m_NumPath = p;
		}

		return WhichDir;
	}

	if (ErrorCheck)
	{
		WhichDir += 4;
		WhichDir %= 8;

		int negdir = (WhichDir + 8 - 1); negdir %= 8;

		if (CanWeMoveForward(sx + this->m_PathDir[negdir * 2], sy + this->m_PathDir[negdir * 2 + 1]))
		{
			int p = VerifyThatOnPath(sx, sy);

			if (p != -1)
			{
				this->m_HitAttr[sx + sy * this->m_width] = 1;
				this->m_NumPath = p;
			}
			return negdir;
		}

		int posdir = (WhichDir + 8 + 1); posdir %= 8;

		if (CanWeMoveForward(sx + this->m_PathDir[posdir * 2], sy + this->m_PathDir[posdir * 2 + 1]))
		{
			int p = VerifyThatOnPath(sx, sy);

			if (p != -1)
			{
				this->m_HitAttr[sx + sy * this->m_width] = 1;
				this->m_NumPath = p;
			}

			return posdir;
		}
	}

	this->m_HitAttr[sx + sy * this->m_width] = 1;

	return -1;
}

int CMapPath::FindNextDir2(int sx, int sy, int dx, int dy, int dirstart, bool First, bool ErrorCheck) // OK
{
	int MinDist = 10000000, ldir;
	int WhichDir = 0;

	if (First)
	{
		for (int i = 0; i < 8; i++)
		{
			ldir = i % 8;
			int newx = sx + this->m_PathDir[ldir * 2], newy = sy + this->m_PathDir[ldir * 2 + 1];
			int dist = GetDist(newx, newy, dx, dy);

			if (MinDist > dist && IsThisSpotOK(newx, newy))
			{
				MinDist = dist; WhichDir = ldir;
			}
		}
	}
	else
	{
		for (int i = dirstart + 8 - 1; i <= dirstart + 8 + 1; i++)
		{
			ldir = i % 8;
			int newx = sx + this->m_PathDir[ldir * 2], newy = sy + this->m_PathDir[ldir * 2 + 1];
			int dist = GetDist(newx, newy, dx, dy);

			if (MinDist > dist && IsThisSpotOK(newx, newy))
			{
				MinDist = dist; WhichDir = ldir;
			}
		}

		if (MinDist == 10000000)
		{
			for (int i = dirstart + 2; i <= dirstart + 6; i++)
			{
				ldir = i % 8;
				int newx = sx + this->m_PathDir[ldir * 2], newy = sy + this->m_PathDir[ldir * 2 + 1];
				int dist = GetDist(newx, newy, dx, dy);

				if (MinDist > dist && IsThisSpotOK(newx, newy))
				{
					MinDist = dist; WhichDir = ldir;
				}
			}
		}
	}

	this->m_LastDir = dirstart;

	int LastDist = GetDist(sx, sy, sx + this->m_PathDir[this->m_LastDir * 2], sy + this->m_PathDir[this->m_LastDir * 2 + 1]);

	if (LastDist > MinDist - 6 && LastDist < MinDist + 6 && MinDist >= 25)
	{
		WhichDir = this->m_LastDir;
	}

	if (MinDist == 10000000)
		return -1;

	if (CanWeMoveForward2(sx + this->m_PathDir[WhichDir * 2], sy + this->m_PathDir[WhichDir * 2 + 1]))
	{
		int p = VerifyThatOnPath(sx, sy);

		if (p != -1)
		{
			this->m_HitAttr[sx + sy * this->m_width] = 1;
			this->m_NumPath = p;
		}

		return WhichDir;
	}

	if (ErrorCheck)
	{
		WhichDir += 4;
		WhichDir %= 8;

		int negdir = (WhichDir + 8 - 1); negdir %= 8;

		if (CanWeMoveForward2(sx + this->m_PathDir[negdir * 2], sy + this->m_PathDir[negdir * 2 + 1]))
		{
			int p = VerifyThatOnPath(sx, sy);

			if (p != -1)
			{
				this->m_HitAttr[sx + sy * this->m_width] = 1;
				this->m_NumPath = p;
			}

			return negdir;
		}

		int posdir = (WhichDir + 8 + 1); posdir %= 8;

		if (CanWeMoveForward2(sx + this->m_PathDir[posdir * 2], sy + this->m_PathDir[posdir * 2 + 1]))
		{
			int p = VerifyThatOnPath(sx, sy);

			if (p != -1)
			{
				this->m_HitAttr[sx + sy * this->m_width] = 1;
				this->m_NumPath = p;
			}

			return posdir;
		}
	}

	this->m_HitAttr[sx + sy * this->m_width] = 1;
	return -1;
}

int CMapPath::FindNextDir3(int sx, int sy, int dx, int dy, int dirstart, bool First, bool ErrorCheck) // OK
{
	int MinDist = 10000000, ldir;
	int WhichDir = 0;

	if (First)
	{
		for (int i = 0; i < 8; i++)
		{
			ldir = i % 8;
			int newx = sx + this->m_PathDir[ldir * 2], newy = sy + this->m_PathDir[ldir * 2 + 1];
			int dist = GetDist(newx, newy, dx, dy);

			if (MinDist > dist && IsThisSpotOK(newx, newy))
			{
				if (CanWeMoveForward3(newx, newy))
				{
					MinDist = dist;
					WhichDir = ldir;
				}
			}
		}
	}
	else
	{
		for (int i = dirstart + 8 - 1; i <= dirstart + 8 + 1; i++)
		{
			ldir = i % 8;
			int newx = sx + this->m_PathDir[ldir * 2], newy = sy + this->m_PathDir[ldir * 2 + 1];
			int dist = GetDist(newx, newy, dx, dy);

			if (MinDist > dist && IsThisSpotOK(newx, newy))
			{
				if (CanWeMoveForward3(newx, newy))
				{
					MinDist = dist;
					WhichDir = ldir;
				}
			}
		}

		if (MinDist == 10000000)
		{
			for (int i = dirstart + 2; i <= dirstart + 6; i++)
			{
				ldir = i % 8;
				int newx = sx + this->m_PathDir[ldir * 2], newy = sy + this->m_PathDir[ldir * 2 + 1];
				int dist = GetDist(newx, newy, dx, dy);

				if (MinDist > dist && IsThisSpotOK(newx, newy))
				{
					if (CanWeMoveForward3(newx, newy))
					{
						MinDist = dist;
						WhichDir = ldir;
					}
				}
			}
		}
	}

	if (MinDist == 10000000)
	{
		return -1;
	}

	if (CanWeMoveForward3(sx + this->m_PathDir[WhichDir * 2], sy + this->m_PathDir[WhichDir * 2 + 1]))
	{
		int p = VerifyThatOnPath(sx, sy);

		if (p != -1)
		{
			this->m_HitAttr[sx + sy * this->m_width] = 1;
			this->m_NumPath = p;
		}

		return WhichDir;
	}

	if (ErrorCheck)
	{
		WhichDir += 4;
		WhichDir %= 8;

		int negdir = (WhichDir + 8 - 1); negdir %= 8;

		if (CanWeMoveForward3(sx + this->m_PathDir[negdir * 2], sy + this->m_PathDir[negdir * 2 + 1]))
		{
			int p = VerifyThatOnPath(sx, sy);

			if (p != -1)
			{
				this->m_HitAttr[sx + sy * this->m_width] = 1;
				this->m_NumPath = p;
			}
			return negdir;
		}

		int posdir = (WhichDir + 8 + 1); posdir %= 8;

		if (CanWeMoveForward3(sx + this->m_PathDir[posdir * 2], sy + this->m_PathDir[posdir * 2 + 1]))
		{
			int p = VerifyThatOnPath(sx, sy);

			if (p != -1)
			{
				this->m_HitAttr[sx + sy * this->m_width] = 1;
				this->m_NumPath = p;
			}

			return posdir;
		}
	}

	this->m_HitAttr[sx + sy * this->m_width] = 1;
	return -1;
}

bool CMapPath::FindPath(int startx, int starty, int endx, int endy, bool ErrorCheck) // OK
{
	this->m_NumPath = 0;

	int WhichDir = 0;
	int NumFails = 0;

	memset(this->m_HitAttr, 0, this->m_width*this->m_height);

	bool First = true;

	while (startx != endx || starty != endy)
	{
		WhichDir = this->FindNextDir(startx, starty, endx, endy, WhichDir, First, ErrorCheck);

		First = 0;

		if (WhichDir >= 0)
		{
			this->m_PathX[this->m_NumPath] = startx;
			this->m_PathY[this->m_NumPath] = starty;
			startx += this->m_PathDir[WhichDir * 2];
			starty += this->m_PathDir[WhichDir * 2 + 1];
			this->m_NumPath++;

			if (this->m_NumPath >= 15)
			{
				return 0;
			}
		}
		else if (ErrorCheck)
		{
			this->m_NumPath--;

			if (this->m_NumPath < 0)
			{
				return 0;
			}

			startx = this->m_PathX[this->m_NumPath];
			starty = this->m_PathY[this->m_NumPath];
			NumFails++;

			if (NumFails >= 10)
			{
				return 0;
			}
		}
		else
		{
			break;
		}

	}

	this->m_PathX[this->m_NumPath] = startx;
	this->m_PathY[this->m_NumPath] = starty;
	this->m_NumPath++;

	if (this->m_NumPath >= 15)
	{
		return 0;
	}

	return 1;
}

bool CMapPath::FindPath2(int startx, int starty, int endx, int endy, bool ErrorCheck) // OK
{
	this->m_NumPath = 0;

	int WhichDir = 0;
	int NumFails = 0;

	memset(this->m_HitAttr, 0, this->m_width*this->m_height);

	bool First = true;

	while (startx != endx || starty != endy)
	{
		WhichDir = this->FindNextDir2(startx, starty, endx, endy, WhichDir, First, ErrorCheck);

		First = false;

		if (WhichDir >= 0)
		{
			this->m_PathX[this->m_NumPath] = startx;
			this->m_PathY[this->m_NumPath] = starty;
			startx += this->m_PathDir[WhichDir * 2];
			starty += this->m_PathDir[WhichDir * 2 + 1];
			this->m_NumPath++;

			if (this->m_NumPath >= 14)
			{
				this->m_NumPath--;
				break;
			}
		}
		else if (ErrorCheck)
		{
			this->m_NumPath--;

			if (this->m_NumPath < 0)
			{
				return 0;
			}

			startx = this->m_PathX[this->m_NumPath];
			starty = this->m_PathY[this->m_NumPath];
			NumFails++;

			if (NumFails >= 10)
			{
				return 0;
			}
		}
		else
		{
			break;
		}

	}

	this->m_PathX[this->m_NumPath] = startx;
	this->m_PathY[this->m_NumPath] = starty;
	this->m_NumPath++;

	if (this->m_NumPath >= 15)
	{
		return 0;
	}

	return 1;
}

bool CMapPath::FindPath3(int startx, int starty, int endx, int endy, bool ErrorCheck) // OK
{
	this->m_NumPath = 0;

	int WhichDir = 0;
	int NumFails = 0;

	memset(this->m_HitAttr, 0, this->m_width*this->m_height);

	bool First = true;

	while (startx != endx || starty != endy)
	{
		WhichDir = this->FindNextDir3(startx, starty, endx, endy, WhichDir, First, ErrorCheck);

		First = false;

		if (WhichDir >= 0)
		{
			this->m_PathX[this->m_NumPath] = startx;
			this->m_PathY[this->m_NumPath] = starty;
			startx += this->m_PathDir[WhichDir * 2];
			starty += this->m_PathDir[WhichDir * 2 + 1];
			this->m_NumPath++;

			if (this->m_NumPath >= 14)
			{
				this->m_NumPath--;
				break;
			}
		}
		else if (ErrorCheck)
		{
			this->m_NumPath--;

			if (this->m_NumPath < 0)
			{
				return 0;
			}

			startx = this->m_PathX[this->m_NumPath];
			starty = this->m_PathY[this->m_NumPath];
			NumFails++;

			if (NumFails >= 10)
			{
				return false;
			}
		}
		else
		{
			break;
		}
	}

	this->m_PathX[this->m_NumPath] = startx;
	this->m_PathY[this->m_NumPath] = starty;
	this->m_NumPath++;

	if (this->m_NumPath >= 15)
	{
		return 0;
	}

	return 1;
}

void CMapPath::SetMapDimensions(int width, int height, BYTE* MapAttr) // OK
{
	this->m_width = width;

	this->m_height = height;

	this->m_MapAttr = MapAttr;

	this->m_HitAttr = new BYTE[this->m_width*this->m_height];
}

int CMapPath::GetPath() // OK
{
	return this->m_NumPath;
}

BYTE* CMapPath::GetPathX() // OK
{
	return this->m_PathX;
}

BYTE* CMapPath::GetPathY() // OK
{
	return this->m_PathY;
}
