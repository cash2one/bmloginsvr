#include "WilExtractor.h"
#include "mirmap.h"
#include <io.h>
#include <Shlwapi.h>
#include <direct.h>
//////////////////////////////////////////////////////////////////////////
bool WilExtractorMain(const char* _pszFile)
{
	if(0 != _access(_pszFile, 0))
	{
		printf("%s not exist\n", _pszFile);
		return false;
	}

	char szBuf[200];
	char szSrc[MAX_PATH];
	int nLine = 0;

	FILE* pf = fopen(_pszFile, "r");
	if(pf == NULL)
	{
		printf("%s can not open\n", _pszFile);
		return false;
	}

	fgets(szBuf, sizeof(szBuf), pf);
	if(szBuf[0] != 'l')
	{
		printf("not list\n");
		return false;
	}
	if(szBuf[5] == 'o')
	{
		char szNumber[10];
		int nIndex = 0;
		for(int i = 7; i < strlen(szBuf); ++i)
		{
			if(szBuf[i] >= '0' &&
				szBuf[i] <= '9')
			{
				szNumber[nIndex++] = szBuf[i];
			}
			else if(szBuf[i] == ':')
			{
				szNumber[nIndex] = 0;
				break;
			}
		}

		int nNumber = atoi(szNumber);
		if(nNumber == 0)
		{
			strcpy(szSrc, "objects.wil");
		}
		else
		{
			sprintf(szSrc, "objects%d.wil", nNumber + 1);
		}

		SWilFile xFile;
		if(!xFile.Load(szSrc))
		{
			printf("%s can not find\n", szSrc);
			return false;
		}
		else
		{
			while(fgets(szBuf, sizeof(szBuf), pf))
			{
				nNumber = atoi(szBuf);
				sprintf(szSrc, ".\\img\\%06d.bmp",
					nNumber);
				if(xFile.ExtractBitmap8(szSrc, nNumber))
				{
					printf("%d ok\n", nNumber);
				}
			}
		}

		printf("ok\n");
		fclose(pf);
		return true;
	}
	else if(szBuf[5] == 'b')
	{
		int nNumber = 0;
		{
			sprintf(szSrc, "tiles.wil");
		}

		SWilFile xFile;
		if(!xFile.Load(szSrc))
		{
			printf("%s can not find\n", szSrc);
			return false;
		}
		else
		{
			while(fgets(szBuf, sizeof(szBuf), pf))
			{
				nNumber = atoi(szBuf);
				sprintf(szSrc, ".\\img\\%06d.bmp",
					nNumber);
				if(xFile.ExtractBitmap8(szSrc, nNumber))
				{
					printf("%d ok\n", nNumber);
				}
			}
		}

		printf("ok\n");
		fclose(pf);
		return true;
	}
	else if(szBuf[5] == 'm')
	{
		int nNumber = 0;
		{
			sprintf(szSrc, "smtiles.wil");
		}

		SWilFile xFile;
		if(!xFile.Load(szSrc))
		{
			printf("%s can not find\n", szSrc);
			return false;
		}
		else
		{
			while(fgets(szBuf, sizeof(szBuf), pf))
			{
				nNumber = atoi(szBuf);
				sprintf(szSrc, ".\\img\\%06d.bmp",
					nNumber);
				if(xFile.ExtractBitmap8(szSrc, nNumber))
				{
					printf("%d ok\n", nNumber);
				}
			}
		}

		printf("ok\n");
		fclose(pf);
		return true;
	}
	else
	{
		printf("not o and f and b\n");
	}
}

bool WilExtractOneFile(const char* _pszFile, int _nIndex, const char* _pszDestPath)
{
	if(0 != _access(_pszFile, 0))
	{
		printf("%s not exist\n", _pszFile);
		return false;
	}

	char szBuf[200];
	char szSrc[MAX_PATH];
	int nLine = 0;

	SWilFile xFile;
	if(!xFile.Load(_pszFile))
	{
		printf("%s can not find\n", _pszFile);
		return false;
	}
	else
	{
		int nNumber = _nIndex;
		sprintf(szSrc, "%s\\%06d.bmp",
			_pszDestPath, nNumber);
		if(xFile.ExtractBitmap8(szSrc, nNumber))
		{
			printf("%d ok\n", nNumber);
			return true;
		}
	}

	return false;
}

#ifdef _DEBUG
#define DEBUG_MSG(T)	MessageBox(NULL, T, "TIP", MB_OK)
#else
#define DEBUG_MSG(T)	
#endif

const char* GetLastCharOfString(const char* _pszSrc, char _cFind)
{
	for(int i = strlen(_pszSrc); i >= 0; --i)
	{
		if(_pszSrc[i] == _cFind)
		{
			return _pszSrc + i;
		}
	}

	return NULL;
}

bool WilExtractorMainEx(const char* _pszIniFile, const char* _pszSrcPath, const char* _pszDestPath)
{
	if(0 != _access(_pszIniFile, 0))
	{
		printf("%s not exist\n", _pszIniFile);
		return false;
	}

	char szParam[1024];
	sprintf(szParam, "[0]%s\n[1]%s\n[2]%s",
		_pszIniFile, _pszSrcPath, _pszDestPath);
	//DEBUG_MSG(szParam);
	//MessageBox(NULL, szParam, "tip", MB_OK);

	char szBuf[200];
	char szSrc[MAX_PATH];
	int nLine = 0;

	FILE* pf = fopen(_pszIniFile, "r");
	if(pf == NULL)
	{
		printf("%s can not open\n", _pszIniFile);
		DEBUG_MSG("ini can not open");
		return false;
	}

	fgets(szBuf, sizeof(szBuf), pf);
	if(szBuf[0] != 'l')
	{
		printf("not list\n");
		DEBUG_MSG("not list");
		return false;
	}

	//	conflict
	char szBakDirectory[MAX_PATH];
	const char* pszSubDirectory = GetLastCharOfString(_pszDestPath, '/') + 1;
	sprintf(szBakDirectory, "%s/../%s_conflict",
		_pszDestPath, pszSubDirectory);
	if(!PathFileExists(szBakDirectory))
	{
		mkdir(szBakDirectory);
	}

	if(szBuf[5] == 'o')
	{
		char szNumber[10];
		int nIndex = 0;
		for(int i = 7; i < strlen(szBuf); ++i)
		{
			if(szBuf[i] >= '0' &&
				szBuf[i] <= '9')
			{
				szNumber[nIndex++] = szBuf[i];
			}
			else if(szBuf[i] == ':')
			{
				szNumber[nIndex] = 0;
				break;
			}
		}

		int nNumber = atoi(szNumber);
		if(nNumber == 0)
		{
			//strcpy(szSrc, "objects.wil");
			sprintf(szSrc, "%s/objects.wil", _pszSrcPath);
		}
		else
		{
			sprintf(szSrc, "%s/objects%d.wil", _pszSrcPath, nNumber + 1);
		}

		SWilFile xFile;
		if(!xFile.Load(szSrc))
		{
			printf("%s can not find\n", szSrc);
			DEBUG_MSG("src can not find");
			return false;
		}
		else
		{
			while(fgets(szBuf, sizeof(szBuf), pf))
			{
				nNumber = atoi(szBuf);
				sprintf(szSrc, "%s/%06d.bmp",
					_pszDestPath, nNumber);

				BOOL bFileExist = PathFileExists(szSrc);
				if(bFileExist)
				{
					char szDestFile[MAX_PATH];
					sprintf(szDestFile, "%s/%06d_bak.bmp", szBakDirectory, nNumber);
					CopyFile(szSrc, szDestFile, FALSE);
				}

				if(xFile.ExtractBitmap8(szSrc, nNumber))
				{
					printf("%d ok\n", nNumber);

					if(bFileExist)
					{
						char szDestFile[MAX_PATH];
						sprintf(szDestFile, "%s/%06d.bmp", szBakDirectory, nNumber);
						CopyFile(szSrc, szDestFile, FALSE);
					}
				}
			}
		}

		printf("ok\n");
		fclose(pf);
		return true;
	}
	else if(szBuf[5] == 'b')
	{
		int nNumber = 0;
		{
			sprintf(szSrc, "%s/tiles.wil", _pszSrcPath);
		}

		SWilFile xFile;
		if(!xFile.Load(szSrc))
		{
			printf("%s can not find\n", szSrc);
			DEBUG_MSG("src not not find");
			return false;
		}
		else
		{
			while(fgets(szBuf, sizeof(szBuf), pf))
			{
				nNumber = atoi(szBuf);
				sprintf(szSrc, "%s/%06d.bmp",
					_pszDestPath, nNumber);

				BOOL bFileExist = PathFileExists(szSrc);
				if(bFileExist)
				{
					char szDestFile[MAX_PATH];
					sprintf(szDestFile, "%s/%06d_bak.bmp", szBakDirectory, nNumber);
					CopyFile(szSrc, szDestFile, FALSE);
				}

				if(xFile.ExtractBitmap8(szSrc, nNumber))
				{
					printf("%d ok\n", nNumber);

					if(bFileExist)
					{
						char szDestFile[MAX_PATH];
						sprintf(szDestFile, "%s/%06d.bmp", szBakDirectory, nNumber);
						CopyFile(szSrc, szDestFile, FALSE);
					}
				}
			}
		}

		printf("ok\n");
		fclose(pf);
		return true;
	}
	else if(szBuf[5] == 'm')
	{
		int nNumber = 0;
		{
			sprintf(szSrc, "%s/smtiles.wil", _pszSrcPath);
		}

		SWilFile xFile;
		if(!xFile.Load(szSrc))
		{
			printf("%s can not find\n", szSrc);
			DEBUG_MSG("src can not find");
			return false;
		}
		else
		{
			while(fgets(szBuf, sizeof(szBuf), pf))
			{
				nNumber = atoi(szBuf);
				sprintf(szSrc, "%s/%06d.bmp",
					_pszDestPath, nNumber);

				BOOL bFileExist = PathFileExists(szSrc);
				if(bFileExist)
				{
					char szDestFile[MAX_PATH];
					sprintf(szDestFile, "%s/%06d_bak.bmp", szBakDirectory, nNumber);
					CopyFile(szSrc, szDestFile, FALSE);
				}

				if(xFile.ExtractBitmap8(szSrc, nNumber))
				{
					printf("%d ok\n", nNumber);

					if(bFileExist)
					{
						char szDestFile[MAX_PATH];
						sprintf(szDestFile, "%s/%06d.bmp", szBakDirectory, nNumber);
						CopyFile(szSrc, szDestFile, FALSE);
					}
				}
			}
		}

		printf("ok\n");
		fclose(pf);
		return true;
	}
	else
	{
		printf("not o and f and b\n");
		DEBUG_MSG("not o and f and b");
	}
}