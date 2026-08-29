// ItemValue.cpp: implementation of the CItemValue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemValue.h"
#include "ItemManager.h"
#include "ItemStack.h"
#include "MemScript.h"
#include "Util.h"

CItemValue gItemValue;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemValue::CItemValue() // OK
{
	this->m_ItemValueInfo.clear();
}

CItemValue::~CItemValue() // OK
{

}

void CItemValue::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->m_ItemValueInfo.clear();

	try
	{
		while (true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			ITEM_VALUE_INFO info;

			info.ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetNumber(),lpMemScript->GetAsNumber()));

			info.Level = lpMemScript->GetAsNumber();

			info.Grade = lpMemScript->GetAsNumber();

			info.Value = lpMemScript->GetAsNumber();

			this->m_ItemValueInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;

	this->GCItemListValueSend();
}

bool CItemValue::GetItemValue(CItem* lpItem,int* value) // OK
{
	for(std::vector<ITEM_VALUE_INFO>::iterator it = this->m_ItemValueInfo.begin(); it != this->m_ItemValueInfo.end(); it++)
	{
		if(it->ItemIndex == lpItem->m_Index)
		{
			if(it->Level == -1 || it->Level == lpItem->m_Level)
			{
				if(it->Grade == -1 || it->Grade == lpItem->m_NewOption)
				{
					if(gItemStack.GetItemMaxStack(it->ItemIndex,it->Level) == 0 || it->ItemIndex == GET_ITEM(4,7) || it->ItemIndex == GET_ITEM(4,15))
					{
						(*value) = it->Value;
						return 1;
					}
					else
					{
						(*value) = (int)(it->Value*lpItem->m_Durability);
						return 1;
					}
				}
			}
		}
	}

	return 0;
}


void CItemValue::GCItemListValueSend(int aIndex) // OK
{
	BYTE send[8192];

	PMSG_ITEM_LIST_VALUE_SEND pMsg;

	pMsg.header.set(0xF3,0xED,0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_ITEM_VALUE_INFO info;

	for each(ITEM_VALUE_INFO lpInfo in this->m_ItemValueInfo)
	{
		info.Index = lpInfo.ItemIndex;

		info.Level = lpInfo.Level;

		info.Grade = lpInfo.Grade;

		info.Value = lpInfo.Value;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));

	if(aIndex == -1)
	{
		for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
		{
			if(gObjIsConnectedGP(n) != 0)
			{
				DataSend(n,send,size);
			}
		}
	}
	else
	{
		DataSend(aIndex,send,size);
	}
}