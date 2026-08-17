#pragma once

#define INVENTORY_WEAR_SIZE 12
#define GetChangeUp(x) (((x>>4)&0x01)?3:(((x>>3)&0x01)?2:1))
#define PointerDeleteItem ((void(__cdecl*)(int,WORD*,int))0x004CCE00)
#define SendRequestEquipmentItem ((bool(_cdecl*)(int,int,int,int))0x0043C250)
#define DeleteEquippingEffect ((void(*)(DWORD))0x0045C130)
#define DeleteBugOrPet ((void(*)(DWORD))0x004FFFA0)
#define GetItemEquiped(x) (*(DWORD*)(MAIN_STRUCT_SERIAL)+(536+(68*x)))
#define GetItemEquipedIndex(x) (*(WORD*)(GetItemEquiped(x)))
#define InventoryMap (&*(WORD*)0x07EA8410)
#define InventoryBackup *(DWORD*)(0x07EA9800)
#define EquipmentItem *(bool*)(0x07EAA165)
#define PointerSlot *(DWORD*)(0x07EA5B18)
#define ItemTarget *(DWORD*)(0x07EAA160)
#define PickedItem *(WORD*)(0x07E91350)

void InitItemMove();
void RightClickMove();
bool AutoMoveItem(int x,int y,DWORD inventory,int width,int height);
BYTE InventoryRectCheck(int x,int y,int width,int height);
BYTE GetInventoryEmptySlot(int ItemIndex);
BYTE GetInventoryWearSlot(DWORD Item);

static int slotx[INVENTORY_WEAR_SIZE] = {485,604,545,545,545,485,604,595,485,515,515,575};
static int sloty[INVENTORY_WEAR_SIZE] = {89,89,46,89,152,152,152,46,46,89,152,152};