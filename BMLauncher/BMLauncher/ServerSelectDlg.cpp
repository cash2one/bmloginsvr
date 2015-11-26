#include "StdAfx.h"
#include "ServerSelectDlg.h"
#include "cJSON.h"
//////////////////////////////////////////////////////////////////////////
BMServerSelectDlg::BMServerSelectDlg()
{

}

BMServerSelectDlg::~BMServerSelectDlg()
{

}

LRESULT BMServerSelectDlg::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_CREATE)
	{
		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("BMServerSelectDlg.xml"), (UINT)0, NULL, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);
		CreateServerButtons();
		//Init();
		return 0;
	}
	else if(uMsg == WM_DESTROY)
	{
		//PostQuitMessage(0);
	}
	else if(uMsg == WM_ERASEBKGND)
	{
		return 1;
	}
	else if( uMsg == WM_NCACTIVATE ) {

		if( !::IsIconic(m_hWnd) ) {

			return (wParam == 0) ? TRUE :FALSE;

		}

	}
	else if( uMsg == WM_NCCALCSIZE ) {

		return 0;

	}
	else if( uMsg == WM_NCPAINT ) {

		return 0;

	}
	else if(uMsg == WM_NCHITTEST)
	{
		return HTCLIENT;
	}

	LRESULT lRes = 0;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void BMServerSelectDlg::Notify(TNotifyUI& msg)
{
	if(msg.sType == "windowinit")
	{
		//	create with server list
	}

	const char* pszSenderName = msg.pSender->GetName();

	if(msg.sType == DUI_MSGTYPE_CLICK)
	{
		if(0 == _tcscmp(pszSenderName, "button_quit")) {
			Close(IDCANCEL);
		}

		int nSelectId = 0;
		if(sscanf(pszSenderName, "server_%d", &nSelectId) == 1)
		{
			//	selected a server
			ServerItemInfoVector::const_iterator begIter = m_xServerItems.begin();
			ServerItemInfoVector::const_iterator endIter = m_xServerItems.end();

			for(begIter;
				begIter != endIter;
				++begIter)
			{
				const ServerItemInfo& refItem = *begIter;

				if(refItem.nId == nSelectId)
				{
					m_xSelectedServerAddress = refItem.xServerAddr;
					break;
				}
			}
		}
		if(!m_xSelectedServerAddress.empty())
		{
			Close();
		}
	}
}

int BMServerSelectDlg::LoadServerListData(const char* _pszJsonData)
{
	int nLoaded = 0;
	if(NULL == _pszJsonData)
	{
		return 0;
	}

	cJSON* pRoot = cJSON_Parse(_pszJsonData);
	if(NULL == pRoot)
	{
		return 0;
	}

	if(pRoot->type != cJSON_Object)
	{
		return 0;
	}

	do 
	{
		cJSON* pServers = cJSON_GetObjectItem(pRoot, "Servers");
		if(NULL == pServers)
		{
			break;
		}

		if(pServers->type != cJSON_Array)
		{
			break;
		}

		int nArrayCount = cJSON_GetArraySize(pServers);
		m_xServerItems.clear();
		m_xServerItems.reserve(nArrayCount);

		for(int i = 0; i < nArrayCount; ++i)
		{
			cJSON* pArrayItem = cJSON_GetArrayItem(pServers, i);
			if(NULL == pArrayItem)
			{
				continue;
			}

			if(pArrayItem->type != cJSON_Object)
			{
				continue;
			}

			cJSON* pItem = cJSON_GetObjectItem(pArrayItem, "Id");
			if(NULL == pItem ||
				pItem->type != cJSON_Number)
			{
				continue;
			}
			int nId = pItem->valueint;

			pItem = cJSON_GetObjectItem(pArrayItem, "Addr");
			if(NULL == pItem ||
				pItem->type != cJSON_String)
			{
				continue;
			}

			const char* pszAddress = pItem->valuestring;

			pItem = cJSON_GetObjectItem(pArrayItem, "Name");
			if(NULL == pItem ||
				pItem->type != cJSON_String)
			{
				continue;
			}

			const char* pszServerName = pItem->valuestring;

			ServerItemInfo item;
			item.nId = nId;
			item.xServerAddr = pszAddress;
			item.xServerName = pszServerName;
			m_xServerItems.push_back(item);
			++nLoaded;
		}
	} while (false);

	cJSON_Delete(pRoot);
	pRoot = NULL;

	return nLoaded;
}

void BMServerSelectDlg::CreateServerButtons()
{
	CVerticalLayoutUI* pButtonContainer = (CVerticalLayoutUI*)m_pm.FindControl("layout_servers");
	if(NULL == pButtonContainer)
	{
		return;
	}

	ServerItemInfoVector::const_iterator begIter = m_xServerItems.begin();
	ServerItemInfoVector::const_iterator endIter = m_xServerItems.end();
	char szBuf[MAX_PATH] = {0};

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		const ServerItemInfo& refItem = *begIter;

		CButtonUI* pButton = new CButtonUI;
		sprintf(szBuf, "server_%d", refItem.nId);
		pButton->SetName(szBuf);
		if(refItem.xServerName.empty())
		{
			sprintf(szBuf, "·þÎñÆ÷£º%d", refItem.nId);
		}
		else
		{
			sprintf(szBuf, "%s", refItem.xServerName.c_str());
		}
		pButton->SetText(szBuf);
		//pButton->SetFixedWidth(260);
		pButton->SetFixedHeight(35);
		pButton->SetNormalImage("file='ok.png' source='0,0,260,35'");
		pButton->SetHotImage("file='ok.png' source='0,35,260,70'");
		pButton->SetPushedImage("file='ok.png' source='0,70,260,105'");
		pButtonContainer->Add(pButton);

		CLabelUI* pGapLabel = new CLabelUI;
		pGapLabel->SetFixedHeight(10);
		pButtonContainer->Add(pGapLabel);
	}
}