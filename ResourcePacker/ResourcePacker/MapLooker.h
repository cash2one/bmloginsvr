#ifndef _INC_MAPLOOKER_
#define _INC_MAPLOOKER_
//////////////////////////////////////////////////////////////////////////
#include <stdio.h>
//////////////////////////////////////////////////////////////////////////
#define MODE_NONE		0
#define MODE_LOADMAP	1
#define MODE_GETCOORD	2
#define MODE_GETDATA	3
#define MODE_SETDATA	4
#define MODE_WHOLE		5
#define MODE_SAVE		6
#define MODE_LIST		7
#define MODE_GETINDEX	8
#define MODE_SETINDEX	9
//////////////////////////////////////////////////////////////////////////
void MapLooker();
void PrintAndWrite(FILE* _pfile, const char* _pbuf);
//////////////////////////////////////////////////////////////////////////
//	setdata x,y o[n] : set barea to n
//	setdata x,y b[n] : set bkimg to n
//	setdata x,y m[n] : set midimg to n
//	setdata x,y f[n] : set frimg to n
//	setdata x,y a[n] : set aniframe to n
//	setdata x,y d[n] : set doorindex to n
//	setdata x,y c[n] : set dooroffset to n
//	getdata x,y
//	load path
//	getcoord o[n] : get coordinate where barea equals n
//	getcoord a[n] : get coordinate where has an animation
//	getcoord d[n] : get coordinate where has a door
//	whole o[n] : set the whole map brea to n
//	save path : save the map
//////////////////////////////////////////////////////////////////////////
#endif