#pragma once

#define MAX_NOTICE 10
#define MAX_NOTICE_SIZE 128

struct NOTICE_INFO
{
	char text[MAX_NOTICE_SIZE];
	BYTE color;
};

void ClearNotice();
void RenderNotice();
void ScrollNotice();
void NoticeAdd(BYTE color,char* text);