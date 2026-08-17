// ItemManager.h: interface for the CItemManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "QueryManager.h"

class CItemManager
{
public:
	bool Connect(char* odbc,char* user,char* pass);
	int GetItemCount();
	int GetItemSerial();
	void SaveItemCount();
private:
	CQueryManager m_Query;
	int m_count;
	int m_last;
};

extern CItemManager gItemManager;
