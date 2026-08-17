// Protect.cpp: implementation of the CProtect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Protect.h"
#include "CCRC32.H"
#include "Util.h"

CProtect gProtect;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProtect::CProtect() // OK
{

}

CProtect::~CProtect() // OK
{

}

bool CProtect::ReadMainFile(char* name) // OK
{
	CCRC32 CRC32;

	if(CRC32.FileCRC(name,&this->m_ClientFileCRC,1024) == 0)
	{
		ErrorMessageBox("[Error] ReadMainFile #1: Failed to calculate CRC for the main file.");
		return false;
	}

	HANDLE file = CreateFile(name,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,0);

	if(file == INVALID_HANDLE_VALUE)
	{
		ErrorMessageBox("[Error] ReadMainFile #2: Failed to open the main file.");
		return false;
	}

	if(GetFileSize(file,0) != sizeof(MAIN_FILE_INFO))
	{
		ErrorMessageBox("[Error] ReadMainFile #3: Unexpected size for the main file.");
		CloseHandle(file);
		return false;
	}

	DWORD OutSize = 0;

	if(ReadFile(file,&this->m_MainInfo,sizeof(MAIN_FILE_INFO),&OutSize,0) == 0)
	{
		ErrorMessageBox("[Error] ReadMainFile #4: Failed to read the main file data.");
		CloseHandle(file);
		return false;
	}

	for(int n=0;n < sizeof(MAIN_FILE_INFO);n++)
	{
		((BYTE*)&this->m_MainInfo)[n] += (BYTE)(0x95^HIBYTE(n));
		((BYTE*)&this->m_MainInfo)[n] ^= (BYTE)(0xCA^LOBYTE(n));
	}

	CloseHandle(file);
	return true;
}

void CProtect::CheckLauncher() // OK
{
	if((this->m_MainInfo.LauncherStart & 1) == 0)
	{
		return;
	}

	if(OpenMutex(MUTEX_ALL_ACCESS,0,this->m_MainInfo.LauncherMutex) == 0)
	{
		ExitProcess(0);
	}
}

void CProtect::CheckInstance() // OK
{
	if((this->m_MainInfo.LauncherStart & 2) == 0)
	{
		return;
	}

	char buff[256];

	wsprintf(buff,"MAIN_97KOR_%s",this->m_MainInfo.IpAddress);

	if(OpenMutex(MUTEX_ALL_ACCESS,0,buff) == 0)
	{
		CreateMutex(0,0,buff);
	}
	else
	{
		ExitProcess(0);
	}
}

void CProtect::CheckClientFile() // OK
{
	if(this->m_MainInfo.ClientNameCRC == 0)
	{
		return;
	}

	char name[MAX_PATH] = {0};

	if(GetModuleFileName(0,name,sizeof(name)) == 0)
	{
		ErrorMessageBox("[Error] CheckClientFile #1: Failed to get the module file name.");
		ExitProcess(0);
	}

	if(_stricmp(ConvertModuleFileName(name),this->m_MainInfo.ClientName) != 0)
	{
		ErrorMessageBox("[Error] CheckClientFile #2: Unexpected client file name.");
		ExitProcess(0);
	}

	CCRC32 CRC32;

	DWORD ClientCRC32;

	if(CRC32.FileCRC(this->m_MainInfo.ClientName,&ClientCRC32,1024) == 0)
	{
		ErrorMessageBox("[Error] CheckClientFile #3: Failed to calculate CRC for the client file.");
		ExitProcess(0);
	}

	if(this->m_MainInfo.ClientNameCRC != ClientCRC32)
	{
		ErrorMessageBox("[Error] CheckClientFile #4: Client file CRC does not match.");
		ExitProcess(0);
	}
}

void CProtect::CheckPluginFile() // OK
{
	for(int n = 0; n < 5; n++)
	{
		if(this->m_MainInfo.PluginNameCRC[n] == 0)
		{
			continue;
		}

		CCRC32 CRC32;

		DWORD PluginNameCRC;

		if(CRC32.FileCRC(this->m_MainInfo.PluginName[n],&PluginNameCRC,1024) == 0)
		{
			ErrorMessageBox("[Error] CheckPluginFile #1: Failed to calculate CRC for a plugin file.");
			ExitProcess(0);
		}

		if(this->m_MainInfo.PluginNameCRC[n] != PluginNameCRC)
		{
			ErrorMessageBox("[Error] CheckPluginFile #2: Plugin file CRC does not match.");
			ExitProcess(0);
		}

		HMODULE module = LoadLibrary(this->m_MainInfo.PluginName[n]);

		if(module == 0)
		{
			ErrorMessageBox("[Error] CheckPluginFile #3: Failed to load a plugin file.");
			ExitProcess(0);
		}

		void (*EntryProc)() = (void(*)())GetProcAddress(module,"EntryProc");

		if(EntryProc != 0)
		{
			EntryProc();
		}
	}
}