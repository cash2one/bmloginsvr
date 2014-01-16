#include "BMHumSaveControl.h"
#include <Windows.h>
#include <ZipArchive.h>
#include <stdio.h>
#include <Shlwapi.h>
//////////////////////////////////////////////////////////////////////////
#if _MSC_VER == 1310
#ifdef _DEBUG
#pragma comment(lib, "ZipArchive_d03.lib")
#else
#pragma comment(lib, "ZipArchive_r03.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "ZipArchive_d.lib")
#else
#pragma comment(lib, "ZipArchive.lib")
#endif
#endif

#pragma comment(lib, "shlwapi.lib")
//////////////////////////////////////////////////////////////////////////
const char* g_szHeader[] =
{
	"head",
		"head1",
		"head2"
};

const char* g_szData[] =
{
	"data",
		"data1",
		"data2"
};

typedef struct _tagHumHead{
	char szName[20];
	char sex;
	char job;
	unsigned short level;
}HumHead, *PHumHead;

typedef struct _tagHumSave{
	CZipArchive* pFile;
	HumHead head[3];

}HumSave, *PHumSave;
//////////////////////////////////////////////////////////////////////////
//	Unexport function
//////////////////////////////////////////////////////////////////////////
int ReadHumHeadFromData(const char* _pszBuf, HumHead* _pHead){
	int nDatalen = 0;
	const char* pData = _pszBuf;
	int nNameLength = *(int*)pData;
	pData += 4;
	nDatalen += 4;
	memcpy(_pHead->szName, pData, nNameLength + 1);
	pData += nNameLength + 1;
	nDatalen += nNameLength + 1;
	_pHead->sex = *pData;
	pData += 1;
	nDatalen += 1;
	_pHead->job = *pData;
	pData += 1;
	nDatalen += 1;
	_pHead->level = *(unsigned short*)pData;
	nDatalen += 2;

	return nDatalen;
}

int WriteHumHeadToData(char* _pszBuf, const HumHead* _pHead){
	//	Write it
	int nDatalen = 0;
	//ZeroMemory(buf, sizeof(buf));
	char* pData = _pszBuf;
	//	name length
	int nNamelen = strlen(_pHead->szName);
	memcpy(pData, &nNamelen, sizeof(nNamelen));
	pData += 4;
	nDatalen += 4;
	//	name content
	memcpy(pData, _pHead->szName, nNamelen + 1);
	pData += nNamelen + 1;
	nDatalen += nNamelen + 1;
	//	sex
	memcpy(pData, &_pHead->sex, sizeof(char));
	pData += 1;
	nDatalen += 1;
	//	job
	memcpy(pData, &_pHead->job, sizeof(char));
	pData += 1;
	nDatalen += 1;
	//	level
	memcpy(pData, &_pHead->level, sizeof(unsigned short));
	nDatalen += 2;

	return nDatalen;
}


//////////////////////////////////////////////////////////////////////////
//	export function
//////////////////////////////////////////////////////////////////////////
int CreateHumSave(const char* _pszPath){
	if(NULL != _pszPath){
#ifdef _DEBUG
		printf("%s\n", _pszPath);
#endif
	}else{
		return 0;
	}

	CZipArchive xArchive;
	if(!PathFileExists(_pszPath)){
		bool bRet = xArchive.Open(_pszPath, CZipArchive::zipCreate);
		if(bRet){
			xArchive.Close();
		}else{
			//	Set password
		}
		return bRet ? 0: 2;
	}

	return 1;
}

int DeleteHumSave(const char* _pszPath){
	if(PathFileExists(_pszPath)){
		BOOL bRet = DeleteFile(_pszPath);
		return bRet == TRUE ? 0 : 1;
	}
	return 2;
}

void CloseHumSave(int _hFileHandle){
	PHumSave pFile = (PHumSave)_hFileHandle;
	if(pFile){
		if(pFile->pFile){
			pFile->pFile->Close();
			delete pFile->pFile;
		}
#ifdef _DEBUG
		printf("\nfile has been closed.\n");
#endif
		delete pFile;
	}
}

int OpenHumSave(const char* _pszPath){
	//	Test if exists
	if(!PathFileExists(_pszPath)){
#ifdef _DEBUG
		printf("\n%s not exists.", _pszPath);
#endif
		return 0;
	}

	PHumSave pSave = new HumSave;
	ZeroMemory(pSave, sizeof(HumSave));
	pSave->pFile = new CZipArchive;
	bool bRet = pSave->pFile->Open(_pszPath);
	if(!bRet){
		pSave->pFile->Close();
		delete pSave->pFile;
		delete pSave;
#ifdef _DEBUG
		printf("\nCan't open the file");
#endif
		return 0;
	}

	//	Get hum information
	char buf[512] = {0};
	ZIP_INDEX_TYPE zIndex = 0;

	for(int i = 0; i < 3; ++i){
		zIndex = pSave->pFile->FindFile(g_szHeader[i]);
		if(ZIP_FILE_INDEX_NOT_FOUND != zIndex){
			CZipFileHeader* pHeader = pSave->pFile->GetFileInfo(zIndex);
			if(pHeader != NULL){
				ZeroMemory(buf, sizeof(buf));
				pSave->pFile->OpenFile(zIndex);
				pSave->pFile->ReadFile(buf, pHeader->m_uUncomprSize);
				pSave->pFile->CloseFile();
				//	Parse it
				ReadHumHeadFromData(buf, &pSave->head[i]);
#ifdef _DEBUG
				printf("\n[%d]%s %d %d %d loaded.\n",
					i,
					pSave->head[i].szName,
					pSave->head[i].job,
					pSave->head[i].sex,
					pSave->head[i].level);
#endif
			}
		}
	}
	return (int)pSave;
}

int AddGameRole(int _hFileHandle, const char* _pszRoleName, char _job, char _sex){
	//	Get the empty index
	int nIndex = -1;
	char buf[512];

	//	Data valid
	if(strlen(_pszRoleName) > 19 ||
		_pszRoleName[0] == 0){
			return 4;
		}
		if(_job < 0 ||
			_job > 2){
				return 4;
			}
			if(_sex < 1 ||
				_sex > 2)
			{
				return 4;
			}

			PHumSave pSave = (PHumSave)_hFileHandle;
			if(pSave == NULL){
				return 2;
			}
			if(pSave->pFile == NULL){
				return 2;
			}
			if(pSave->pFile->IsClosed()){
				return 2;
			}

			//	Repeated name
			for(int i = 0; i < 3; ++i){
				if(0 == strcmp(pSave->head[i].szName, _pszRoleName)){
					return 6;
				}
			}

			//	Get the valid index
			for(int i = 0; i < 3; ++i){
				if(pSave->head[i].szName[0] == 0){
					nIndex = i;
					break;
				}
			}

			if(nIndex == -1){
				return 1;
			}

			ZIP_INDEX_TYPE zIndex = pSave->pFile->FindFile(g_szHeader[nIndex]);
			if(zIndex != ZIP_FILE_INDEX_NOT_FOUND){
				return 3;
			}
			CZipFileHeader zHeader;
			zHeader.SetFileName(g_szHeader[nIndex]);
			pSave->pFile->OpenNewFile(zHeader);
			//	Write it
			int nDatalen = 0;
			ZeroMemory(buf, sizeof(buf));
			strcpy(pSave->head[nIndex].szName, _pszRoleName);
			pSave->head[nIndex].job = _job;
			pSave->head[nIndex].sex = _sex;
			pSave->head[nIndex].level = 1;
			nDatalen = WriteHumHeadToData(buf, &pSave->head[nIndex]);

			int nRet = 0;
			if(nDatalen != 0){
				pSave->pFile->WriteNewFile(buf, nDatalen);
			}else{
				ZeroMemory(&pSave->head[nIndex], sizeof(HumHead));
				nRet = 5;
			}
			pSave->pFile->CloseNewFile();
#ifdef _DEBUG
			printf("\nCreate %s in save.", g_szHeader[nIndex]);
#endif
			return nRet;
}

int DelGameRole(int _hFileHandle, const char* _pszRoleName){
	PHumSave pSave = (PHumSave)_hFileHandle;
	int nIndex = -1;
	for(int i = 0; i < 3; ++i){
		if(0 == strcmp(pSave->head[i].szName, _pszRoleName)){
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1){
		return 1;
	}

	ZIP_INDEX_TYPE zIndex = 0;
	zIndex = pSave->pFile->FindFile(g_szHeader[nIndex]);
	if(zIndex != ZIP_FILE_INDEX_NOT_FOUND){
		pSave->pFile->RemoveFile(zIndex);
	}
	zIndex = pSave->pFile->FindFile(g_szData[nIndex]);
	if(zIndex != ZIP_FILE_INDEX_NOT_FOUND){
		pSave->pFile->RemoveFile(zIndex);
	}
	ZeroMemory(&pSave->head[nIndex], sizeof(HumHead));
	return 0;
}

int GetGameRoleInfo_Value(int _hFileHandle, int _index, char* _job, char* _sex, unsigned short* _level){
	if(_index < 0 ||
		_index > 2)
	{
		return 1;
	}

	PHumSave pSave = (PHumSave)_hFileHandle;
	*_job = pSave->head[_index].job;
	*_sex = pSave->head[_index].sex;
	*_level = pSave->head[_index].level;

	return 0;
}

char* GetGameRoleInfo_Name(int _hFileHandle, int _index){
	if(_index < 0 ||
		_index > 2)
	{
		return NULL;
	}

	PHumSave pSave = (PHumSave)_hFileHandle;
	char* pszRet = (char*)malloc(strlen(pSave->head[_index].szName) + 1);
	strcpy(pszRet, pSave->head[_index].szName);
	return pszRet;
}

int GetGameRoleIndex(int _hFileHandle, const char* _pszRoleName){
	PHumSave pSave = (PHumSave)_hFileHandle;
	int nIndex = 3;

	for(int i = 0; i < 3; ++i){
		if(0 == strcmp(pSave->head[i].szName, _pszRoleName)){
			nIndex = i;
			break;
		}
	}

	return nIndex;
}

int ReadGameRoleSize(int _hFileHandle, const char* _pszRoleName)
{
	PHumSave pSave = (PHumSave)_hFileHandle;
	int nIndex = -1;
	char* pData = NULL;

	for(int i = 0; i < 3; ++i){
		if(0 == strcmp(pSave->head[i].szName, _pszRoleName)){
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1){
#ifdef _DEBUG
		printf("\nCan't find head %s\n",
			_pszRoleName);
#endif
		return 0;
	}

	ZIP_INDEX_TYPE zIndex = 0;
	zIndex = pSave->pFile->FindFile(g_szData[nIndex]);
	if(zIndex != ZIP_FILE_INDEX_NOT_FOUND){
		CZipFileHeader* pHeader = pSave->pFile->GetFileInfo(zIndex);
		return pHeader->m_uUncomprSize;
	}
	else
	{
#ifdef _DEBUG
		printf("\ncan't locate %s\n",
			_pszRoleName);
#endif
	}

	return 0;
}

int ReadGameRoleData(int _hFileHandle, const char* _pszRoleName, void* _pData)
{
	char* pBuf = (char*)_pData;
	PHumSave pSave = (PHumSave)_hFileHandle;
	int nIndex = -1;

	for(int i = 0; i < 3; ++i){
		if(0 == strcmp(pSave->head[i].szName, _pszRoleName)){
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1){
#ifdef _DEBUG
		printf("\nCan't find head %s\n",
			_pszRoleName);
#endif
		return 0;
	}

	ZIP_INDEX_TYPE zIndex = 0;
	zIndex = pSave->pFile->FindFile(g_szData[nIndex]);
	if(zIndex != ZIP_FILE_INDEX_NOT_FOUND){
		CZipFileHeader* pHeader = pSave->pFile->GetFileInfo(zIndex);
		if(NULL != pHeader){
			pSave->pFile->OpenFile(zIndex);
			pSave->pFile->ReadFile(pBuf, pHeader->m_uUncomprSize);
			pSave->pFile->CloseFile();
			return 0;
		}
	}
	else
	{
#ifdef _DEBUG
		printf("\ncan't locate %s\n",
			_pszRoleName);
#endif
	}

	return 1;
}

/*
char* ReadGameRoleData(int _hFileHandle, const char* _pszRoleName, int* _outsize){
*_outsize = 0;
PHumSave pSave = (PHumSave)_hFileHandle;
int nIndex = -1;
char* pData = NULL;

for(int i = 0; i < 3; ++i){
if(0 == strcmp(pSave->head[i].szName, _pszRoleName)){
nIndex = i;
break;
}
}

if(nIndex == -1){
#ifdef _DEBUG
printf("\nCan't find head %s\n",
_pszRoleName);
#endif
return 0;
}

ZIP_INDEX_TYPE zIndex = 0;
zIndex = pSave->pFile->FindFile(g_szData[nIndex]);
if(zIndex != ZIP_FILE_INDEX_NOT_FOUND){
CZipFileHeader* pHeader = pSave->pFile->GetFileInfo(zIndex);
if(NULL != pHeader){
pData = (char*)malloc(pHeader->m_uUncomprSize + 1);
pSave->pFile->OpenFile(zIndex);
pSave->pFile->ReadFile(pData, pHeader->m_uUncomprSize);
pSave->pFile->CloseFile();
*_outsize = pHeader->m_uUncomprSize;
pData[pHeader->m_uUncomprSize] = 0;
}
}
else
{
#ifdef _DEBUG
printf("\ncan't locate %s\n",
_pszRoleName);
#endif
}

return pData;
}*/

int WriteGameRoleData(int _hFileHandle, const char* _pszRoleName, const char* _pData, int _datalen){
	PHumSave pSave = (PHumSave)_hFileHandle;
	int nIndex = -1;

	for(int i = 0; i < 3; ++i){
		if(0 == strcmp(pSave->head[i].szName, _pszRoleName)){
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1){
		return 1;
	}

	ZIP_INDEX_TYPE zIndex = 0;
	zIndex = pSave->pFile->FindFile(g_szData[nIndex]);
	if(zIndex != ZIP_FILE_INDEX_NOT_FOUND){
		pSave->pFile->RemoveFile(zIndex);

		//	Overwrite file
		CZipFileHeader header;
		header.SetFileName(g_szData[nIndex]);
		pSave->pFile->OpenNewFile(header);
		pSave->pFile->WriteNewFile(_pData, _datalen);
		pSave->pFile->CloseNewFile();
	}
	else
	{
		//return 2;
		//	New file
		CZipFileHeader header;
		header.SetFileName(g_szData[nIndex]);
		pSave->pFile->OpenNewFile(header);
		pSave->pFile->WriteNewFile(_pData, _datalen);
		pSave->pFile->CloseNewFile();
	}

	return 0;
}

int WriteGameRoleInfo(int _hFileHandle, const char* _pszRoleName, char _job, char _sex, unsigned short _level){
	PHumSave pSave = (PHumSave)_hFileHandle;
	int nIndex = -1;

	for(int i = 0; i < 3; ++i){
		if(0 == strcmp(pSave->head[i].szName, _pszRoleName)){
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1){
		return 1;
	}

	pSave->head[nIndex].job = _job;
	pSave->head[nIndex].sex = _sex;
	pSave->head[nIndex].level = _level;

	char buf[512] = {0};
	int nDatalen = WriteHumHeadToData(buf, &pSave->head[nIndex]);
	ZIP_INDEX_TYPE zIndex = pSave->pFile->FindFile(g_szHeader[nIndex]);
	if(zIndex != ZIP_FILE_INDEX_NOT_FOUND){
		pSave->pFile->RemoveFile(zIndex);
		CZipFileHeader header;
		header.SetFileName(g_szHeader[nIndex]);
		pSave->pFile->RemoveFile(zIndex);
		pSave->pFile->OpenNewFile(header);
		pSave->pFile->WriteNewFile(buf, nDatalen);
		pSave->pFile->CloseNewFile();
	}
	else
	{
		return 2;
	}

	return 0;
}

int UpdateGameRoleInfo(int _hFileHandle, const char* _pszRoleName, unsigned short _level){
	PHumSave pSave = (PHumSave)_hFileHandle;
	int nIndex = -1;

	for(int i = 0; i < 3; ++i){
		if(0 == strcmp(pSave->head[i].szName, _pszRoleName)){
			nIndex = i;
			break;
		}
	}

	if(nIndex == -1){
		return 1;
	}

	pSave->head[nIndex].level = _level;

	char buf[512] = {0};
	int nDatalen = WriteHumHeadToData(buf, &pSave->head[nIndex]);
	ZIP_INDEX_TYPE zIndex = pSave->pFile->FindFile(g_szHeader[nIndex]);
	if(zIndex != ZIP_FILE_INDEX_NOT_FOUND){
		pSave->pFile->RemoveFile(zIndex);
		CZipFileHeader header;
		header.SetFileName(g_szHeader[nIndex]);
		pSave->pFile->RemoveFile(zIndex);
		pSave->pFile->OpenNewFile(header);
		pSave->pFile->WriteNewFile(buf, nDatalen);
		pSave->pFile->CloseNewFile();
	}
	else
	{
		return 2;
	}

	return 0;
}

int ReadGameRoleHeadInfo(int _hFileHandle, int _index, void* _pData)
{
	PHumSave pSave = (PHumSave)_hFileHandle;
	char* pData = (char*)_pData;
	int nWriteSize = 0;
	
	if(_index < 0 ||
		_index > 2)
	{
		return 0;
	}

	if(pSave->head[_index].szName[0] != 0)
	{
		char nameLen = strlen(pSave->head[_index].szName);
		memcpy(pData, &nameLen, sizeof(int));
		pData += 1;
		nWriteSize += 1;
		memcpy(pData, pSave->head[_index].szName, nameLen + 1);
		pData += nameLen + 1;
		nWriteSize += nameLen + 1;
		memcpy(pData, &pSave->head[_index].sex, 1);
		pData += 1;
		nWriteSize += 1;
		memcpy(pData, &pSave->head[_index].job, 1);
		pData += 1;
		nWriteSize += 1;
		memcpy(pData, &pSave->head[_index].level, 2);
		pData += 2;
		nWriteSize += 2;
	}

	return nWriteSize;
}






//////////////////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved){
	return TRUE;
}