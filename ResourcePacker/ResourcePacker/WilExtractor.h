#ifndef _INC_WILEXTRACTOR
#define _INC_WILEXTRACTOR
//////////////////////////////////////////////////////////////////////////
bool WilExtractorMain(const char* _pszFile);
bool WilExtractorMainEx(const char* _pszIniFile, const char* _pszSrcPath, const char* _pszDestPath);

bool WilExtractOneFile(const char* _pszFile, int _nIndex, const char* _pszDestPath);
//////////////////////////////////////////////////////////////////////////
#endif