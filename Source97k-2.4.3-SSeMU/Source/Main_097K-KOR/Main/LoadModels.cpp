#include "stdafx.h"
#include "LoadModels.h"
#include "CustomItem.h"

void InitModels()
{
	SetCompleteHook(0xFF, 0x0045A038, &PartObjectColorHook);

	SetCompleteHook(0xFF, 0x00504A02, &PartObjectColorHook);
}

void PartObjectColorHook(int Type, float Alpha, float Bright, float Light[3], bool ExtraMon)
{
	if (gCustomItem.GetCustomItemColor((Type - ITEM_BASE_MODEL), Light) == false)
	{
		PartObjectColor(Type, Alpha, Bright, Light, ExtraMon);
	}
}