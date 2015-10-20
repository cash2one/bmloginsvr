#include <afxwin.h>
#include "MapPacker.h"
#include <Shlwapi.h>
#include "MirMap.h"
#include "WilExtractor.h"
#include <direct.h>
#include "CommandLineHelper.h"
#include <map>
//////////////////////////////////////////////////////////////////////////
void ExeWaitForEnd(const char* _pszRunCmd)
{
	STARTUPINFO si = {sizeof(si)};
	PROCESS_INFORMATION pi = {0};

	char szRunParam[512];
	strcpy(szRunParam, _pszRunCmd);

	CreateProcess(
		NULL,
		szRunParam,
		NULL,           // process security attributes
		NULL,           // primary thread security attributes
		0,              // handles are inherited
		0,              // creation flags
		NULL,           // use parent's environment
		NULL,           // use parent's current directory
		&si,   // STARTUPINFO pointer
		&pi);   // receives PROCESS_INFORMATION

	// Wait for the processs to finish
	DWORD rc = WaitForSingleObject(
		pi.hProcess, // process handle
		INFINITE);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

/*判断一个路径是否是已存在的目录*/
BOOL IsDirectory(LPCTSTR pstrPath)
{
	if (NULL == pstrPath)
	{
		return FALSE;
	}

	/*去除路径末尾的反斜杠*/
	CString strPath = pstrPath;
	if (strPath.Right(1) == _T('\\'))
	{
		strPath.Delete(strPath.GetLength()-1);
	}

	CFileFind finder;
	BOOL bRet = finder.FindFile(strPath);
	if (!bRet)
	{
		return FALSE;
	}

	/*判断该路径是否是目录*/
	finder.FindNextFile();
	bRet = finder.IsDirectory();
	finder.Close();
	return bRet;
}

/*判断一个路径是否是已存在的目录*/
/*删除目录及目录中的所有内容*/
BOOL DeleteFolder(LPCTSTR pstrFolder)
{
	if ((NULL == pstrFolder))
	{
		return FALSE;
	}

	/*检查输入目录是否是合法目录*/
	if (!IsDirectory(pstrFolder))
	{
		return FALSE;
	}

	/*创建源目录中查找文件的通配符*/
	CString strWildcard(pstrFolder);
	if (strWildcard.Right(1) != _T('\\'))
	{
		strWildcard += _T("\\");
	}
	strWildcard += _T("*.*");

	/*打开文件查找，查看源目录中是否存在匹配的文件*/
	/*调用FindFile后，必须调用FindNextFile才能获得查找文件的信息*/
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		/*查找下一个文件*/
		bWorking = finder.FindNextFile();

		/*跳过当前目录“.”和上一级目录“..”*/
		if (finder.IsDots())
		{
			continue;
		}

		/*得到当前目录的子文件的路径*/
		CString strSubFile = finder.GetFilePath();

		/*判断当前文件是否是目录,*/
		/*如果是目录，递归调用删除目录,*/
		/*否则，直接删除文件*/
		if (finder.IsDirectory())
		{
			if (!DeleteFolder(strSubFile))
			{
				finder.Close();
				return FALSE;
			}
		}
		else
		{
			if (!DeleteFile(strSubFile))
			{
				finder.Close();
				return FALSE;
			}
		}

	} /*while (bWorking)*/

	/*关闭文件查找*/
	finder.Close();

	/*删除空目录*/
	return RemoveDirectory(pstrFolder);
}
//////////////////////////////////////////////////////////////////////////
int MapPackerMain()
{
	CommandLineHelper xCmd;
	if(!xCmd.InitParam())
	{
		return -1;
	}

	const char* pszMapFile = xCmd.GetParam("mapfile");
	const char* pszResFile = xCmd.GetParam("resfilepath");
	const char* pszMapIndex = xCmd.GetParam("mapindex");
	const char* pszOutputPath = xCmd.GetParam("outputpath");

	if(NULL == pszMapFile ||
		NULL == pszResFile ||
		NULL == pszMapIndex ||
		NULL == pszOutputPath)
	{
		return -2;
	}

	AfxMessageBox("DD");

	return MapPacker(pszMapFile, pszResFile, atoi(pszMapIndex), pszOutputPath);
}

int MapPacker(const char* _pszMapFile, const char* _pszResFile, int _nMapIndex, const char* _pOutputPath)
{
	char szWorkingPath[MAX_PATH];
	strcpy(szWorkingPath, _pOutputPath);
	strcat(szWorkingPath, "/working");

	if(!PathFileExists(szWorkingPath))
	{
		mkdir(szWorkingPath);
	}

	char szWorkingOldPath[MAX_PATH];
	sprintf(szWorkingOldPath, "%s/old", szWorkingPath);
	if(!PathFileExists(szWorkingOldPath))
	{
		mkdir(szWorkingOldPath);
	}

	char szWorkingNewPath[MAX_PATH];
	sprintf(szWorkingNewPath, "%s/new", szWorkingPath);
	if(!PathFileExists(szWorkingNewPath))
	{
		mkdir(szWorkingNewPath);
	}

	TileMap xMap;
	if(!xMap.LoadMap(_pszMapFile))
	{
		return -1;
	}

	MapInfo info;
	xMap.GetMapInfo(info);
	MapTile* pTile = NULL;
	int nResIndex = 1;

	char szResFile[MAX_PATH];

	std::map<int, int> xResIndexMap;

	//	tiles
	sprintf(szResFile, "%s/tiles.wil", _pszResFile);
	for(int i = 0; i < info.nRow; ++i)
	{
		for(int j = 0; j < info.nCol; ++j)
		{
			pTile = xMap.GetCellData(i, j);

			if((pTile->wBkImg & (~0x8000)) > 0)
			{
				int nValue = (pTile->wBkImg & (~0x8000)) - 1;
				bool bBlocked = (pTile->wBkImg & (0x8000)) != 0;

				//	test value
				std::map<int, int>::const_iterator xFinder = xResIndexMap.find(nValue);
				if(xFinder != xResIndexMap.end())
				{
					//	exists
					//	set new index
					pTile->wBkImg = xFinder->second + 1;
					if(bBlocked)
					{
						pTile->wBkImg |= 0x8000;
					}
					continue;
				}

				if(WilExtractOneFile(szResFile, nValue, szWorkingOldPath))
				{
					//	change index
					char szBmpFileName[MAX_PATH];
					sprintf(szBmpFileName, "%s\\%06d.bmp",
						szWorkingOldPath, nValue);

					int nNewValue = nResIndex;
					nResIndex++;
					char szNewBmpFileName[MAX_PATH];
					sprintf(szNewBmpFileName, "%s\\%06d.bmp",
						szWorkingNewPath, nNewValue);

					CopyFile(szBmpFileName, szNewBmpFileName, FALSE);

					//	set new index
					pTile->wBkImg = nNewValue + 1;
					if(bBlocked)
					{
						pTile->wBkImg |= 0x8000;
					}

					xResIndexMap.insert(std::make_pair(nValue, nNewValue));

					printf("new bmp file %d.bmp\n", nValue);
				}
				else
				{
					printf("error while extracting file %d.bmp\n", nValue);
				}
			}
		}
	}

	//	smtiles
	xResIndexMap.clear();

	sprintf(szResFile, "%s/smtiles.wil", _pszResFile);
	for(int i = 0; i < info.nRow; ++i)
	{
		for(int j = 0; j < info.nCol; ++j)
		{
			pTile = xMap.GetCellData(i, j);

			if((pTile->wMidImg & (~0x8000)) > 0)
			{
				int nValue = (pTile->wMidImg & (~0x8000)) - 1;
				bool bBlocked = (pTile->wMidImg & (0x8000)) != 0;

				//	test value
				std::map<int, int>::const_iterator xFinder = xResIndexMap.find(nValue);
				if(xFinder != xResIndexMap.end())
				{
					//	exists
					//	set new index
					pTile->wMidImg = xFinder->second + 1;
					if(bBlocked)
					{
						pTile->wMidImg |= 0x8000;
					}
					continue;
				}

				if(WilExtractOneFile(szResFile, nValue, szWorkingOldPath))
				{
					//	change index
					char szBmpFileName[MAX_PATH];
					sprintf(szBmpFileName, "%s\\%06d.bmp",
						szWorkingOldPath, nValue);

					int nNewValue = nResIndex;
					nResIndex++;
					char szNewBmpFileName[MAX_PATH];
					sprintf(szNewBmpFileName, "%s\\%06d.bmp",
						szWorkingNewPath, nNewValue);

					CopyFile(szBmpFileName, szNewBmpFileName, FALSE);

					//	set new index
					pTile->wMidImg = nNewValue + 1;
					if(bBlocked)
					{
						pTile->wMidImg |= 0x8000;
					}

					xResIndexMap.insert(std::make_pair(nValue, nNewValue));

					printf("new bmp file %d.bmp\n", nValue);
				}
				else
				{
					printf("error while extracting file %d.bmp\n", nValue);
				}
			}
		}
	}

	//	which objects
	for(int i = 0; i < 40; ++i)
	{
		int nNumber = i;
		char szObjectsName[20] = {0};
		if(nNumber == 0)
		{
			sprintf(szObjectsName, "objects");
		}
		else
		{
			sprintf(szObjectsName, "objects%d", nNumber + 1);
		}

		bool bBreak = false;

		for(int i = 0; i < info.nRow; ++i)
		{
			for(int j = 0; j < info.nCol; ++j)
			{
				pTile = xMap.GetCellData(i, j);

				if((pTile->wFrImg & (~0x8000)) > 0 &&
					pTile->bArea == nNumber)
				{
					printf("%s\n", szObjectsName);
					bBreak = true;
					break;
				}
			}

			if(bBreak)
			{
				break;
			}
		}
	}

	getchar();

	//	objects
	for(int i = 0; i < 40; ++i)
	{
		int nNumber = i;
		char szObjectsName[20] = {0};
		if(nNumber == 0)
		{
			sprintf(szObjectsName, "objects");
		}
		else
		{
			sprintf(szObjectsName, "objects%d", nNumber + 1);
		}

		sprintf(szResFile, "%s/%s.wil", _pszResFile, szObjectsName);

		xResIndexMap.clear();

		for(int i = 0; i < info.nRow; ++i)
		{
			for(int j = 0; j < info.nCol; ++j)
			{
				pTile = xMap.GetCellData(i, j);

				if((pTile->wFrImg & (~0x8000)) > 0 &&
					pTile->bArea == nNumber)
				{
					int nValue = (pTile->wFrImg & (~0x8000));
					bool bBlocked = (pTile->wFrImg & (0x8000)) != 0;

					char bAni = pTile->bAniFrame;
					if((bAni & 0x80) > 0)
					{
						bAni &= (~0x80);
					}

					if(bAni > 0)
					{
						int nFrBase = nValue;
						int nAniFrames = bAni;
						bool bAniSetted = false;

						for(int k = 0; k < nAniFrames; ++k)
						{
							nValue = nFrBase + k;
							--nValue;

							//	test value
							std::map<int, int>::const_iterator xFinder = xResIndexMap.find(nValue);
							if(xFinder != xResIndexMap.end())
							{
								//	exists
								//	set new index
								if(!bAniSetted)
								{
									pTile->wFrImg = xFinder->second + 1;
									if(bBlocked)
									{
										pTile->wFrImg |= 0x8000;
									}
									bAniSetted = true;
								}
								
								continue;
							}
							
							//if(WilExtractOneFile(szResFile, nValue, szWorkingOldPath))
							char szOldFile[MAX_PATH];
							sprintf(szOldFile, "%s/%s/%06d.bmp", _pszResFile, szObjectsName, nValue);
							if(PathFileExists(szOldFile))
							{
								//	change index
								/*char szBmpFileName[MAX_PATH];
								sprintf(szBmpFileName, "%s\\%06d.bmp",
									szWorkingOldPath, nValue);*/

								int nNewValue = nResIndex;
								nResIndex++;
								char szNewBmpFileName[MAX_PATH];
								sprintf(szNewBmpFileName, "%s\\%06d.bmp",
									szWorkingNewPath, nNewValue);

								CopyFile(szOldFile, szNewBmpFileName, FALSE);

								//	set new index
								if(!bAniSetted)
								{
									pTile->wFrImg = nNewValue + 1;
									if(bBlocked)
									{
										pTile->wFrImg |= 0x8000;
									}
									bAniSetted = true;
								}

								xResIndexMap.insert(std::make_pair(nValue, nNewValue));

								printf("new bmp file %d.bmp\n", nValue);
							}
							else
							{
								printf("error while extracting file %d.bmp\n", nValue);
							}
						}
					}
					else
					{
						--nValue;

						//	test value
						std::map<int, int>::const_iterator xFinder = xResIndexMap.find(nValue);
						if(xFinder != xResIndexMap.end())
						{
							//	exists
							//	set new index
							pTile->wFrImg = xFinder->second + 1;
							if(bBlocked)
							{
								pTile->wFrImg |= 0x8000;
							}
							continue;
						}

						//if(WilExtractOneFile(szResFile, nValue, szWorkingOldPath))
						char szOldFile[MAX_PATH];
						sprintf(szOldFile, "%s/%s/%06d.bmp", _pszResFile, szObjectsName, nValue);
						if(PathFileExists(szOldFile))
						{
							//	change index
							/*char szBmpFileName[MAX_PATH];
							sprintf(szBmpFileName, "%s\\%06d.bmp",
								szWorkingOldPath, nValue);*/

							int nNewValue = nResIndex;
							nResIndex++;
							char szNewBmpFileName[MAX_PATH];
							sprintf(szNewBmpFileName, "%s\\%06d.bmp",
								szWorkingNewPath, nNewValue);

							CopyFile(szOldFile, szNewBmpFileName, FALSE);

							//	set new index
							pTile->wFrImg = nNewValue + 1;
							if(bBlocked)
							{
								pTile->wFrImg |= 0x8000;
							}

							xResIndexMap.insert(std::make_pair(nValue, nNewValue));

							printf("new bmp file %d.bmp\n", nValue);
						}
						else
						{
							printf("error while extracting file %d.bmp\n", nValue);
						}
					}
				}
			}
		}
	}

	//	new map index
	xMap.SetResIndex(_nMapIndex);

	char szNewMapFile[MAX_PATH];
	sprintf(szNewMapFile, "%s/map%03d.map", _pOutputPath, _nMapIndex);
	xMap.SaveMap(szNewMapFile);

	//	pack
	char szSibFile[MAX_PATH];
	sprintf(szSibFile, "%s/map%03d.sib", _pOutputPath, _nMapIndex);

	char szCmd[512] = {0};
	sprintf(szCmd, "sibpacker_tt.exe \"%s\" \"%s\"",
		szWorkingNewPath, szSibFile);
	ExeWaitForEnd(szCmd);

	AfxMessageBox("OK");

	//	delete
	DeleteFolder(szWorkingPath);

	return 1;
}