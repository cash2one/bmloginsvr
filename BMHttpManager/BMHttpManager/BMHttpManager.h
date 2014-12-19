#ifndef _INC_BMHTTPMANAGER_
#define _INC_BMHTTPMANAGER_
//////////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include <curl.h>
#include <fastdelegate.h>
#include <FastDelegateBind.h>
//////////////////////////////////////////////////////////////////////////
#define HTTP_CONNECTTIMEOUT		10
#define HTTP_TIMEOUT			10
//////////////////////////////////////////////////////////////////////////
typedef fastdelegate::FastDelegate2<const char*, size_t, void> RequestDoneCallback;
//////////////////////////////////////////////////////////////////////////
class BMHttpManager
{
public:
	static BMHttpManager* GetInstance()
	{
		static BMHttpManager s_ins;
		return &s_ins;
	}
	~BMHttpManager(){UnInit();}

protected:
	BMHttpManager();

public:
	bool Init();
	void UnInit();

	bool DoGetRequestSync(const char* _pszUrl, RequestDoneCallback _callback);

public:
	static size_t curl_request_done_callback(void *buffer, size_t size, size_t nmemb, void *userp);

protected:
	CURL* m_pCUrl;
};

//////////////////////////////////////////////////////////////////////////
#endif