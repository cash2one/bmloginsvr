#include "StdAfx.h"
#include "EasyUtils.h"
//////////////////////////////////////////////////////////////////////////
bool EasyDownloadFile(const char* _pszUrl, const char* _pszPath)
{
	int ret = URLDownloadToFile(NULL, _pszUrl, _pszPath, 0, NULL);
	if(S_OK != ret)
	{
		return false;
	}
	return true;
}

char* EasyReadFile(const char* _pszFilePath)
{
	FILE* pf = fopen(_pszFilePath, "rb+");
	if(NULL == pf){
		return NULL;
	}

	fseek(pf, 0, SEEK_END);
	size_t size = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	char* pBuf = new char[size + 1];

	int nRead = fread(pBuf, size, 1, pf);
	if(nRead != 1){
		fclose(pf);
		delete[] pBuf;
		pBuf = NULL;
		return NULL;
	}

	fclose(pf);
	pf = NULL;

	pBuf[size] = 0;
	return pBuf;
}