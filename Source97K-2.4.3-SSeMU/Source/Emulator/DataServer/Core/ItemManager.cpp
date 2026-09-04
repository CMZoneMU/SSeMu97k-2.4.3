// ItemManager.cpp: implementation of the CItemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemManager.h"
#include "Util.h"

CItemManager gItemManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool CItemManager::Connect(char* odbc,char* user,char* pass) // OK
{
	if(this->m_Query.Connect(odbc,user,pass) == false)
	{
		return false;
	}

	if(this->m_Query.ExecQuery("SELECT ItemCount FROM GameServerInfo") == 0 || this->m_Query.Fetch() == SQL_NO_DATA)
	{
		this->m_count = 0;

		this->m_Query.Close();

		this->m_Query.ExecQuery("INSERT INTO GameServerInfo (Number,ItemCount,ZenCount,AceItemCount) VALUES (0,0,0,0)");
		
		this->m_Query.Close();
	}
	else
	{
		this->m_count = this->m_Query.GetAsInteger("ItemCount");

		this->m_Query.Close();
	}

	this->m_last = this->m_count;

	return true;
}

int CItemManager::GetItemCount() // OK
{
	return this->m_count;
}

int CItemManager::GetItemSerial() // OK
{
	if((++this->m_count) >= 0x7FFFFFFF)
	{
		this->m_count = 1;
	}

	return this->m_count;
}

void CItemManager::SaveItemCount() // OK
{
	if(this->m_count != this->m_last)
	{
		this->m_last = this->m_count;

		this->m_Query.ExecQuery("UPDATE GameServerInfo SET ItemCount=%d",this->m_count);

		this->m_Query.Close();
	}
}