// ProjBuilder.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CommandLineHelper.h"
#include "tinyxml/tinyxml.h"
#include <Shlwapi.h>
#include <vector>
#include <string>
#include <direct.h>
#include <time.h>
#include <io.h>

using std::vector;
using std::string;

#pragma comment(lib, "shlwapi.lib")

const char* GetRootPath()
{
	static char s_szAppPath[MAX_PATH] = {0};
	if(s_szAppPath[0] == 0)
	{
		GetModuleFileName(NULL, s_szAppPath, sizeof(s_szAppPath));
		PathRemoveFileSpec(s_szAppPath);
	}

	return s_szAppPath;
}

int string_split(const string& str, vector<string>& ret_, string sep = ",")
{
	if (str.empty())
	{
		return 0;
	}

	string tmp;
	string::size_type pos_begin = str.find_first_not_of(sep);
	string::size_type comma_pos = 0;

	while (pos_begin != string::npos)
	{
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}

		if (!tmp.empty())
		{
			ret_.push_back(tmp);
			tmp.clear();
		}
	}
	return 0;
}

int run_command(const char* _pszFile, const char* _pszParameters)
{
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = _pszFile;
	ShExecInfo.lpParameters = _pszParameters;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
	if(ShellExecuteEx(&ShExecInfo))
	{
		if(WAIT_OBJECT_0 == WaitForSingleObject(ShExecInfo.hProcess,INFINITE))
		{
			return 0;
		}
		else
		{
			printf("Copy project failed\n");
			return -8;
		}
	}
	else
	{
		int nErr = GetLastError();
		printf("ShellExecuteEx error:%d\n", nErr);
		return -9;
	}

	return -1;
}

/*
文件名：   rd.c

----------------------------------------------------
    c中提供的对文件夹操作的函数，只能对空文件夹进行
删除，这使很多初学者在编码过程中产生许多困扰，我也
很不爽这件事情，所以编写这个对非空文件夹进行删除的
函数，仅供参考。

注意：本函数编写以VC6为依据，其中关于文件夹的操作函数
      与标准c有所区别。如VC6中的findclose可能需要用c
	  中的closedir（）来代替。
----------------------------------------------------
日期         程序员                       变更记录

2010.4.28    海总（掌门人号）           创建文件，编写函数


----------------------------------------------------
*/



/*
函数入口：文件夹的绝对路径
          const char*  dirPath

函数功能：删除该文件夹，包括其中所有的文件和文件夹

返回值：  0  删除 
         -1  路径不对，或其它情况，没有执行删除操作
*/
int  removeDir(const char*  dirPath)
{

	struct _finddata_t fb;   //查找相同属性文件的存储结构体
	char  path[250];          
	long    handle;
	int  resultone;
	int   noFile;            //对系统隐藏文件的处理标记
	
	noFile = 0;
	handle = 0;

	
	//制作路径
	strcpy(path,dirPath);
    strcat (path,"/*");

	handle = _findfirst(path,&fb);
	//找到第一个匹配的文件
	if (handle != 0)
	{
		//当可以继续找到匹配的文件，继续执行
		while (0 == _findnext(handle,&fb))
		{
			//windows下，常有个系统文件，名为“..”,对它不做处理
			noFile = strcmp(fb.name,"..");
			
			if (0 != noFile)
			{
				//制作完整路径
				memset(path,0,sizeof(path));
				strcpy(path,dirPath);
				strcat(path,"/");
				strcat (path,fb.name);
				//属性值为16，则说明是文件夹，迭代
				if (fb.attrib == 16)
				{
					 removeDir(path);	
				}
				//非文件夹的文件，直接删除。对文件属性值的情况没做详细调查，可能还有其他情况。
				else
				{
					remove(path);
				}
			}	
		}
		//关闭文件夹，只有关闭了才能删除。找这个函数找了很久，标准c中用的是closedir
		//经验介绍：一般产生Handle的函数执行后，都要进行关闭的动作。
		_findclose(handle);
	}
		//移除文件夹
		resultone = rmdir(dirPath);
		return  resultone;
}






int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	MessageBox(NULL, "DEBUG", "D", MB_OK);
#endif
	//	get params
	CommandLineHelper xCmdParser;
	if(!xCmdParser.InitParam())
	{
		printf("Can't initialize command line parser\n");
		return -1;
	}

	const char* pszProjSourceDir = xCmdParser.GetParam("proj_source_dir");
	if(NULL == pszProjSourceDir)
	{
		printf("Invalid param 'proj_source_dir'\n");
		return -2;
	}

	const char* pszProjOutputDir = xCmdParser.GetParam("proj_output_dir");
	if(NULL == pszProjOutputDir)
	{
		printf("Invalid param 'proj_output_dir'\n");
		return -3;
	}

	const char* pszProjName = xCmdParser.GetParam("proj_name");
	if(NULL == pszProjName)
	{
		printf("Invalid param 'proj_name'\n");
		return -4;
	}

	const char* pszVcToolDir = xCmdParser.GetParam("vc_tool_dir");
	if(NULL == pszVcToolDir)
	{
		printf("Invalid param 'vc_tool_dir'\n");
		return -4;
	}

	const char* pszAddMacro = xCmdParser.GetParam("proj_addition_macro");

	//	modify the proj config file to change the macro
	if(NULL == pszAddMacro ||
		strlen(pszAddMacro) == 0)
	{
		printf("Invalid param 'proj_addition_macro'\n");
		return -6;
	}

	const char* pszProjFileName = xCmdParser.GetParam("proj_file_name");
	if(NULL == pszProjFileName)
	{
		printf("Invalid param 'proj_file_name'\n");
		return -20;
	}


	//	parse macro name and value
	char szMacroName[MAX_PATH] = {0};
	char szMacroValue[MAX_PATH] = {0};
	size_t uMacroLength = strlen(pszAddMacro);
	bool bFindMacroEqual = false;

	for(size_t i = 0; i < uMacroLength; ++i)
	{
		if(pszAddMacro[i] == '=')
		{
			bFindMacroEqual = true;
		}
		else
		{
			if(bFindMacroEqual)
			{
				szMacroValue[strlen(szMacroValue)] = pszAddMacro[i];
			}
			else
			{
				szMacroName[strlen(szMacroName)] = pszAddMacro[i];
			}
		}
	}

	//	get timestamp
	time_t rawtime;
	struct tm * timeinfo;
	char szTimestamp [128];

	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (szTimestamp,sizeof(szTimestamp),"%y%m%d%H%M%S",timeinfo);
	strcpy(szMacroValue, szTimestamp);

	//	copy to destinate directory
	char szDestinateDirectory[MAX_PATH];
	sprintf(szDestinateDirectory, "%s\\%s_build%s", pszProjOutputDir, pszProjName, szMacroValue);
	if(PathFileExists(szDestinateDirectory))
	{
		return -5;
	}

	char szCommand[MAX_PATH * 2];
	sprintf(szCommand, "%s %s /s /i /y", pszProjSourceDir, szDestinateDirectory);
	if(0 == run_command("xcopy.exe", szCommand))
	{
		printf("Copy project to %s done\n", szDestinateDirectory);
	}
	else
	{
		printf("Copy project failed\n");
		return -8;
	}

	//	edit config file
	bool bProjConfigUpdated = false;
	char szProjConfileFilePath[MAX_PATH] = {0};
	sprintf(szProjConfileFilePath, "%s/%s.vcproj",
		szDestinateDirectory, pszProjFileName);

	TiXmlDocument doc;
	if(!doc.LoadFile(szProjConfileFilePath))
	{
		printf("Can't load project config file:%s\n", szProjConfileFilePath);
		return -5;
	}

	TiXmlElement* pRoot = doc.RootElement();
	TiXmlNode* pConfigNode = pRoot->FirstChild("Configurations");
	TiXmlNode* pChildNode = pConfigNode->FirstChild("Configuration");

	while(NULL != pChildNode)
	{
		//	reset config
		TiXmlElement* pElement = pChildNode->FirstChildElement();
		while(NULL != pElement)
		{
			const char* pszNameAttrib = pElement->Attribute("Name");
			if(NULL != pszNameAttrib &&
				0 == strcmp(pszNameAttrib, "VCCLCompilerTool"))
			{
				//	ok
				const char* pszPreAttrib = pElement->Attribute("PreprocessorDefinitions");
				if(NULL != pszPreAttrib)
				{
					std::string xPreAttrib = pszPreAttrib;

					//	分割宏
					vector<string> xMacros;
					string_split(xPreAttrib, xMacros, ";");

					if(xMacros.size() != 0)
					{
						//	遍历来寻找宏
						bool bFindMacro = false;

						for(size_t i = 0; i < xMacros.size(); ++i)
						{
							string& refMacroStr = xMacros[i];
							size_t uFndPos = refMacroStr.find(szMacroName);
							int nValidType = 0;

							if(0 == uFndPos)
							{
								//	找到了宏 判定是否有后续
								if(strlen(szMacroName) == refMacroStr.length())
								{
									//	只有该宏
									nValidType = 1;
								}
								else if(strlen(szMacroName) < refMacroStr.length())
								{
									if(refMacroStr[strlen(szMacroName)] == '=')
									{
										//	合法的宏
										nValidType = 2;
									}
								}
							}

							if(1 == nValidType)
							{
								//	只有宏 没有值
								if(strlen(szMacroValue) != 0)
								{
									refMacroStr += "=";
									refMacroStr += szMacroValue;
									bFindMacro = true;
									break;
								}
							}
							else if(2 == nValidType)
							{
								//	有宏 有值 更新值
								if(strlen(szMacroValue) != 0)
								{
									refMacroStr = szMacroName;
									refMacroStr += "=";
									refMacroStr += szMacroValue;
									bFindMacro = true;
									break;
								}
							}
						}

						if(bFindMacro)
						{
							//	找到宏 那么更新节点值
							string xFullMacro = xMacros[0];

							for(size_t i = 1; i < xMacros.size(); ++i)
							{
								xFullMacro += ";";
								xFullMacro += xMacros[i];
							}

							pElement->SetAttribute("PreprocessorDefinitions", xFullMacro.c_str());
							printf("Update PreprocessorDefinitions=%s\n", xFullMacro.c_str());
							bProjConfigUpdated = true;
						}
						else
						{
							//	没有找到宏 添加一个
							string xFullMacro = xMacros[0];

							for(size_t i = 1; i < xMacros.size(); ++i)
							{
								xFullMacro += ";";
								xFullMacro += xMacros[i];
							}
							xFullMacro += ";";
							xFullMacro += pszAddMacro;
							xFullMacro += "=";
							xFullMacro += szMacroValue;

							pElement->SetAttribute("PreprocessorDefinitions", xFullMacro.c_str());
							printf("Add PreprocessorDefinitions=%s\n", xFullMacro.c_str());
							bProjConfigUpdated = true;
						}
					}
				}
				break;
			}

			pElement = pElement->NextSiblingElement();
		}

		pChildNode = pChildNode->NextSibling();
	}

	if(bProjConfigUpdated)
	{
		doc.SaveFile(szProjConfileFilePath);
	}

	printf("Compiling project...\n");

	sprintf(szCommand, "%s/%s.vcproj \"Release|Win32\"", szDestinateDirectory, pszProjFileName);
	if(0 == run_command(pszVcToolDir, szCommand))
	{
		printf("Compile project done\n");
	}
	else
	{
		printf("Compile project failed\n");
		return -11;
	}

	/*printf("Packing sources\n");

	sprintf(szCommand, "%s\\Release", szDestinateDirectory);
	if(!PathFileExists(szCommand))
	{
		printf("Build files not exists.");
		return -13;
	}

	//	remove build files
	if(0 == removeDir(szCommand))
	{
		printf("Remove build files done..");
	}
	else
	{
		printf("Remove build files failed..");
		return -14;
	}*/

	return 0;
}

