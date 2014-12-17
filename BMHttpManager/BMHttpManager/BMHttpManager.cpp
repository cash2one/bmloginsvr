#include "BMHttpManager.h"
//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma comment(lib, "libcurld.lib")
#else
#pragma comment(lib, "libcurl.lib")
#endif
//////////////////////////////////////////////////////////////////////////
BMHttpManager::BMHttpManager()
{
	m_pCUrl = NULL;
}

bool BMHttpManager::Init()
{
	//m_pCUrl = curl_easy_init();
	//	start thread??
	return true;
}

void BMHttpManager::UnInit()
{
	/*if(m_pCUrl)
	{
		curl_easy_cleanup(m_pCUrl);
		m_pCUrl = NULL;
	}*/
}

bool BMHttpManager::DoGetRequestSync(const char* _pszUrl, RequestDoneCallback _callback)
{
	curl_global_init(CURL_GLOBAL_ALL);

	CURL* pCUrl = curl_easy_init();
	bool bRet = false;

	do 
	{
		if(NULL == pCUrl)
		{
			break;
		}

		curl_easy_setopt(pCUrl, CURLOPT_URL, _pszUrl);
		curl_easy_setopt(pCUrl, CURLOPT_HTTPGET, 1);
		curl_easy_setopt(pCUrl, CURLOPT_READFUNCTION, NULL);
		curl_easy_setopt(pCUrl, CURLOPT_WRITEFUNCTION, &BMHttpManager::curl_request_done_callback);
		curl_easy_setopt(pCUrl, CURLOPT_WRITEDATA, (void*)&_callback);
		curl_easy_setopt(pCUrl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(pCUrl, CURLOPT_CONNECTTIMEOUT, HTTP_CONNECTTIMEOUT);
		curl_easy_setopt(pCUrl, CURLOPT_TIMEOUT, HTTP_TIMEOUT);

		//	proxy
		curl_easy_setopt(pCUrl, CURLOPT_PROXY, NULL);

		CURLcode code = curl_easy_perform(pCUrl);
		if(code != CURLE_OK)
		{
			//	error
			_callback("request error", 0);
			break;
		}

		bRet = true;
	} while (0);

	curl_easy_cleanup(pCUrl);
	curl_global_cleanup();

	return bRet;
}

size_t BMHttpManager::curl_request_done_callback(void *buffer, size_t size, size_t nmemb, void *userp)
{
	RequestDoneCallback* pCallback = (RequestDoneCallback*)userp;

	if(NULL == pCallback ||
		NULL == buffer ||
		0 == size)
	{
		return -1;
	}

	const char* pData = (const char*)buffer;
	(*pCallback)(pData, size);
	return nmemb;
}