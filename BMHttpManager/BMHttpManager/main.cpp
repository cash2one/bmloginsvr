// main.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "BMHttpManager.h"
#include "EasyUtils.h"

class DataRecv
{
public:
	void onDataRecv(const char* _pData, size_t _uLen)
	{
		printf(_pData);
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	DataRecv dr;

	//BMHttpManager::GetInstance()->DoGetRequestSync("127.0.0.1:8081/mailverify?mail=sryan@qq.com", fastdelegate::bind(&DataRecv::onDataRecv, &dr));

	if(!EasyDownloadFile("http://sryanyuan.github.io/web/config/ls.ini", "h:/ls.ini")){
		printf("cannot download file");
		return -1;
	}

	char* pRead = EasyReadFile("h:/ls.ini");
	if(!pRead){
		printf("cannot read file");
		return -2;
	}

	printf(pRead);
	delete[] pRead;

	return 0;
}

