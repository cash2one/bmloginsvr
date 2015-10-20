#ifndef _INC_PACKAGEBMP_
#define _INC_PACKAGEBMP_
//////////////////////////////////////////////////////////////////////////
#include "SIBDefine.h"
//////////////////////////////////////////////////////////////////////////
int MergeImgOffset(const char* _pszPath);
int GetDirectoryFiles(const char* _pszPath, const char* _pszExt, FILENAMES& _xFiles);
//	Return the max number
int MakeNumber(FILENAMES& _xNames, FILENUMBERS& _xNumbers);
int SIBPackBMP8BitAnd16Bit(const char* _pszPath, const char* _pszDest);
bool SIBParseOffset(const char* _pszOft, POINT& _pt);
//	Extra
int ExtraSIBFile(const char* _pszSIBPath, const char* _pszOutput);
//////////////////////////////////////////////////////////////////////////
#endif