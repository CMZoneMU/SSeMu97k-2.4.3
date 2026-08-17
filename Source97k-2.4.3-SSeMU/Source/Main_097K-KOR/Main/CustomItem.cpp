#include "stdafx.h"
#include "CustomItem.h"

CCustomItem gCustomItem;

CCustomItem::CCustomItem()
{
	for (int n = 0; n < MAX_CUSTOM_ITEM; n++)
	{
		this->m_CustomItemInfo[n].Index = -1;
	}
}

CCustomItem::~CCustomItem()
{

}

void CCustomItem::Load(CUSTOM_ITEM_INFO* info)
{
	for (int n = 0; n < MAX_CUSTOM_ITEM; n++)
	{
		this->SetInfo(info[n]);
	}
}

void CCustomItem::SetInfo(CUSTOM_ITEM_INFO info)
{
	if (info.Index < 0 || info.Index >= MAX_CUSTOM_ITEM)
	{
		return;
	}

	this->m_CustomItemInfo[info.Index] = info;
}

CUSTOM_ITEM_INFO* CCustomItem::GetInfo(int index)
{
	if (index < 0 || index >= MAX_CUSTOM_ITEM)
	{
		return 0;
	}

	if (this->m_CustomItemInfo[index].Index != index)
	{
		return 0;
	}

	return &this->m_CustomItemInfo[index];
}

CUSTOM_ITEM_INFO* CCustomItem::GetInfoByItem(int ItemIndex)
{
	for (int n = 0; n < MAX_CUSTOM_ITEM; n++)
	{
		CUSTOM_ITEM_INFO* lpInfo = this->GetInfo(n);

		if (lpInfo == 0)
		{
			continue;
		}

		if (lpInfo->ItemIndex == ItemIndex)
		{
			return lpInfo;
		}
	}

	return 0;
}

bool CCustomItem::GetCustomItemColor(int ItemIndex, float* ItemColor)
{
	CUSTOM_ITEM_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if (lpInfo == 0)
	{
		return false;
	}

	ItemColor[0] = (lpInfo->ColorR / 255.0f);

	ItemColor[1] = (lpInfo->ColorG / 255.0f);

	ItemColor[2] = (lpInfo->ColorB / 255.0f);

	return true;
}