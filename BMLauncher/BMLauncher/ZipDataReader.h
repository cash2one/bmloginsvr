#ifndef _INC_ZIPDATAREADER_
#define _INC_ZIPDATAREADER_
//////////////////////////////////////////////////////////////////////////
#include <ZipArchive.h>
#include <Windows.h>
//#include "SimpleIni.h"
//////////////////////////////////////////////////////////////////////////
#define READZIP_OK					0
#define READZIP_ZIPFILE_NOTFOUND	1
#define READZIP_SUBFILE_NOTFOUND	2
#define READZIP_ERR_UNKNOW			99
//////////////////////////////////////////////////////////////////////////
int ReadZipData(const char* _pszZipFileName, const char* _pszSubFileName, BYTE** _ppReadBuf, size_t* _uLen, const char* _pszZipPsw);
//////////////////////////////////////////////////////////////////////////
#endif