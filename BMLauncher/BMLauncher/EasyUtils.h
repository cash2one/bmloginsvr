#ifndef _INC_EASY_UTILS_
#define _INC_EASY_UTILS_
//////////////////////////////////////////////////////////////////////////
#include <UrlMon.h>
#include <stdio.h>
#pragma comment(lib, "urlmon.lib")
//////////////////////////////////////////////////////////////////////////
bool EasyDownloadFile(const char* _pszUrl, const char* _pszPath);
char* EasyReadFile(const char* _pszFilePath);
//////////////////////////////////////////////////////////////////////////
#endif