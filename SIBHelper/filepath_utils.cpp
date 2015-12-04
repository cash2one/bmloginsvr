#include "filepath_utils.h"
#include <Shlwapi.h>
#include <direct.h>
//////////////////////////////////////////////////////////////////////////
using std::string;
//////////////////////////////////////////////////////////////////////////
void dfsFolder(std::string folderPath, std::string& _refSubPath, FolderFilenameList& _refNames, const char* _pszExtension)
{
	_finddata_t FileInfo;
	string strfind = folderPath + "\\*";
	string strSubPath = _refSubPath;
	long Handle = _findfirst(strfind.c_str(), &FileInfo);

	if (Handle == -1L)
	{
		return;
	}
	do{
		//判断是否有子目录
		if (FileInfo.attrib & _A_SUBDIR)    
		{
			//这个语句很重要
			if( (strcmp(FileInfo.name,".") != 0 ) &&(strcmp(FileInfo.name,"..") != 0))   
			{
				string newPath = folderPath + "\\" + FileInfo.name;
				string subPath = strSubPath + "\\" + FileInfo.name;
				dfsFolder(newPath, subPath, _refNames, _pszExtension);
			}
		}
		else  
		{
			bool bValid = true;

			if(strcmp(_pszExtension, "") != 0)
			{
				if(strcmp(_pszExtension, PathFindExtension(FileInfo.name)) != 0)
				{
					bValid = false;
				}
			}

			if(bValid)
			{
				FolderFileInfo info;
				info.xFilename = FileInfo.name;
				info.xSubPath = strSubPath;

				_refNames.push_back(info);
			}
		}
	}while (_findnext(Handle, &FileInfo) == 0);

	_findclose(Handle);
}

void CreateAllPath(const char* _pszParentPath, const char* _pszPath)
{
	char szPath[MAX_PATH] = {0};
	char szFullPath[MAX_PATH] = {0};
	sprintf(szFullPath, "%s/", _pszParentPath);
	int nIndex = strlen(_pszPath);

	for(int i = 0; i < nIndex; ++i)
	{
		if(_pszPath[i] == '\\' ||
			_pszPath[i] == '/')
		{
			if(strlen(szPath) != 0)
			{
				//	create path
				strcat(szFullPath, "/");
				strcat(szFullPath, szPath);

				if(!PathFileExists(szFullPath))
				{
					mkdir(szFullPath);
				}
			}
			ZeroMemory(szPath, sizeof(szPath));
			continue;
		}

		szPath[strlen(szPath)] = _pszPath[i];
	}

	if(szPath[0] != 0)
	{
		//	create path
		strcat(szFullPath, "/");
		strcat(szFullPath, szPath);

		if(!PathFileExists(szFullPath))
		{
			mkdir(szFullPath);
		}
	}
}