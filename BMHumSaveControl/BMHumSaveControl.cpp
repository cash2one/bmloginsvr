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
	char job;
	char sex;
	int level;
}HumHead, *PHumHead;

typedef struct _tagHumSave{
	CZipArchive* pFile;
	HumHead head[3];

}HumSave, *PHumSave;
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
		}
		return bRet ? 0: 2;
	}

	return 1;
}

int OpenHumSave(const char* _pszPath){
	return 0;
}

int AddGameRole(int _hFileHandle, const char* _pszRoleName, char _job, char _sex){
	return 1;
}

int DelGameRole(int _hFileHandle, const char* _pszRoleName){
	return 1;
}

int GetGameRoleInfo(int _index, char* _pszRoleName, char* _job, char* _sex, int* _level){
	return 1;
}







//////////////////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved){
	return TRUE;
}