#include "IniToSql.h"
//#include "Ini2Sql.h"
#include <assert.h>
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "sqlite3.lib")

static const char* szKeyName[] =
{
	"id",
	"name",
	"lucky",
	"curse",
	"hide",
	"accuracy",
	"atkspeed",
	"atkpalsy",
	"atkpois",
	"movespeed",
	"weight",
	"reqtype",
	"reqvalue",
	"sex",
	"type",
	"maxdc",
	"dc",
	"maxac",
	"ac",
	"maxmac",
	"mac",
	"maxsc",
	"sc",
	"maxmc",
	"mc",
	"maxhp",
	"hp",
	"maxmp",
	"mp",
	"expr",
	"maxexpr",
	"level",
	"extra",
	"tex"
};

char szItemPath[MAX_PATH] = {0};
char szMonsPath[MAX_PATH] = {0};
char szDropItem[MAX_PATH] = {0};
char szMagicsPath[MAX_PATH] = {0};

void GetIniFileSections(const char* _pszFile, std::vector<int>& _xOutput)
{
	int nRet = 0;
	char* pBuf = new char[102400];
	DWORD dwRead = 0;

	dwRead = GetPrivateProfileSectionNames(pBuf, 102399, _pszFile);
	char szValue[10];
	int nIndex = 0;
	int nValue = 0;

	for(DWORD i = 0; i < dwRead; ++i)
	{
		if(pBuf[i] == 0)
		{
			szValue[nIndex] = 0;
			nValue = atoi(szValue);
			_xOutput.push_back(nValue);
			nIndex = 0;
		}
		else
		{
			szValue[nIndex++] = pBuf[i];
		}
	}

	delete[] pBuf;
}

void GetIniFileKeys(const char* _pszAppName, const char* _pszFile, std::vector<int>& _xOutput)
{
	int nRet = 0;
	char* pBuf = new char[102400];
	DWORD dwRead = 0;

	dwRead = GetPrivateProfileSection(_pszAppName, pBuf, 102399, _pszFile);
	char szValue[10];
	int nIndex = 0;
	int nValue = 0;

	bool bReadEqual = false;

	for(DWORD i = 0; i < dwRead; ++i)
	{
		//if(pBuf[i] == 0)
		if(pBuf[i] == '=')
		{
			bReadEqual = true;
			szValue[nIndex] = 0;
			nValue = atoi(szValue);
			_xOutput.push_back(nValue);
			nIndex = 0;
		}
		else if(pBuf[i] == 0)
		{
			bReadEqual = false;
		}
		else if(!bReadEqual &&
			pBuf[i] >= '0' &&
			pBuf[i] <= '9')
		{
			szValue[nIndex++] = pBuf[i];
		}
	}

	delete[] pBuf;
}

bool ParseDropItem(const char* _pszFile, char* _pBuf, int _id, std::map<std::string, int>& _xNameIDMap)
{
	char szSubValue[10];
	char szDescpribe[MAX_PATH];
	char szItemName[20];
	char szProb[10];
	char szValue[10];
	itoa(_id, szValue, 10);

	std::vector<int> xKeys;
	//std::map<std::string, int> xNameIdMap;
	//if(false == GetNameToIDMap(szItemPath, xNameIdMap))
	{
		//return false;
	}
	//if(0 == xNameIdMap.size())
	{
		//return false;
	}

	//////////////////////////////////////////////////////////////////////////
	xKeys.clear();
	GetIniFileKeys(szValue, _pszFile, xKeys);
	bool bWriteName = true;
	int nNameIndex = 0;
	int nProbIndex = 0;
	bool bFirst = true;
	char szOut[100];
	bool bRet = false;

	if(!xKeys.empty())
	{
		for(int i = 0; i < xKeys.size(); ++i)
		{
			itoa(xKeys[i], szSubValue, 10);
			GetPrivateProfileString(szValue, szSubValue, "", szDescpribe, MAX_PATH - 1, _pszFile);
			bWriteName = true;
			nNameIndex = 0;
			nProbIndex = 0;

			if(strlen(szDescpribe))
			{
				for(int i = 0; i <= strlen(szDescpribe); ++i)
				{
					if(szDescpribe[i] == '|')
					{
						bWriteName = false;
						szItemName[nNameIndex] = 0;
						nNameIndex = 0;
					}
					else if(szDescpribe[i] == '\0')
					{
						szProb[nProbIndex] = 0;
						nProbIndex = 0;

						int nProb = atoi(szProb);
						if(nProb != 0 &&
							strlen(szItemName))
						{
							std::string xName = szItemName;
							std::map<std::string, int>::const_iterator fnditer = _xNameIDMap.find(xName);

							if(fnditer != _xNameIDMap.end())
							{
								szOut[0] = 0;
								if(bFirst)
								{
									sprintf(szOut, "%d|%d",
										fnditer->second, nProb);
									bFirst = false;
								}
								else
								{
									sprintf(szOut, ",%d|%d",
										fnditer->second, nProb);
								}
								if(szOut[0] != 0)
								{
									strcat(_pBuf, szOut);
									bRet = true;
								}
							}
						}
					}
					else
					{
						if(bWriteName)
						{
							szItemName[nNameIndex++] = szDescpribe[i];
						}
						else
						{
							szProb[nProbIndex++] = szDescpribe[i];
						}
					}
				}
			}
		}
	}

	return bRet;
}

bool ParseDropItem(const char* _pszFile, char* _pBuf, int _id)
{
	char szSubValue[10];
	char szDescpribe[MAX_PATH];
	char szItemName[20];
	char szProb[10];
	char szValue[10];
	itoa(_id, szValue, 10);

	std::vector<int> xKeys;
	std::map<std::string, int> xNameIdMap;
	if(false == GetNameToIDMap(szItemPath, xNameIdMap))
	{
		return false;
	}
	if(0 == xNameIdMap.size())
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	xKeys.clear();
	GetIniFileKeys(szValue, _pszFile, xKeys);
	bool bWriteName = true;
	int nNameIndex = 0;
	int nProbIndex = 0;
	bool bFirst = true;
	char szOut[100];
	bool bRet = false;

	if(!xKeys.empty())
	{
		for(int i = 0; i < xKeys.size(); ++i)
		{
			itoa(xKeys[i], szSubValue, 10);
			GetPrivateProfileString(szValue, szSubValue, "", szDescpribe, MAX_PATH - 1, _pszFile);
			bWriteName = true;
			nNameIndex = 0;
			nProbIndex = 0;

			if(strlen(szDescpribe))
			{
				for(int i = 0; i <= strlen(szDescpribe); ++i)
				{
					if(szDescpribe[i] == '|')
					{
						bWriteName = false;
						szItemName[nNameIndex] = 0;
						nNameIndex = 0;
					}
					else if(szDescpribe[i] == '\0')
					{
						szProb[nProbIndex] = 0;
						nProbIndex = 0;

						int nProb = atoi(szProb);
						if(nProb != 0 &&
							strlen(szItemName))
						{
							std::string xName = szItemName;
							std::map<std::string, int>::const_iterator fnditer = xNameIdMap.find(xName);

							if(fnditer != xNameIdMap.end())
							{
								szOut[0] = 0;
								if(bFirst)
								{
									sprintf(szOut, "%d|%d",
										fnditer->second, nProb);
									bFirst = false;
								}
								else
								{
									sprintf(szOut, ",%d|%d",
										fnditer->second, nProb);
								}
								if(szOut[0] != 0)
								{
									strcat(_pBuf, szOut);
									bRet = true;
								}
							}
						}
					}
					else
					{
						if(bWriteName)
						{
							szItemName[nNameIndex++] = szDescpribe[i];
						}
						else
						{
							szProb[nProbIndex++] = szDescpribe[i];
						}
					}
				}
			}
		}
	}

	return bRet;
}

bool Ini2SqlDropItem(sqlite3* _pSql, const char* _pszFilename)
{
	if(NULL == _pSql)
	{
		//return false;
	}

	int nRet = 0;
	char* pBuf = new char[102400];
	pBuf[0] = 0;
	char szValue[10];
	
	std::vector<int> xSections;
	GetIniFileSections(_pszFilename, xSections);

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

	std::map<std::string, int> xNameIdMap;
	if(false == GetNameToIDMap(szItemPath, xNameIdMap))
	{
		return false;
	}
	if(0 == xNameIdMap.size())
	{
		return false;
	}

	int nStrlen = 0;
	for(size_t i = 0; i < xSections.size(); ++i)
	{
		itoa(xSections[i], szValue, 10);
		sprintf(pBuf, "Insert into DropItem values(%d,'",
			xSections[i]);
		nStrlen = strlen(pBuf);
		//GetPrivateProfileString(szValue, "data", "", nStrlen + pBuf, 102400, _pszFilename);
		if(ParseDropItem(_pszFilename, nStrlen + pBuf, xSections[i], xNameIdMap))
		{
			if(strlen(pBuf) > nStrlen)
			{
				strcat(pBuf, "')");
				nRet = sqlite3_exec(_pSql, pBuf, NULL, NULL, &pErr);
				if(nRet == SQLITE_OK)
				{
					printf("Insert to DropItem %s ok\n",
						szValue);
				}
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
}

bool Ini2SqlItems(sqlite3* _pSql, const char* _pszFilename)
{
	char* pErr = NULL;
	char buf[10240];
	ZeroMemory(buf, sizeof(buf));
	ItemAttrib item;
	ZeroMemory(&item, sizeof(ItemAttrib));
	char szID[10];
	char szValue[MAX_PATH];

	std::vector<int> xSections;
	GetIniFileSections(_pszFilename, xSections);

	//	Item表
	if(strlen(_pszFilename) != 0)
	{
		sprintf(buf, "create table Items (id integer primary key,"\
			"name varchar(20),"\
			"lucky char,"\
			"curse char,"\
			"hide char,"\
			"accuracy char,"\
			"atkspeed char,"\
			"atkpalsy char,"\
			"atkpois char,"\
			"movespeed char,"\
			"weight char,"\
			"reqtype char,"\
			"reqvalue char,"\
			"sex char,"\
			"type char,"\
			"maxdc integer,"\
			"dc integer,"\
			"maxac integer,"\
			"ac integer,"\
			"maxmac integer,"\
			"mac integer,"\
			"maxsc integer,"\
			"sc integer,"\
			"maxmc integer,"\
			"mc integer,"\
			"maxhp integer,"\
			"hp integer,"\
			"maxmp integer,"\
			"mp integer,"\
			"maxexpr integer,"\
			"expr integer,"\
			"level integer,"\
			"extra integer,"\
			"tex integer,"\
			"price integer)");

		if(SQLITE_OK != sqlite3_exec(_pSql, buf, NULL, NULL, &pErr))
		{
			printf(pErr);
			printf("\n创建表Items失败\n");
			sqlite3_free(pErr);
			return false;
		}

		int nRet = sqlite3_exec(_pSql, "Begin transaction", 0, 0, &pErr);
		if(nRet != SQLITE_OK)
		{
			printf("%s\n", pErr);
			sqlite3_free(pErr);
			return false;
		}

		for(int i = 0; i < xSections.size(); ++i)
		{
			sprintf(szID, "%d", xSections[i]);
			::GetPrivateProfileString(szID, "name", "", szValue, sizeof(szValue), _pszFilename);
			if(strlen(szValue) == 0)
			{
				//	无效的NAME
				continue;
			}

			//	name
			item.id = xSections[i];
			strcpy(item.name, szValue);

			item.lucky			 = ::GetPrivateProfileInt(szID, "lucky", 0, _pszFilename);
			item.curse			 = ::GetPrivateProfileInt(szID, "curse", 0, _pszFilename);
			item.hide			 = ::GetPrivateProfileInt(szID, "hide", 0, _pszFilename);
			item.accuracy		 = ::GetPrivateProfileInt(szID, "accuracy", 0, _pszFilename);
			item.atkSpeed		 = ::GetPrivateProfileInt(szID, "atkspeed", 0, _pszFilename);
			item.atkPalsy		 = ::GetPrivateProfileInt(szID, "atkpalsy", 0, _pszFilename);
			item.atkPois		 = ::GetPrivateProfileInt(szID, "atkPois", 0, _pszFilename);
			item.moveSpeed		 = ::GetPrivateProfileInt(szID, "movespeed", 0, _pszFilename);
			item.weight			 = ::GetPrivateProfileInt(szID, "weight", 0, _pszFilename);
			item.reqType		 = ::GetPrivateProfileInt(szID, "reqtype", 0, _pszFilename);
			item.reqValue		 = ::GetPrivateProfileInt(szID, "reqvalue", 0, _pszFilename);
			item.sex			 = ::GetPrivateProfileInt(szID, "sex", 0, _pszFilename);
			item.type			 = ::GetPrivateProfileInt(szID, "type", 0, _pszFilename);
			item.maxDC			 = ::GetPrivateProfileInt(szID, "maxdc", 0, _pszFilename);
			item.DC				 = ::GetPrivateProfileInt(szID, "dc", 0, _pszFilename);
			item.maxAC			 = ::GetPrivateProfileInt(szID, "maxac", 0, _pszFilename);
			item.AC				 = ::GetPrivateProfileInt(szID, "ac", 0, _pszFilename);
			item.maxMAC			 = ::GetPrivateProfileInt(szID, "maxmac", 0, _pszFilename);
			item.MAC			 = ::GetPrivateProfileInt(szID, "mac", 0, _pszFilename);
			item.maxSC			 = ::GetPrivateProfileInt(szID, "maxsc", 0, _pszFilename);
			item.SC				 = ::GetPrivateProfileInt(szID, "sc", 0, _pszFilename);
			item.maxMC			 = ::GetPrivateProfileInt(szID, "maxmc", 0, _pszFilename);
			item.MC				 = ::GetPrivateProfileInt(szID, "mc", 0, _pszFilename);
			item.maxHP			 = ::GetPrivateProfileInt(szID, "maxhp", 0, _pszFilename);
			item.HP				 = ::GetPrivateProfileInt(szID, "hp", 0, _pszFilename);
			item.maxMP			 = ::GetPrivateProfileInt(szID, "maxmp", 0, _pszFilename);
			item.MP				 = ::GetPrivateProfileInt(szID, "mp", 0, _pszFilename);
			item.maxEXPR		 = ::GetPrivateProfileInt(szID, "maxexpr", 0, _pszFilename);
			item.EXPR			 = ::GetPrivateProfileInt(szID, "expr", 0, _pszFilename);
			item.level			 = ::GetPrivateProfileInt(szID, "level", 0, _pszFilename);
			item.extra			 = ::GetPrivateProfileInt(szID, "extra", 0, _pszFilename);
			item.tex			 = ::GetPrivateProfileInt(szID, "tex", 0, _pszFilename);
			item.price			 = ::GetPrivateProfileInt(szID, "price", 0, _pszFilename);

			sprintf(buf, "insert into Items values(%d,"\
				"'%s',"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d)",
				item.id,
				item.name,
				item.lucky,
				item.curse,
				item.hide,
				item.accuracy,
				item.atkSpeed,
				item.atkPalsy,
				item.atkPois,
				item.moveSpeed,
				item.weight,
				item.reqType,
				item.reqValue,
				item.sex,
				item.type,
				item.maxDC,
				item.DC,
				item.maxAC,
				item.AC,
				item.maxMAC,
				item.MAC,
				item.maxSC,
				item.SC,
				item.maxMC,
				item.MC,
				item.maxHP,
				item.HP,
				item.maxMP,
				item.MP,
				item.maxEXPR,
				item.EXPR,
				item.level,
				item.extra,
				item.tex,
				item.price);

			if(SQLITE_OK != sqlite3_exec(_pSql, buf, NULL, NULL, &pErr))
			{
				printf(pErr);
				printf("ID[%d]写入表Item失败\n", item.id);
				sqlite3_free(pErr);
				return -3;
			}
			else
			{
				printf("成功写入ID[%d]进表Item\n", item.id);
			}
		}

		nRet = sqlite3_exec(_pSql, "Commit transaction", NULL, 0, &pErr);
		if(nRet != SQLITE_OK)
		{
			printf("%s\n", pErr);
			sqlite3_free(pErr);
			return false;
		}
		else
		{
			printf("transaction Items OK!\n");
		}
	}

	return true;
}

bool Ini2SqlMonsters(sqlite3* _pSql, const char* _pszFilename)
{
	char* pErr = NULL;
	char buf[10240];
	ZeroMemory(buf, sizeof(buf));
	ItemAttrib item;
	ZeroMemory(&item, sizeof(ItemAttrib));
	char szID[10];
	char szValue[MAX_PATH];

	std::vector<int> xSections;
	GetIniFileSections(_pszFilename, xSections);

	//	Monster表
	if(strlen(_pszFilename) != 0)
	{
		sprintf(buf, "create table Monsters (id integer primary key,"\
			"name varchar(20),"\
			"lucky char,"\
			"curse char,"\
			"hide char,"\
			"accuracy char,"\
			"atkspeed char,"\
			"atkpalsy char,"\
			"atkpois char,"\
			"movespeed char,"\
			"weight char,"\
			"reqtype char,"\
			"reqvalue char,"\
			"sex char,"\
			"type char,"\
			"maxdc integer,"\
			"dc integer,"\
			"maxac integer,"\
			"ac integer,"\
			"maxmac integer,"\
			"mac integer,"\
			"maxsc integer,"\
			"sc integer,"\
			"maxmc integer,"\
			"mc integer,"\
			"maxhp integer,"\
			"hp integer,"\
			"maxmp integer,"\
			"mp integer,"\
			"maxexpr integer,"\
			"expr integer,"\
			"level integer,"\
			"extra integer,"\
			"tex integer,"\
			"price integer)");

		if(SQLITE_OK != sqlite3_exec(_pSql, buf, NULL, NULL, &pErr))
		{
			printf(pErr);
			printf("\n创建表Monsters失败\n");
			sqlite3_free(pErr);
			return false;
		}

		int nRet = sqlite3_exec(_pSql, "Begin transaction", 0, 0, &pErr);
		if(nRet != SQLITE_OK)
		{
			printf("%s\n", pErr);
			sqlite3_free(pErr);
			return false;
		}

		for(int i = 0; i < xSections.size(); ++i)
		{
			sprintf(szID, "%d", xSections[i]);
			::GetPrivateProfileString(szID, "name", "", szValue, sizeof(szValue), _pszFilename);
			if(strlen(szValue) == 0)
			{
				//	无效的NAME
				continue;
			}

			//	name
			item.id = xSections[i];
			strcpy(item.name, szValue);

			item.lucky			 = ::GetPrivateProfileInt(szID, "lucky", 0, _pszFilename);
			item.curse			 = ::GetPrivateProfileInt(szID, "curse", 0, _pszFilename);
			item.hide			 = ::GetPrivateProfileInt(szID, "hide", 0, _pszFilename);
			item.accuracy		 = ::GetPrivateProfileInt(szID, "accuracy", 0, _pszFilename);
			item.atkSpeed		 = ::GetPrivateProfileInt(szID, "atkspeed", 0, _pszFilename);
			item.atkPalsy		 = ::GetPrivateProfileInt(szID, "atkpalsy", 0, _pszFilename);
			item.atkPois		 = ::GetPrivateProfileInt(szID, "atkPois", 0, _pszFilename);
			item.moveSpeed		 = ::GetPrivateProfileInt(szID, "movespeed", 0, _pszFilename);
			item.weight			 = ::GetPrivateProfileInt(szID, "weight", 0, _pszFilename);
			item.reqType		 = ::GetPrivateProfileInt(szID, "reqtype", 0, _pszFilename);
			item.reqValue		 = ::GetPrivateProfileInt(szID, "reqvalue", 0, _pszFilename);
			item.sex			 = ::GetPrivateProfileInt(szID, "sex", 0, _pszFilename);
			item.type			 = ::GetPrivateProfileInt(szID, "type", 0, _pszFilename);
			item.maxDC			 = ::GetPrivateProfileInt(szID, "maxdc", 0, _pszFilename);
			item.DC				 = ::GetPrivateProfileInt(szID, "dc", 0, _pszFilename);
			item.maxAC			 = ::GetPrivateProfileInt(szID, "maxac", 0, _pszFilename);
			item.AC				 = ::GetPrivateProfileInt(szID, "ac", 0, _pszFilename);
			item.maxMAC			 = ::GetPrivateProfileInt(szID, "maxmac", 0, _pszFilename);
			item.MAC			 = ::GetPrivateProfileInt(szID, "mac", 0, _pszFilename);
			item.maxSC			 = ::GetPrivateProfileInt(szID, "maxsc", 0, _pszFilename);
			item.SC				 = ::GetPrivateProfileInt(szID, "sc", 0, _pszFilename);
			item.maxMC			 = ::GetPrivateProfileInt(szID, "maxmc", 0, _pszFilename);
			item.MC				 = ::GetPrivateProfileInt(szID, "mc", 0, _pszFilename);
			item.maxHP			 = ::GetPrivateProfileInt(szID, "maxhp", 0, _pszFilename);
			item.HP				 = ::GetPrivateProfileInt(szID, "hp", 0, _pszFilename);
			item.maxMP			 = ::GetPrivateProfileInt(szID, "maxmp", 0, _pszFilename);
			item.MP				 = ::GetPrivateProfileInt(szID, "mp", 0, _pszFilename);
			item.maxEXPR		 = ::GetPrivateProfileInt(szID, "maxexpr", 0, _pszFilename);
			item.EXPR			 = ::GetPrivateProfileInt(szID, "expr", 0, _pszFilename);
			item.level			 = ::GetPrivateProfileInt(szID, "level", 0, _pszFilename);
			item.extra			 = ::GetPrivateProfileInt(szID, "extra", 0, _pszFilename);
			item.tex			 = ::GetPrivateProfileInt(szID, "tex", 0, _pszFilename);
			item.price			 = ::GetPrivateProfileInt(szID, "price", 0, _pszFilename);

			sprintf(buf, "insert into Monsters values(%d,"\
				"'%s',"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d)",
				item.id,
				item.name,
				item.lucky,
				item.curse,
				item.hide,
				item.accuracy,
				item.atkSpeed,
				item.atkPalsy,
				item.atkPois,
				item.moveSpeed,
				item.weight,
				item.reqType,
				item.reqValue,
				item.sex,
				item.type,
				item.maxDC,
				item.DC,
				item.maxAC,
				item.AC,
				item.maxMAC,
				item.MAC,
				item.maxSC,
				item.SC,
				item.maxMC,
				item.MC,
				item.maxHP,
				item.HP,
				item.maxMP,
				item.MP,
				item.maxEXPR,
				item.EXPR,
				item.level,
				item.extra,
				item.tex,
				item.price);

			if(SQLITE_OK != sqlite3_exec(_pSql, buf, NULL, NULL, &pErr))
			{
				printf(pErr);
				printf("\nID[%d]写入表Monsters失败\n", item.id);
				sqlite3_free(pErr);
				return false;
			}
			else
			{
				printf("成功写入ID[%d]进表Monsters\n", item.id);
			}
		}

		nRet = sqlite3_exec(_pSql, "Commit transaction", NULL, 0, &pErr);
		if(nRet != SQLITE_OK)
		{
			printf("%s\n", pErr);
			sqlite3_free(pErr);
			return false;
		}
		else
		{
			printf("transaction Monsters OK!\n");
		}
	}

	return true;
}

bool Ini2SqlMagic(sqlite3* _pSql, const char* _pszFilename)
{
	char* pErr = NULL;
	char buf[10240];
	ZeroMemory(buf, sizeof(buf));
	//ItemAttrib item;
	//ZeroMemory(&item, sizeof(ItemAttrib));
	char szID[10];
	char szValue[MAX_PATH];

	std::vector<int> xSections;
	GetIniFileSections(_pszFilename, xSections);

	//	Monster表
	if(strlen(_pszFilename) != 0)
	{
		sprintf(buf, "create table Magics (id integer primary key,"\
			"level1 integer,"\
			"level2 integer,"\
			"level3 integer,"\
			"level4 integer,"\
			"level5 integer,"\
			"level6 integer,"\
			"level7 integer,"\
			"job char,"\
			"multiple char,"\
			"increase char,"\
			"basecost char,"\
			"delay integer)");

		if(SQLITE_OK != sqlite3_exec(_pSql, buf, NULL, NULL, &pErr))
		{
			printf(pErr);
			printf("\n创建表Magics失败\n");
			sqlite3_free(pErr);
			return false;
		}

		int nRet = sqlite3_exec(_pSql, "Begin transaction", 0, 0, &pErr);
		if(nRet != SQLITE_OK)
		{
			printf("%s\n", pErr);
			sqlite3_free(pErr);
			return false;
		}

		for(int i = 0; i < xSections.size(); ++i)
		{
			sprintf(szID, "%d", xSections[i]);
			::GetPrivateProfileString(szID, "id", "", szValue, sizeof(szValue), _pszFilename);
			if(strlen(szValue) == 0)
			{
				//	无效的NAME
				continue;
			}

			//	name
			//item.id = xSections[i];
			//strcpy(item.name, szValue);
			int nID = xSections[i];
			int nLevel[7] = {0};
			char job = 0;
			char increase = 0;
			char basecost = 0;
			char multiple = 0;
			DWORD dwDelay = 0;

			nLevel[0] = ::GetPrivateProfileInt(szID, "level1", 0, _pszFilename);
			nLevel[1] = ::GetPrivateProfileInt(szID, "level2", 0, _pszFilename);
			nLevel[2] = ::GetPrivateProfileInt(szID, "level3", 0, _pszFilename);
			nLevel[3] = ::GetPrivateProfileInt(szID, "level4", 0, _pszFilename);
			nLevel[4] = ::GetPrivateProfileInt(szID, "level5", 0, _pszFilename);
			nLevel[5] = ::GetPrivateProfileInt(szID, "level6", 0, _pszFilename);
			nLevel[6] = ::GetPrivateProfileInt(szID, "level7", 0, _pszFilename);

			job = ::GetPrivateProfileInt(szID, "job", 0, _pszFilename);
			basecost = ::GetPrivateProfileInt(szID, "basecost", 0, _pszFilename);
			multiple = ::GetPrivateProfileInt(szID, "multiple", 0, _pszFilename);
			increase = ::GetPrivateProfileInt(szID, "increase", 0, _pszFilename);

			dwDelay = ::GetPrivateProfileInt(szID, "delay", 0, _pszFilename);

			sprintf(buf, "insert into Magics values(%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d,"\
				"%d)",
				nID,
				nLevel[0],
				nLevel[1],
				nLevel[2],
				nLevel[3],
				nLevel[4],
				nLevel[5],
				nLevel[6],
				job,
				multiple,
				increase,
				basecost,
				dwDelay);

			if(SQLITE_OK != sqlite3_exec(_pSql, buf, NULL, NULL, &pErr))
			{
				printf(pErr);
				printf("\nID[%d]写入表Magics失败\n", nID);
				sqlite3_free(pErr);
				return false;
			}
			else
			{
				printf("成功写入ID[%d]进表Magics\n", nID);
			}
		}

		nRet = sqlite3_exec(_pSql, "Commit transaction", NULL, 0, &pErr);
		if(nRet != SQLITE_OK)
		{
			printf("%s\n", pErr);
			sqlite3_free(pErr);
			return false;
		}
		else
		{
			printf("transaction Magics OK!\n");
		}
	}

	return true;
}

int IniFileTransToSqlFile(const char* lpszCfg, sqlite3* pSql)
{
	char szValue[MAX_PATH];
	/*char szItemPath[MAX_PATH] = {0};
	char szMonsPath[MAX_PATH] = {0};
	char szDropItem[MAX_PATH] = {0};*/
	int nItemNum = 0;
	int nMonsterNum = 0;
	char* pErr = NULL;

	DWORD dwRet = ::GetPrivateProfileString("INPUT", "ITEMFILE", "", szValue, sizeof(szValue), lpszCfg);
	if(strlen(szValue) != 0)
	{
		strcpy(szItemPath, szValue);
	}
	dwRet = ::GetPrivateProfileString("INPUT", "MONSTERFILE", "", szValue, sizeof(szValue), lpszCfg);
	if(strlen(szValue) != 0)
	{
		strcpy(szMonsPath, szValue);
	}
	::GetPrivateProfileString("INPUT", "DROPITEMFILE", "", szValue, sizeof(szValue), lpszCfg);
	if(strlen(szValue) != 0)
	{
		strcpy(szDropItem, szValue);
	}
	::GetPrivateProfileString("INPUT", "MAGICFILE", "", szValue, sizeof(szValue), lpszCfg);
	if(strlen(szValue) != 0)
	{
		strcpy(szMagicsPath, szValue);
	}
	nItemNum = ::GetPrivateProfileInt("INPUT", "ITEMSUM", 0, lpszCfg);
	nMonsterNum = ::GetPrivateProfileInt("INPUT", "MONSTERSUM", 0, lpszCfg);

	Ini2SqlItems(pSql, szItemPath);
	Ini2SqlMonsters(pSql, szMonsPath);
	Ini2SqlDropItem(pSql, szDropItem);
	Ini2SqlMagic(pSql, szMagicsPath);

	/*
	if(strlen(szItemPath) == 0 &&
			strlen(szMonsPath) == 0)
		{
			return -1;
		}
	
		char buf[10240];
		ZeroMemory(buf, sizeof(buf));
		ItemAttrib item;
		ZeroMemory(&item, sizeof(ItemAttrib));
		char szID[10];
	
		//	Item表
		if(strlen(szItemPath) != 0)
		{
			sprintf(buf, "create table Items (id integer primary key,"\
				"name varchar(20),"\
				"lucky char,"\
				"curse char,"\
				"hide char,"\
				"accuracy char,"\
				"atkspeed char,"\
				"atkpalsy char,"\
				"atkpois char,"\
				"movespeed char,"\
				"weight char,"\
				"reqtype char,"\
				"reqvalue char,"\
				"sex char,"\
				"type char,"\
				"maxdc integer,"\
				"dc integer,"\
				"maxac integer,"\
				"ac integer,"\
				"maxmac integer,"\
				"mac integer,"\
				"maxsc integer,"\
				"sc integer,"\
				"maxmc integer,"\
				"mc integer,"\
				"maxhp integer,"\
				"hp integer,"\
				"maxmp integer,"\
				"mp integer,"\
				"maxexpr integer,"\
				"expr integer,"\
				"level integer,"\
				"extra integer,"\
				"tex integer)");
	
			if(SQLITE_OK != sqlite3_exec(pSql, buf, NULL, NULL, &pErr))
			{
				printf(pErr);
				printf("\n创建表Items失败\n");
				sqlite3_free(pErr);
				return -2;
			}
	
			int nRet = sqlite3_exec(pSql, "Begin transaction", 0, 0, &pErr);
			if(nRet != SQLITE_OK)
			{
				printf("%s\n", pErr);
				sqlite3_free(pErr);
				return -3;
			}
	
			for(int i = 0; i < nItemNum; ++i)
			{
				sprintf(szID, "%d", i);
				::GetPrivateProfileString(szID, "name", "", szValue, sizeof(szValue), szItemPath);
				if(strlen(szValue) == 0)
				{
					//	无效的NAME
					continue;
				}
	
				//	name
				item.id = i;
				strcpy(item.name, szValue);
	
				item.lucky			 = ::GetPrivateProfileInt(szID, "lucky", 0, szItemPath);
				item.curse			 = ::GetPrivateProfileInt(szID, "curse", 0, szItemPath);
				item.hide			 = ::GetPrivateProfileInt(szID, "hide", 0, szItemPath);
				item.accuracy		 = ::GetPrivateProfileInt(szID, "accuracy", 0, szItemPath);
				item.atkSpeed		 = ::GetPrivateProfileInt(szID, "atkspeed", 0, szItemPath);
				item.atkPalsy		 = ::GetPrivateProfileInt(szID, "atkpalsy", 0, szItemPath);
				item.atkPois		 = ::GetPrivateProfileInt(szID, "atkPois", 0, szItemPath);
				item.moveSpeed		 = ::GetPrivateProfileInt(szID, "movespeed", 0, szItemPath);
				item.weight			 = ::GetPrivateProfileInt(szID, "weight", 0, szItemPath);
				item.reqType		 = ::GetPrivateProfileInt(szID, "reqtype", 0, szItemPath);
				item.reqValue		 = ::GetPrivateProfileInt(szID, "reqvalue", 0, szItemPath);
				item.sex			 = ::GetPrivateProfileInt(szID, "sex", 0, szItemPath);
				item.type			 = ::GetPrivateProfileInt(szID, "type", 0, szItemPath);
				item.maxDC			 = ::GetPrivateProfileInt(szID, "maxdc", 0, szItemPath);
				item.DC				 = ::GetPrivateProfileInt(szID, "dc", 0, szItemPath);
				item.maxAC			 = ::GetPrivateProfileInt(szID, "maxac", 0, szItemPath);
				item.AC				 = ::GetPrivateProfileInt(szID, "ac", 0, szItemPath);
				item.maxMAC			 = ::GetPrivateProfileInt(szID, "maxmac", 0, szItemPath);
				item.MAC			 = ::GetPrivateProfileInt(szID, "mac", 0, szItemPath);
				item.maxSC			 = ::GetPrivateProfileInt(szID, "maxsc", 0, szItemPath);
				item.SC				 = ::GetPrivateProfileInt(szID, "sc", 0, szItemPath);
				item.maxMC			 = ::GetPrivateProfileInt(szID, "maxmc", 0, szItemPath);
				item.MC				 = ::GetPrivateProfileInt(szID, "mc", 0, szItemPath);
				item.maxHP			 = ::GetPrivateProfileInt(szID, "maxhp", 0, szItemPath);
				item.HP				 = ::GetPrivateProfileInt(szID, "hp", 0, szItemPath);
				item.maxMP			 = ::GetPrivateProfileInt(szID, "maxmp", 0, szItemPath);
				item.MP				 = ::GetPrivateProfileInt(szID, "mp", 0, szItemPath);
				item.maxEXPR		 = ::GetPrivateProfileInt(szID, "maxexpr", 0, szItemPath);
				item.EXPR			 = ::GetPrivateProfileInt(szID, "expr", 0, szItemPath);
				item.level			 = ::GetPrivateProfileInt(szID, "level", 0, szItemPath);
				item.extra			 = ::GetPrivateProfileInt(szID, "extra", 0, szItemPath);
				item.tex			 = ::GetPrivateProfileInt(szID, "tex", 0, szItemPath);
	
				sprintf(buf, "insert into Items values(%d,"\
					"'%s',"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d)",
					item.id,
					item.name,
					item.lucky,
					item.curse,
					item.hide,
					item.accuracy,
					item.atkSpeed,
					item.atkPalsy,
					item.atkPois,
					item.moveSpeed,
					item.weight,
					item.reqType,
					item.reqValue,
					item.sex,
					item.type,
					item.maxDC,
					item.DC,
					item.maxAC,
					item.AC,
					item.maxMAC,
					item.MAC,
					item.maxSC,
					item.SC,
					item.maxMC,
					item.MC,
					item.maxHP,
					item.HP,
					item.maxMP,
					item.MP,
					item.maxEXPR,
					item.EXPR,
					item.level,
					item.extra,
					item.tex);
	
				if(SQLITE_OK != sqlite3_exec(pSql, buf, NULL, NULL, &pErr))
				{
					printf(pErr);
					printf("ID[%d]写入表Item失败\n", item.id);
					sqlite3_free(pErr);
					return -3;
				}
				else
				{
					printf("成功写入ID[%d]进表Item\n", item.id);
				}
			}
	
			nRet = sqlite3_exec(pSql, "Commit transaction", NULL, 0, &pErr);
			if(nRet != SQLITE_OK)
			{
				printf("%s\n", pErr);
				sqlite3_free(pErr);
				return -4;
			}
			else
			{
				printf("transaction Items OK!\n");
			}
		}
	
		//	Monster表
		if(strlen(szMonsPath) != 0)
		{
			sprintf(buf, "create table Monsters (id integer primary key,"\
				"name varchar(20),"\
				"lucky char,"\
				"curse char,"\
				"hide char,"\
				"accuracy char,"\
				"atkspeed char,"\
				"atkpalsy char,"\
				"atkpois char,"\
				"movespeed char,"\
				"weight char,"\
				"reqtype char,"\
				"reqvalue char,"\
				"sex char,"\
				"type char,"\
				"maxdc integer,"\
				"dc integer,"\
				"maxac integer,"\
				"ac integer,"\
				"maxmac integer,"\
				"mac integer,"\
				"maxsc integer,"\
				"sc integer,"\
				"maxmc integer,"\
				"mc integer,"\
				"maxhp integer,"\
				"hp integer,"\
				"maxmp integer,"\
				"mp integer,"\
				"maxexpr integer,"\
				"expr integer,"\
				"level integer,"\
				"extra integer,"\
				"tex integer)");
	
			if(SQLITE_OK != sqlite3_exec(pSql, buf, NULL, NULL, &pErr))
			{
				printf(pErr);
				printf("\n创建表Monsters失败\n");
				sqlite3_free(pErr);
				return -2;
			}
	
			int nRet = sqlite3_exec(pSql, "Begin transaction", 0, 0, &pErr);
			if(nRet != SQLITE_OK)
			{
				printf("%s\n", pErr);
				sqlite3_free(pErr);
				return -3;
			}
	
			for(int i = 0; i < nMonsterNum; ++i)
			{
				sprintf(szID, "%d", i);
				::GetPrivateProfileString(szID, "name", "", szValue, sizeof(szValue), szMonsPath);
				if(strlen(szValue) == 0)
				{
					//	无效的NAME
					continue;
				}
	
				//	name
				item.id = i;
				strcpy(item.name, szValue);
	
				item.lucky			 = ::GetPrivateProfileInt(szID, "lucky", 0, szMonsPath);
				item.curse			 = ::GetPrivateProfileInt(szID, "curse", 0, szMonsPath);
				item.hide			 = ::GetPrivateProfileInt(szID, "hide", 0, szMonsPath);
				item.accuracy		 = ::GetPrivateProfileInt(szID, "accuracy", 0, szMonsPath);
				item.atkSpeed		 = ::GetPrivateProfileInt(szID, "atkspeed", 0, szMonsPath);
				item.atkPalsy		 = ::GetPrivateProfileInt(szID, "atkpalsy", 0, szMonsPath);
				item.atkPois		 = ::GetPrivateProfileInt(szID, "atkPois", 0, szMonsPath);
				item.moveSpeed		 = ::GetPrivateProfileInt(szID, "movespeed", 0, szMonsPath);
				item.weight			 = ::GetPrivateProfileInt(szID, "weight", 0, szMonsPath);
				item.reqType		 = ::GetPrivateProfileInt(szID, "reqtype", 0, szMonsPath);
				item.reqValue		 = ::GetPrivateProfileInt(szID, "reqvalue", 0, szMonsPath);
				item.sex			 = ::GetPrivateProfileInt(szID, "sex", 0, szMonsPath);
				item.type			 = ::GetPrivateProfileInt(szID, "type", 0, szMonsPath);
				item.maxDC			 = ::GetPrivateProfileInt(szID, "maxdc", 0, szMonsPath);
				item.DC				 = ::GetPrivateProfileInt(szID, "dc", 0, szMonsPath);
				item.maxAC			 = ::GetPrivateProfileInt(szID, "maxac", 0, szMonsPath);
				item.AC				 = ::GetPrivateProfileInt(szID, "ac", 0, szMonsPath);
				item.maxMAC			 = ::GetPrivateProfileInt(szID, "maxmac", 0, szMonsPath);
				item.MAC			 = ::GetPrivateProfileInt(szID, "mac", 0, szMonsPath);
				item.maxSC			 = ::GetPrivateProfileInt(szID, "maxsc", 0, szMonsPath);
				item.SC				 = ::GetPrivateProfileInt(szID, "sc", 0, szMonsPath);
				item.maxMC			 = ::GetPrivateProfileInt(szID, "maxmc", 0, szMonsPath);
				item.MC				 = ::GetPrivateProfileInt(szID, "mc", 0, szMonsPath);
				item.maxHP			 = ::GetPrivateProfileInt(szID, "maxhp", 0, szMonsPath);
				item.HP				 = ::GetPrivateProfileInt(szID, "hp", 0, szMonsPath);
				item.maxMP			 = ::GetPrivateProfileInt(szID, "maxmp", 0, szMonsPath);
				item.MP				 = ::GetPrivateProfileInt(szID, "mp", 0, szMonsPath);
				item.maxEXPR		 = ::GetPrivateProfileInt(szID, "maxexpr", 0, szMonsPath);
				item.EXPR			 = ::GetPrivateProfileInt(szID, "expr", 0, szMonsPath);
				item.level			 = ::GetPrivateProfileInt(szID, "level", 0, szMonsPath);
				item.extra			 = ::GetPrivateProfileInt(szID, "extra", 0, szMonsPath);
				item.tex			 = ::GetPrivateProfileInt(szID, "tex", 0, szMonsPath);
	
				sprintf(buf, "insert into Monsters values(%d,"\
					"'%s',"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d,"\
					"%d)",
					item.id,
					item.name,
					item.lucky,
					item.curse,
					item.hide,
					item.accuracy,
					item.atkSpeed,
					item.atkPalsy,
					item.atkPois,
					item.moveSpeed,
					item.weight,
					item.reqType,
					item.reqValue,
					item.sex,
					item.type,
					item.maxDC,
					item.DC,
					item.maxAC,
					item.AC,
					item.maxMAC,
					item.MAC,
					item.maxSC,
					item.SC,
					item.maxMC,
					item.MC,
					item.maxHP,
					item.HP,
					item.maxMP,
					item.MP,
					item.maxEXPR,
					item.EXPR,
					item.level,
					item.extra,
					item.tex);
	
				if(SQLITE_OK != sqlite3_exec(pSql, buf, NULL, NULL, &pErr))
				{
					printf(pErr);
					printf("\nID[%d]写入表Monsters失败\n", item.id);
					sqlite3_free(pErr);
					return -3;
				}
				else
				{
					printf("成功写入ID[%d]进表Monsters\n", item.id);
				}
			}
	
			nRet = sqlite3_exec(pSql, "Commit transaction", NULL, 0, &pErr);
			if(nRet != SQLITE_OK)
			{
				printf("%s\n", pErr);
				sqlite3_free(pErr);
				return -4;
			}
			else
			{
				printf("transaction Monsters OK!\n");
			}
		}
	
		if(strlen(szDropItem))
		{
			Ini2SqlDropItem(pSql, szDropItem);
		}*/
	

	printf("数据库文件生成成功\n");
	return 1;
}


int SqlFileTransToIniFile(const char* lpszCfg, sqlite3* pSql)
{
	char szItem[MAX_PATH] = {0};
	char szMonster[MAX_PATH] = {0};
	char szValue[MAX_PATH] = {0};

	::GetPrivateProfileString("OUTPUT", "ITEMFILE", "", szValue, sizeof(szValue), lpszCfg);
	if(strlen(szValue) != 0)
	{
		strcpy(szItem, szValue);
	}
	::GetPrivateProfileString("OUTPUT", "MONSTERFILE", "", szValue, sizeof(szValue), lpszCfg);
	if(strlen(szValue) != 0)
	{
		strcpy(szMonster, szValue);
	}

	if(strlen(szItem) == 0 &&
		strlen(szMonster) == 0)
	{
		return -1;
	}

	char* pErr = NULL;
	if(strlen(szItem))
	{
		if(SQLITE_OK != sqlite3_exec(pSql, "select * from Items", DBCallback, szItem, &pErr))
		{
			printf(pErr);
			sqlite3_free(pErr);
			printf("\n执行select * from Items 失败!\n");
			return -2;
		}
		else
		{
			//	Running
			printf("%s写入完毕\n", szItem);
		}
	}

	if(strlen(szMonster))
	{
		if(SQLITE_OK != sqlite3_exec(pSql, "select * from Monsters", DBCallback, szMonster, &pErr))
		{
			printf(pErr);
			sqlite3_free(pErr);
			printf("\n执行select * from Monsters 失败!\n");
			return -2;
		}
		else
		{
			//	Running
			printf("%s写入完毕\n", szMonster);
		}
	}

	return 0;
}

int DBCallback(void* _param, int _count, char** _value, char** _name)
{
	//ItemAttrib* pItem = (ItemAttrib*)_param;
	const char* szOutputFile = (const char*)_param;
	ItemAttrib item;
	ItemAttrib* pItem = &item;

	assert(_count == 34);

	//	初始化
	pItem->id		 = atoi(_value[0]);
	strcpy(pItem->name, _value[1]);
	pItem->lucky	 = atoi(_value[2]);
	pItem->curse	 = atoi(_value[3]);
	pItem->hide		 = atoi(_value[4]);
	pItem->accuracy	 = atoi(_value[5]);
	pItem->atkSpeed	 = atoi(_value[6]);
	pItem->atkPalsy  = atoi(_value[7]);
	pItem->atkPois  = atoi(_value[8]);
	pItem->moveSpeed  = atoi(_value[9]);
	pItem->weight  = atoi(_value[10]);
	pItem->reqType  = atoi(_value[11]);
	pItem->reqValue  = atoi(_value[12]);
	pItem->sex  = atoi(_value[13]);
	pItem->type  = atoi(_value[14]);
	pItem->maxDC  = atoi(_value[15]);
	pItem->DC  = atoi(_value[16]);
	pItem->maxAC  = atoi(_value[17]);
	pItem->AC  = atoi(_value[18]);
	pItem->maxMAC  = atoi(_value[19]);
	pItem->MAC  = atoi(_value[20]);
	pItem->maxSC  = atoi(_value[21]);
	pItem->SC  = atoi(_value[22]);
	pItem->maxMC  = atoi(_value[23]);
	pItem->MC  = atoi(_value[24]);
	pItem->maxHP  = atoi(_value[25]);
	pItem->HP  = atoi(_value[26]);
	pItem->maxMP  = atoi(_value[27]);
	pItem->MP  = atoi(_value[28]);
	pItem->maxEXPR	= atoi(_value[29]);
	pItem->EXPR  = atoi(_value[30]);
	pItem->level  = atoi(_value[31]);
	pItem->extra  = atoi(_value[32]);
	pItem->tex  = atoi(_value[33]);

	//	写文件
	//	获取了 写入INI
	char szID[10];
	sprintf(szID, "%d", item.id);
	char szValue[MAX_PATH];

	sprintf(szValue, "%s", szID);
	if(!::WritePrivateProfileString(szID, "id", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%s", item.name);
	if(!::WritePrivateProfileString(szID, "name", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.lucky);
	if(!::WritePrivateProfileString(szID, "lucky", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.curse);
	if(!::WritePrivateProfileString(szID, "curse", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.hide);
	if(!::WritePrivateProfileString(szID, "hide", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.accuracy);
	if(!::WritePrivateProfileString(szID, "accuracy", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.atkSpeed);
	if(!::WritePrivateProfileString(szID, "atkSpeed", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.atkPalsy);
	if(!::WritePrivateProfileString(szID, "atkPalsy", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.atkPois);
	if(!::WritePrivateProfileString(szID, "atkPois", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.moveSpeed);
	if(!::WritePrivateProfileString(szID, "moveSpeed", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.weight);
	if(!::WritePrivateProfileString(szID, "weight", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.reqType);
	if(!::WritePrivateProfileString(szID, "reqType", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.reqValue);
	if(!::WritePrivateProfileString(szID, "reqValue", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.sex);
	if(!::WritePrivateProfileString(szID, "sex", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.type);
	if(!::WritePrivateProfileString(szID, "type", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.maxDC);
	if(!::WritePrivateProfileString(szID, "maxDC", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.DC);
	if(!::WritePrivateProfileString(szID, "DC", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.maxAC);
	if(!::WritePrivateProfileString(szID, "maxAC", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.AC);
	if(!::WritePrivateProfileString(szID, "AC", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.maxMAC);
	if(!::WritePrivateProfileString(szID, "maxMAC", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.MAC);
	if(!::WritePrivateProfileString(szID, "MAC", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.maxSC);
	if(!::WritePrivateProfileString(szID, "maxSC", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.SC);
	if(!::WritePrivateProfileString(szID, "SC", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.maxMC);
	if(!::WritePrivateProfileString(szID, "maxMC", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.MC);
	if(!::WritePrivateProfileString(szID, "MC", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.maxHP);
	if(!::WritePrivateProfileString(szID, "maxHP", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.HP);
	if(!::WritePrivateProfileString(szID, "HP", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.maxMP);
	if(!::WritePrivateProfileString(szID, "maxMP", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.MP);
	if(!::WritePrivateProfileString(szID, "MP", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.maxEXPR);
	if(!::WritePrivateProfileString(szID, "maxEXPR", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.EXPR);
	if(!::WritePrivateProfileString(szID, "EXPR", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.level);
	if(!::WritePrivateProfileString(szID, "level", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.extra);
	if(!::WritePrivateProfileString(szID, "extra", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	sprintf(szValue, "%d", item.tex);
	if(!::WritePrivateProfileString(szID, "tex", szValue, szOutputFile))
	{
		printf("写入%s文件失败,key:%s", szOutputFile, szValue);
		return -1;
	}

	printf("成功写入ID[%s]的信息记录[%s]\n", szID, szOutputFile);
	return 0;
}


int IniFileTransToSqlFileMain()
{
	char szValue[MAX_PATH];
	char szCfgFile[MAX_PATH];

	SKNL3GetCurrentAppPath(szCfgFile, sizeof(szCfgFile));
	sprintf(szCfgFile, "%s%s", szCfgFile, "inisql.ini");

	DWORD dwRet = 0;
	::GetPrivateProfileString("OUTPUT", "SQLFILE", "", szValue, sizeof(szValue), szCfgFile);
	if(strlen(szValue) == 0)
	{
		printf("无法获取SQL输出文件信息In file[%s]\n", szCfgFile);
		return -1;
	}

	if(PathFileExists(szValue))
	{
		DeleteFile(szValue);
	}
	sqlite3* sql;
	if(SQLITE_OK != sqlite3_open(szValue, &sql))
	{
		printf("无法创建%s\n", szValue);
		return -2;
	}

	::GetPrivateProfileString("OUTPUT", "SQLPASSWORD", "", szValue, sizeof(szValue), szCfgFile);
	if(strlen(szValue) != 0)
	{
		printf("设置SQL密码为%s\n", szValue);
		if(SQLITE_OK != sqlite3_key(sql, szValue, strlen(szValue)))
		{
			printf("设置SQL密码失败\n");
			sqlite3_close(sql);
			return -3;
		}
	}

	IniFileTransToSqlFile(szCfgFile, sql);
	sqlite3_close(sql);
	return 0;
}

int SqlFileTransToIniFileMain()
{
	char szValue[MAX_PATH];
	char szCfgFile[MAX_PATH];

	SKNL3GetCurrentAppPath(szCfgFile, sizeof(szCfgFile));
	sprintf(szCfgFile, "%s%s", szCfgFile, "sqlini.ini");

	DWORD dwRet = 0;
	::GetPrivateProfileString("INPUT", "SQLFILE", "", szValue, sizeof(szValue), szCfgFile);
	if(strlen(szValue) == 0)
	{
		printf("无法获取SQL输入文件信息In file[%s]\n", szCfgFile);
		return -1;
	}

	sqlite3* sql;
	if(SQLITE_OK != sqlite3_open(szValue, &sql))
	{
		printf("无法打开%s\n", szValue);
		return -2;
	}

	::GetPrivateProfileString("INPUT", "SQLPASSWORD", "", szValue, sizeof(szValue), szCfgFile);
	if(strlen(szValue) != 0)
	{
		printf("设置SQL密码为%s\n", szValue);
		if(SQLITE_OK != sqlite3_key(sql, szValue, strlen(szValue)))
		{
			printf("校验SQL密码失败\n");
			sqlite3_close(sql);
			return -3;
		}
	}

	SqlFileTransToIniFile(szCfgFile, sql);
	sqlite3_close(sql);
	return 0;
}

bool GetNameToIDMap(const char* _pszFile, std::map<std::string, int>& _xOutput)
{
	std::vector<int> xSections;
	GetIniFileSections(_pszFile, xSections);

	if(xSections.empty())
	{
		return false;
	}

	char szBuf[MAX_PATH];
	char szKey[10];
	std::string xName;

	for(int i = 0; i < xSections.size(); ++i)
	{
		sprintf(szKey, "%d", xSections[i]);
		GetPrivateProfileString(szKey, "name", "", szBuf, MAX_PATH - 1, _pszFile);
		if(strlen(szBuf) > 0)
		{
			if(0 != strcmp(szBuf, "金币"))
			{
				xName = szBuf;
				if(_xOutput.find(xName) == _xOutput.end())
				{
					_xOutput.insert(std::make_pair(xName, xSections[i]));
				}
			}
		}
	}

	if(_xOutput.size() > 0)
	{
		for(int i = 0; i < 5; ++i)
		{
			sprintf(szBuf, "金币%d", i + 1);
			xName = szBuf;
			_xOutput.insert(std::make_pair(xName, 112 + i));
		}
	}

	return true;
}



/*
*	获得文件大小
*/
/*
DWORD SKNL3GetFileSize(LPCTSTR lpszFileName)
{
	if(lpszFileName == NULL)
	{
		return -1;
	}

	if(lstrlen(lpszFileName) > MAX_PATH)
	{
		return -1;
	}

	WIN32_FIND_DATA wfd;
	HANDLE hFind = ::FindFirstFile(lpszFileName, &wfd);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	::FindClose(hFind);

	if(wfd.nFileSizeHigh != 0)
	{
		return -1;
	}

	return wfd.nFileSizeLow;
}
/ *
*	获得运行目录
* /
BOOL SKNL3GetCurrentAppPath(LPSTR lpBuffer, size_t _buffer_size)
{
	::GetModuleFileName(NULL, lpBuffer, _buffer_size);

	//	调整至 0
	int nStrLen = strlen(lpBuffer);
	lpBuffer += nStrLen;

	for(int i = 0; i < nStrLen; ++i)
	{
		if(*(lpBuffer--) == '\\')
		{
			lpBuffer += 2;
			*lpBuffer = '\0';
			return TRUE;
		}
	}

	return TRUE;
}*/