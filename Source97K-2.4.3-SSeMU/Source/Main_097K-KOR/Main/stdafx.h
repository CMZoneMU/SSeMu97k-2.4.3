#pragma once

typedef unsigned __int64 QWORD;

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT _WIN32_WINNT_WINXP

#ifndef DEBUG_CONSOLE
#define DEBUG_CONSOLE 0
#endif

// System Include
#include <windows.h>
#include <winsock2.h>
#include <math.h>
#include <vector>
#include <map>
#include <cstdio>
#include <iostream>
#include <time.h>
#include <Shellapi.h>
#include <Rpc.h>
#include <gl\GL.h>

// General Includes
#include "Offset.h"
#include "Define.h"
#include "Console.h"
#include "Util.h"
#include "MathLib.h"

// Libraries
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Rpcrt4.lib")
#pragma comment(lib,"Opengl32.lib")

extern WORD CharacterDeleteMaxLevel;
extern DWORD gLevelExperience[1001];