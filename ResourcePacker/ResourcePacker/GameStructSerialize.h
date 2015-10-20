#ifndef _INC_GAMESTRUCT_SERIALIZE_
#define _INC_GAMESTRUCT_SERIALIZE_
#include <Windows.h>
//#include "../BackMirClient/Common/ObjectData.h"
//#include "../BackMirServer/CMainServer/CUser.h"
//////////////////////////////////////////////////////////////////////////
#include "./ByteBuffer.h"
//////////////////////////////////////////////////////////////////////////
#define ATTRIB_MAPID(AT)	AT.tex
//////////////////////////////////////////////////////////////////////////
//BYTEBUFFER_STRUCT_OPERATOR(ItemAttrib);
//BYTEBUFFER_STRUCT_OPERATOR(UserData);
//BYTEBUFFER_STRUCT_OPERATOR(DelaySendInfo);

struct HeroHeader
{
	char szName[20];
	BYTE bJob;
	BYTE bSex;
	USHORT uLevel;
};

BYTEBUFFER_STRUCT_OPERATOR(HeroHeader);
//////////////////////////////////////////////////////////////////////////
#endif