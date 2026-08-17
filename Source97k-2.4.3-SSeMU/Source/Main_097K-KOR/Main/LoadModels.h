#pragma once

#define PartObjectColor ((void(*)(int Type, float Alpha, float Bright, float Light[3], bool ExtraMon))0x00503CF0)

void InitModels();

void PartObjectColorHook(int Type, float Alpha, float Bright, float Light[3], bool ExtraMon);