#ifndef _INC_FILEPATH_UTILS_
#define _INC_FILEPATH_UTILS_
//////////////////////////////////////////////////////////////////////////
#include <string>
#include <Windows.h>
#include <io.h>
#include <list>
//////////////////////////////////////////////////////////////////////////
struct FolderFileInfo
{
	std::string xFilename;
	std::string xSubPath;
};
typedef std::list<FolderFileInfo> FolderFilenameList;
//////////////////////////////////////////////////////////////////////////
void dfsFolder(std::string folderPath, std::string& _refSubPath, FolderFilenameList& _refNames, const char* _pszExtension = "");

void CreateAllPath(const char* _pszParentPath, const char* _pszPath);
//////////////////////////////////////////////////////////////////////////
#endif