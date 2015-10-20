#include "SIBPacker.h"
#include <io.h>
#include <algorithm>
#include <Shlwapi.h>
#include "SIBFile.h"

#define _CONSOLE_PRINT_
//////////////////////////////////////////////////////////////////////////
static const DWORD s_dwZero = 0;
//////////////////////////////////////////////////////////////////////////
int SIBPackBMP8BitAnd16Bit(const char* _pszPath, const char* _pszDest)
{
	FILENAMES xNames;
	FILENUMBERS xNumbers;
	bool bHave8Bits = false;
	bool bHave16Bits = false;

	int nRet = GetDirectoryFiles(_pszPath, "bmp", xNames);
	if(PACK_OK != nRet)
	{
		return nRet;
	}

	int nMaxNumber = MakeNumber(xNames, xNumbers);
	//if(nMaxNumber == 0)
	{
		//return PACK_NOTNUMBER;
	}

	++nMaxNumber;
	SIBImgInfo* pInfos = new SIBImgInfo[nMaxNumber];
	memset(pInfos, 0, sizeof(SIBImgInfo) * nMaxNumber);
	FILENUMBERS::const_iterator finditer = xNumbers.end();
	FILENUMBERS::const_iterator enditer = xNumbers.end();
	char szFileName[MAX_PATH];
	char szBuf[10240];

	SIBPackHeader stHeader;
	DWORD dwWrite = 0;
	memset(&stHeader, 0, sizeof(SIBPackHeader));
	strcpy(stHeader.szInfo, "#sryan's image box!#");
	stHeader.uImgCount = nMaxNumber;

	//	Create destinate file
	HANDLE hDestFile = ::CreateFile(_pszDest, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	if(hDestFile == INVALID_HANDLE_VALUE)
	{
		nRet = PACK_OUTFILE;
		goto lbl_errdestfile;
	}

	DWORD* pPalette8 = NULL;
	DWORD* pPalette16 = NULL;
	//	Read the ImgInfo
	DWORD dwRead = 0;
	HANDLE hBMPFile = INVALID_HANDLE_VALUE;
	HANDLE hOftFile = INVALID_HANDLE_VALUE;
	DWORD dwFileSize = 0;
	POINT ptOffset = {0};
	WORD wBits = 0;

	for(int i = 0; i < nMaxNumber; ++i)
	{
		finditer = std::find(xNumbers.begin(), xNumbers.end(), i);
		if(finditer != enditer)
		{
			//	file exist, read the information
			sprintf(szFileName, "%s\\%06d.bmp",
				_pszPath, i);
			hBMPFile = ::CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
			if(hBMPFile != INVALID_HANDLE_VALUE)
			{
				//	First test it to figure out if it is a 8 or 16bits bitmap
				SetFilePointer(hBMPFile, 0x1C, NULL, FILE_BEGIN);
				ReadFile(hBMPFile, &wBits, sizeof(wBits), &dwRead, NULL);
				if(wBits == 8)
				{
					if(NULL == pPalette8)
					{
						//	Get the palette data
						pPalette8 = new DWORD[256];
						SetFilePointer(hBMPFile, 0x36, NULL, FILE_BEGIN);
						ReadFile(hBMPFile, pPalette8, 256 * sizeof(DWORD), &dwRead, NULL);
					}
				}
				else if(wBits == 16)
				{
					if(NULL == pPalette16)
					{
						pPalette16 = new DWORD[3];
						SetFilePointer(hBMPFile, 0x36, NULL, FILE_BEGIN);
						ReadFile(hBMPFile, pPalette16, 3 * sizeof(DWORD), &dwRead, NULL);
					}
					pInfos[i].uImgPointer |= BITMAP_16BITMASK;
				}
				
				SetFilePointer(hBMPFile, 0x12, NULL, FILE_BEGIN);
				ReadFile(hBMPFile, &pInfos[i].uImgWidth, sizeof(WORD), &dwRead, NULL);
				SetFilePointer(hBMPFile, 0x16, NULL, FILE_BEGIN);
				ReadFile(hBMPFile, &pInfos[i].uImgHeight, sizeof(WORD), &dwRead, NULL);
// 				ReadFile(hBMPFile, szBuf, sizeof(DWORD), &dwRead, NULL);
// 				pInfos[i].uImgWidth = *(unsigned short*)szBuf;
// 				ReadFile(hBMPFile, szBuf, sizeof(DWORD), &dwRead, NULL);
// 				pInfos[i].uImgHeight = *(unsigned short*)szBuf;

				sprintf(szFileName, "%s\\Placements\\%06d.txt",
					_pszPath, i);
				hOftFile = ::CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
				if(hOftFile != INVALID_HANDLE_VALUE)
				{
					dwFileSize = GetFileSize(hOftFile, NULL);
					ReadFile(hOftFile, szBuf, dwFileSize, &dwRead, NULL);
					szBuf[dwRead] = 0;
					if(SIBParseOffset(szBuf, ptOffset))
					{
						pInfos[i].sImgOftX = (short)ptOffset.x;
						pInfos[i].sImgOftY = (short)ptOffset.y;
					}
					CloseHandle(hOftFile);
				}
				CloseHandle(hBMPFile);
			}
		}
	}

#ifdef _CONSOLE_PRINT_
	printf("Debug info:\n");
	for(int i = 0; i < nMaxNumber; ++i)
	{
		printf("%d : Width %d Height %d OffsetX %d OffsetY %d\n",
			i, pInfos[i].uImgWidth, pInfos[i].uImgHeight, pInfos[i].sImgOftX, pInfos[i].sImgOftY);
	}
#endif
	//	First wirte box header
	if(pPalette8)
	{
		stHeader.uPalette += 256;
	}
	if(pPalette16)
	{
		stHeader.uPalette += 3;
	}

	WriteFile(hDestFile, stHeader.szInfo, sizeof(stHeader.szInfo), &dwWrite, NULL);
	WriteFile(hDestFile, &stHeader.uImgCount, sizeof(stHeader.uImgCount), &dwWrite, NULL);
	WriteFile(hDestFile, &stHeader.uPalette, sizeof(stHeader.uPalette), &dwWrite, NULL);
	//	Write the palette
	if(pPalette8)
	{
		WriteFile(hDestFile, pPalette8, 256 * sizeof(DWORD), &dwWrite, NULL);
	}
	if(pPalette16)
	{
		WriteFile(hDestFile, pPalette16, 3 * sizeof(DWORD), &dwWrite, NULL);
	}
	//	Now Write info file
	DWORD dwFileDataPointer = /*Header*/34 + /*Palette*/(pPalette8 != NULL ? 256*4 : 0) + (pPalette16 != NULL ? 3*4 : 0);

	//	ImgInfo size:12
	//	Calculate the ImgInfo size
	for(int i = 0; i < stHeader.uImgCount; ++i)
	{
		if(pInfos[i].uImgWidth == 0)
		{
			dwFileDataPointer += sizeof(DWORD);
		}
		else
		{
			dwFileDataPointer += 12;
		}
	}

	bool bIs16Bits = false;
	for(int i = 0; i < stHeader.uImgCount; ++i)
	{
		if(pInfos[i].uImgWidth == 0)
		{
			//	Empty image, just write zero
			WriteFile(hDestFile, &s_dwZero, sizeof(DWORD), &dwWrite, NULL);
		}
		else
		{
			//	Write the hole structure
			bIs16Bits = (pInfos[i].uImgPointer & BITMAP_16BITMASK) != 0;
			pInfos[i].uImgPointer = dwFileDataPointer;
			if(bIs16Bits)
			{
				pInfos[i].uImgPointer |= BITMAP_16BITMASK;
			}
			WriteFile(hDestFile, &pInfos[i].uImgPointer, sizeof(pInfos[i].uImgPointer), &dwWrite, NULL);
			WriteFile(hDestFile, &pInfos[i].uImgWidth, sizeof(pInfos[i].uImgWidth), &dwWrite, NULL);
			WriteFile(hDestFile, &pInfos[i].uImgHeight, sizeof(pInfos[i].uImgHeight), &dwWrite, NULL);
			WriteFile(hDestFile, &pInfos[i].sImgOftX, sizeof(pInfos[i].sImgOftX), &dwWrite, NULL);
			WriteFile(hDestFile, &pInfos[i].sImgOftY, sizeof(pInfos[i].sImgOftY), &dwWrite, NULL);
			//	Modify in 13.06.01
			if(bIs16Bits)
			{
				//dwFileDataPointer += (pInfos[i].uImgWidth * pInfos[i].uImgHeight * 2);
				dwFileDataPointer += (pInfos[i].uImgWidth * 16 + 31) / 32 * 4 * pInfos[i].uImgHeight;
			}
			else
			{
				//dwFileDataPointer += (pInfos[i].uImgWidth * pInfos[i].uImgHeight * 1);
				dwFileDataPointer += (pInfos[i].uImgWidth * 8 + 31) / 32 * 4 * pInfos[i].uImgHeight;
			}
		}
	}

	//	Write the image data
	DWORD dwImgSize = 0;
	DWORD dwImgHeight = 0;
	DWORD dwImgWidth = 0;
	DWORD dwCanRead = 0;
	for(int i = 0; i < nMaxNumber; ++i)
	{
		finditer = std::find(xNumbers.begin(), xNumbers.end(), i);
		if(finditer != enditer)
		{
			sprintf(szFileName, "%s\\%06d.bmp",
				_pszPath, i);
			hBMPFile = ::CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
			if(hBMPFile != INVALID_HANDLE_VALUE)
			{
				//SetFilePointer(hBMPFile, 0x12, NULL, FILE_BEGIN);
				//ReadFile(hBMPFile, szBuf, sizeof(DWORD), &dwRead, NULL);
				//dwImgWidth = *(unsigned int*)szBuf;
				//ReadFile(hBMPFile, szBuf, sizeof(DWORD), &dwRead, NULL);
				//dwImgHeight = *(unsigned int*)szBuf;
				//dwImgSize = dwImgHeight * dwImgWidth * 1;
				bIs16Bits = (pInfos[i].uImgPointer & BITMAP_16BITMASK) != 0;
				//dwImgSize = pInfos[i].uImgWidth * pInfos[i].uImgHeight * (bIs16Bits ? 2 : 1);
				//	Modify in 13.06.01
				if(bIs16Bits)
				{
					//	16bits
					dwImgSize = (pInfos[i].uImgWidth * 16 + 31) / 32 * 4 * pInfos[i].uImgHeight;
				}
				else
				{
					dwImgSize = (pInfos[i].uImgWidth * 8 + 31) / 32 * 4 * pInfos[i].uImgHeight;
				}
				//	Set the beginning position
				//	Modified on 13.06.19 ,reading the data offset directly
				/*if(bIs16Bits)
				{
					SetFilePointer(hBMPFile, 0x36 + sizeof(DWORD) * 3, NULL, FILE_BEGIN);
				}
				else
				{
					SetFilePointer(hBMPFile, 0x36 + sizeof(DWORD) * 256, NULL, FILE_BEGIN);
				}*/
				DWORD dwDataOffset = 0;
				SetFilePointer(hBMPFile, 0x0A, NULL, FILE_BEGIN);
				ReadFile(hBMPFile, &dwDataOffset, 4, &dwRead, NULL);
				SetFilePointer(hBMPFile, dwDataOffset, NULL, FILE_BEGIN);

				while(dwImgSize > 0)
				{
					dwCanRead = (dwImgSize > sizeof(szBuf) ? sizeof(szBuf) : dwImgSize);
					ReadFile(hBMPFile, szBuf, dwCanRead, &dwRead, NULL);
					WriteFile(hDestFile, szBuf, dwCanRead, &dwWrite, NULL);
					dwImgSize -= dwCanRead;
				}
				CloseHandle(hBMPFile);
			}
		}
	}

	//	Now is over
	CloseHandle(hDestFile);
#ifdef _CONSOLE_PRINT_
	if(pPalette8)
	{
		printf("8bits done!\n");
	}
	if(pPalette16)
	{
		printf("16bits done!\n");
	}
#endif

	delete[] pInfos;
	delete[] pPalette8;
	delete[] pPalette16;
	return nRet;

lbl_errdestfile:
	delete [] pInfos;
	return nRet;
}
//////////////////////////////////////////////////////////////////////////
int GetDirectoryFiles(const char* _pszPath, const char* _pszExt, FILENAMES& _xFiles)
{
	if(0 != _access(_pszPath, 0))
	{
		return PACK_DRTNOTFOUND;
	}

	int nRet = PACK_OK;
	WIN32_FIND_DATA stFindData;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	char szPath[MAX_PATH];
	DWORD dwLastError = 0;

	FILENAMES& xFileNames = _xFiles;

	sprintf(szPath, "%s\\*.%s",
		_pszPath, _pszExt);
	hFile = FindFirstFile(szPath, &stFindData);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		nRet = PACK_INVALID_FILE;
	}
	else
	{
		if(stFindData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		{
			xFileNames.push_back(stFindData.cFileName);
		}
	}

	while(FindNextFile(hFile, &stFindData))
	{
		if(stFindData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		{
			xFileNames.push_back(stFindData.cFileName);
		}
	}

	FindClose(hFile);

	return nRet;
}
//////////////////////////////////////////////////////////////////////////
int MergeImgOffset(const char* _pszPath)
{
	FILENAMES xFileNames;
	int nRet = GetDirectoryFiles(_pszPath, "txt", xFileNames);
	if(nRet != PACK_OK)
	{
		return nRet;
	}

	if(xFileNames.size() == 0)
	{
		return PACK_NOFILE;
	}

	nRet = PACK_OK;
	char szDestFile[MAX_PATH];
	sprintf(szDestFile, "%s\\merge.txt",
		_pszPath);
	HANDLE hDestFile = ::CreateFile(szDestFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	if(hDestFile == INVALID_HANDLE_VALUE)
	{
		return PACK_NEWFILE;
	}

	HANDLE hSrcFile = NULL;
	char szBuf[1024];
	DWORD dwRead = 0;
	DWORD dwFileSize = 0;
	DWORD dwWrite = 0;

	size_t nFileNumber = xFileNames.size();
	for(size_t i = 0; i < nFileNumber; ++i)
	{
		sprintf(szDestFile, "%s\\%s",
			_pszPath, xFileNames[i].c_str());
		hSrcFile = ::CreateFile(szDestFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
		if(hSrcFile == INVALID_HANDLE_VALUE)
		{
			continue;
		}
		dwFileSize = GetFileSize(hSrcFile, NULL);
		if(dwFileSize == 0)
		{
			CloseHandle(hSrcFile);
			continue;
		}
		ReadFile(hSrcFile, szBuf, dwFileSize, &dwRead, NULL);
		if(dwRead == dwFileSize)
		{
			SetFilePointer(hDestFile, 0, NULL, FILE_END);
			WriteFile(hDestFile, szBuf, dwRead, &dwWrite, NULL);
		}
		CloseHandle(hSrcFile);
	}

	return nRet;
}
//////////////////////////////////////////////////////////////////////////
int MakeNumber(FILENAMES& _xNames, FILENUMBERS& _xNumbers)
{
	size_t nSize = _xNames.size();
	if(nSize == 0)
	{
		return 0;
	}
	int nMaxNumber = 0;
	int nGetNumber = 0;

	for(size_t i = 0; i < nSize; ++i)
	{
		nGetNumber = atoi(_xNames[i].c_str());
		_xNumbers.push_back(nGetNumber);
		if(nGetNumber > nMaxNumber)
		{
			nMaxNumber = nGetNumber;
		}
	}

	return nMaxNumber;
}
//////////////////////////////////////////////////////////////////////////
bool SIBParseOffset(const char* _pszOft, POINT& _pt)
{
	size_t nStrlen = strlen(_pszOft);
	if(nStrlen <= 4)
	{
		return false;
	}
	if(_pszOft[nStrlen - 1] != 0x0A ||
		_pszOft[nStrlen - 2] != 0x0D)
	{
		return false;
	}

	int nRead = 0;
	int nIndex = 0;
	char szBuf[10];
	memset(szBuf, 0, sizeof(szBuf));

	for(size_t i = 0; i < nStrlen; ++i)
	{
		if(_pszOft[i] == 0x0A)
		{
			//	Nothing
			nIndex = 0;
			szBuf[nIndex] = 0;
			memset(szBuf, 0, sizeof(szBuf));
		}
		else if(_pszOft[i] == 0x0D)
		{
			++nRead;
			if(nRead == 1)
			{
				_pt.x = atoi(szBuf);
			}
			else if(nRead == 2)
			{
				_pt.y = atoi(szBuf);
				return true;
			}
		}
		else
		{
			szBuf[nIndex++] = _pszOft[i];
		}
	}

	return false;
}


//	Extra
int ExtraSIBFile(const char* _pszSIBPath, const char* _pszOutput)
{
	SIBFile xFile;
	if(!xFile.Load(_pszSIBPath))
	{
		printf("不是SIB文件，文件错误\n");
		return -2;
	}
	int nRet = xFile.Extract(_pszOutput);
	if(nRet == PACK_OK)
	{
		printf("Extract OK!\n");
	}
	else
	{
		printf("Error: %d", nRet);
	}
	return 0;
}