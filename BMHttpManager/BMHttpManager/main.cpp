// main.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "BMHttpManager.h"

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

	BMHttpManager::GetInstance()->DoGetRequestSync("127.0.0.1:8081/mailverify?mail=sryan@qq.com", fastdelegate::bind(&DataRecv::onDataRecv, &dr));

	int a = 0;
	scanf("%d", &a);

	return 0;
}

