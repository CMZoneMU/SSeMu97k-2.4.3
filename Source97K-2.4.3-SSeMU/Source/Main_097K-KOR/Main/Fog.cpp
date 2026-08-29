#include "stdafx.h"
#include "Fog.h"
#include "CustomFog.h"
#include "Offset.h"
#include "Util.h"

void InitFog() // OK
{
	SetDword(0x005522B8,(DWORD)&glEnableFog);
}

void APIENTRY glEnableFog(GLenum cap) // OK
{
	CUSTOM_FOG_INFO* lpInfo = gCustomFog.GetInfoByNumber(World);

	if(lpInfo != 0)
	{
		GLfloat rgba[4] = { (float)(lpInfo->ColorR/255.0f),(float)(lpInfo->ColorG/255.0f),(float)(lpInfo->ColorB/255.0f),1.0f };
		GLfloat fstart = (float)lpInfo->FogStart;
		GLfloat fend = (float)lpInfo->FogEnd;

		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		if(SceneFlag == 5 && cap != GL_BLEND && cap != GL_TEXTURE_2D && cap != GL_DEPTH_TEST)
		{
			glEnable(GL_FOG);
			glFogi(GL_FOG_MODE,GL_LINEAR);
			glHint(GL_FOG_HINT,GL_NICEST);
			glFogf(GL_FOG_START,fstart);
			glFogf(GL_FOG_END,fend);
			glFogfv(GL_FOG_COLOR,rgba);
			glClearColor(rgba[0],rgba[1],rgba[2],rgba[3]);
			glShadeModel(GL_SMOOTH);
		}
		else
		{
			glDisable(GL_FOG);
		}
	}

	return glEnable(cap);
}