#include "IniToSql.h"
#include <Shlwapi.h>
#include <vector>
//////////////////////////////////////////////////////////////////////////
#pragma comment(lib, "shlwapi.lib")
//////////////////////////////////////////////////////////////////////////
/*
bool Ini2SqlDropItem(sqlite3* _pSql, const char* _pszFilename)
{
	if(NULL == _pSql)
	{
		//return false;
	}

	int nRet = 0;
	char* pBuf = new char[102400];
	pBuf[0] = 0;
	DWORD dwRead = 0;

	dwRead = GetPrivateProfileSectionNames(pBuf, 102400 - 1, _pszFilename);
	std::vector<int> xSections;
	char szValue[10];
	int nIndex = 0;
	int nValue = 0;

	for(DWORD i = 0; i < dwRead; ++i)
	{
		if(pBuf[i] == 0)
		{
			szValue[nIndex] = 0;
			nValue = atoi(szValue);
			xSections.push_back(nValue);
			nIndex = 0;
		}
		else
		{
			szValue[nIndex++] = pBuf[i];
		}
	}

	char* pErr = NULL;
	nRet = sqlite3_exec(_pSql, "Create table DropItem(id integer primary key, data varchar(512))", NULL, NULL, &pErr);
	if(nRet != SQLITE_OK)
	{
		sprintf("%s\n", pErr);
		sqlite3_free(pErr);
		return false;
	}

	nRet = sqlite3_exec(_pSql, "begin transaction", NULL, NULL, &pErr);
	if(nRet != SQLITE_OK)
	{
		sprintf("%s\n", pErr);
		sqlite3_free(pErr);
		return false;
	}

	int nStrlen = 0;
	for(size_t i = 0; i < xSections.size(); ++i)
	{
		itoa(xSections[i], szValue, 10);
		sprintf(pBuf, "Insert into DropItem values(%d,",
			xSections[i]);
		nStrlen = strlen(pBuf);
		GetPrivateProfileString(szValue, "data", "", nStrlen + pBuf, 102400, _pszFilename);
		if(strlen(pBuf) > nStrlen)
		{
			strcat(pBuf, ")");
			nRet = sqlite3_exec(_pSql, pBuf, NULL, NULL, &pErr);
			if(nRet == SQLITE_OK)
			{
				printf("Insert to DropItem %s ok\n",
					szValue);
			}
		}
	}

	nRet = sqlite3_exec(_pSql, "commit transaction", NULL, NULL, &pErr);
	if(nRet != SQLITE_OK)
	{
		sprintf("%s\n", pErr);
		sqlite3_free(pErr);
		return false;
	}

	delete[] pBuf;
	return true;
}*/
