// ResourcePacker.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SIBPacker.h"
#include "SIBFile.h"
#include "MapLooker.h"
#include "WilExtractor.h"
#include "IniToSql.h"
#include "MapPacker.h"

//#define PACKER
//#define EXTRACTOR
//#define MAPLOOKER
//#define WILEXTRACTOR
#define MAPPACKER

int _tmain(int argc, _TCHAR* argv[])
{
	//MapLooker();
	//getchar();
	//WilExtractorMain(".\\log.txt");
#ifdef WILEXTRACTOR
	WilExtractorMainEx(argv[1], argv[2], argv[3]);
#endif
	//SIBPackBMP8BitAnd16Bit(argv[1], argv[2]);
	//ExtraSIBFile(argv[1], argv[2]);
#ifdef PACKER
	int nRet = SIBPackBMP8BitAnd16Bit(argv[1], argv[2]);
#endif

#ifdef EXTRACTOR
	//int nRet = ExtraSIBFile(argv[1], argv[2]);
	//IniFileTransToSqlFileMain();
#endif

#ifdef MAPLOOKER
	MapLooker();
#endif

#ifdef MAPPACKER
	MapPackerMain();
#endif


	//getchar();
	return 0;
}