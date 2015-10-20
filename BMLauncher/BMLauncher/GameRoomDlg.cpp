// GameRoomDlg.cpp : 实现文件
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BMLauncher.h"
#include "GameRoomDlg.h"
#include "BMHttpManager.h"
#include "cJSON.h"
#include "LauncherUtils.h"
#include <fastdelegate.h>
#include <FastDelegateBind.h>
//////////////////////////////////////////////////////////////////////////
string UTF8ToGBK(const std::string& strUTF8) 
{ 
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1]; memset(wszGBK, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUTF8.c_str(), -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1); 
	WideCharToMultiByte(CP_ACP,0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL, NULL); //strUTF8 = szGBK; 
	std::string strTemp(szGBK); 
	delete[]szGBK; 
	delete[]wszGBK; 
	return strTemp; 
}
//////////////////////////////////////////////////////////////////////////


// GameRoomDlg 对话框

IMPLEMENT_DYNAMIC(GameRoomDlg, CDialog)

GameRoomDlg::GameRoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(GameRoomDlg::IDD, pParent)
{
	m_nLoginServerID = 0;
}

GameRoomDlg::~GameRoomDlg()
{
}

void GameRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, m_xListCtrl);
	DDX_Control(pDX, IDC_EDIT1, m_xEditPassword);
}

void GameRoomDlg::OnDestroy()
{
	if(m_xPingThread.GetThreadState() == ThreadRunner::TS_RUN)
	{
		m_xPingThread.Stop();
		ThreadRunner::THREAD_STATE ts = m_xPingThread.GetThreadState();

		while(ts == ThreadRunner::TS_RUN)
		{
			Sleep(5);
			ts = m_xPingThread.GetThreadState();
		}
		m_xPingThread.UnInit();
	}
	__super::OnDestroy();
}

BEGIN_MESSAGE_MAP(GameRoomDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &GameRoomDlg::OnBnJoinClicked)
	ON_BN_CLICKED(IDC_BUTTON2, &GameRoomDlg::OnBnRefreshClicked)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_USER_PINGRESULT, &GameRoomDlg::OnUserMsgPingResult)
END_MESSAGE_MAP()


// GameRoomDlg 消息处理程序
void GameRoomDlg::OnBnJoinClicked()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSelIndex = -1;
	DWORD dwCnnIndex = 0xFFFFFFFF;

	for(int i = 0; i < m_xListCtrl.GetItemCount(); ++i)
	{
		if(m_xListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			nSelIndex = i;
			break;
		}
	}

	if(nSelIndex != -1)
	{
		char szBuf[100];
		LVITEM item;
		item.iItem = nSelIndex;
		item.iSubItem = 0;
		item.mask = LVIF_TEXT | LVIF_PARAM;
		item.pszText = szBuf;
		item.cchTextMax = 100;

		if(m_xListCtrl.GetItem(&item))
		{
			int nSelServerID = item.lParam;
			char szMsgTip[MAX_PATH] = {0};
			sprintf(szMsgTip, "确定要加入房间[%s]?", item.pszText);
			int nUserSel = MessageBox(szMsgTip, "询问", MB_YESNO);
			if(nUserSel == IDYES)
			{
				UpdateData(TRUE);
				ZeroMemory(szMsgTip, sizeof(szMsgTip));
				::GetWindowText(m_xEditPassword.GetSafeHwnd(), szMsgTip, sizeof(szMsgTip));

				//	登录该服务器
				DoLoginServer(nSelServerID, szMsgTip);
			}
		}
	}
	else
	{
		MessageBox("请选择有效的游戏房间",
			"错误",
			MB_OK | MB_ICONERROR);
	}
}

LRESULT GameRoomDlg::OnUserMsgPingResult(WPARAM _wParam, LPARAM _lParam)
{
	ULONG ulINetAddr = ULONG(_wParam);
	int nDelay = int(_lParam);
	char szBuf[10];

	//	寻找对应的单元格
	for(int i = 0; i < m_xListCtrl.GetItemCount(); ++i)
	{
		CString xIpText = m_xListCtrl.GetItemText(i, 2);
		ULONG ulAddr = inet_addr((const char*)xIpText);

		if(ulINetAddr == ulAddr)
		{
			if(nDelay < 0)
			{
				strcpy(szBuf ,"timeout");
			}
			else
			{
				sprintf(szBuf, "%d ms", nDelay);
			}
			m_xListCtrl.SetItemText(i, 5, szBuf);
		}
	}

	return 0;
}







BOOL GameRoomDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE ||
			pMsg->wParam == VK_RETURN)
		{
			return FALSE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL GameRoomDlg::OnInitDialog()
{
	BOOL bRet = CDialog::OnInitDialog();

	CListCtrl* pListCtrl = static_cast<CListCtrl*>(GetDlgItem(IDC_LIST1));

	if(NULL != pListCtrl)
	{
		LONG lStyle; 
		lStyle = GetWindowLong(pListCtrl->GetSafeHwnd(), GWL_STYLE);// 获取当前窗口style 
		lStyle &= ~LVS_TYPEMASK; // 清除显示方式位 
		lStyle |= LVS_REPORT; // 设置style 
		SetWindowLong(pListCtrl->GetSafeHwnd(), GWL_STYLE, lStyle);// 设置style 
		DWORD dwStyle = pListCtrl->GetExtendedStyle(); 
		dwStyle |= LVS_EX_FULLROWSELECT;// 选中某行使整行高亮（只适用与report 风格的listctrl ） 
		dwStyle |= LVS_EX_GRIDLINES;// 网格线（只适用与report 风格的listctrl ） 
		pListCtrl->SetExtendedStyle(dwStyle);

		int nColumnCount = 0;
		m_xListCtrl.InsertColumn(nColumnCount++, "房间名称", LVCFMT_CENTER, 120);
		m_xListCtrl.InsertColumn(nColumnCount++, "版本", LVCFMT_CENTER, 80);
		m_xListCtrl.InsertColumn(nColumnCount++, "房间IP", LVCFMT_CENTER, 100);
		m_xListCtrl.InsertColumn(nColumnCount++, "是否加密", LVCFMT_CENTER, 80);
		m_xListCtrl.InsertColumn(nColumnCount++, "在线人数", LVCFMT_CENTER, 80);
		m_xListCtrl.InsertColumn(nColumnCount++, "延迟", LVCFMT_CENTER, 80);

		UpdateServerList();
	}

	m_xEditPassword.SetLimitText(15);

	if(m_xPingThread.Init())
	{
		m_xPingThread.SetRunSleepTime(3 * 1000);
		m_xPingThread.SetReceiverMsgID(WM_USER_PINGRESULT);
		m_xPingThread.SetPingTimeout(200);
		m_xPingThread.SetReceiverHwnd(GetSafeHwnd());
		m_xPingThread.Run();
	}

	return bRet;
}


void GameRoomDlg::UpdateServerList()
{
	if(m_xRoomRequestUrl.empty())
	{
		return;
	}

	//	获取房间列表
	char szUrl[50] = {0};
	sprintf(szUrl, "%s/getgslist", m_xRoomRequestUrl.c_str());
	BMHttpManager::GetInstance()->DoGetRequestSync(szUrl, fastdelegate::bind(&GameRoomDlg::OnServerListResult, this));
}

void GameRoomDlg::OnServerListResult(const char *_pData, size_t _uLen)
{
	if(NULL == _pData ||
		0 == _uLen)
	{
		MessageBox("无法获取房间列表，请稍候重试",
			"错误",
			MB_OK | MB_ICONERROR);
		return;
	}

	m_xListCtrl.DeleteAllItems();
	m_xPingThread.ClearTask();

	cJSON* pRoot = cJSON_Parse(_pData);
	if(NULL == pRoot)
	{
		return;
	}

	do 
	{
		cJSON* pServerList = cJSON_GetObjectItem(pRoot, "Servers");
		if(NULL == pServerList)
		{
			break;
		}
		int nServerCount = cJSON_GetArraySize(pServerList);
		char szIP[20] = {0};

		cJSON* pServerItem = pServerList->child;
		while(NULL != pServerItem)
		{
			//	也是一个object
			int nServerID = cJSON_GetObjectItem(pServerItem, "Id")->valueint;
			const char* pszServerName = cJSON_GetObjectItem(pServerItem, "Note")->valuestring;
			int nOnline = cJSON_GetObjectItem(pServerItem, "Online")->valueint;
			const char* pszPassword = cJSON_GetObjectItem(pServerItem, "Password")->valuestring;
			const char* pszIp = cJSON_GetObjectItem(pServerItem, "Address")->valuestring;
			string xServerName = UTF8ToGBK(pszServerName);
			const char* pszVersion = "";
			cJSON* pVersionItem = cJSON_GetObjectItem(pServerItem, "Version");
			if(NULL != pVersionItem)
			{
				pszVersion = pVersionItem->valuestring;
				if(NULL == pszVersion)
				{
					pszVersion = "";
				}
			}

			//	show ip
			strcpy(szIP, pszIp);
			for(int i = strlen(szIP); i >= 0; --i)
			{
				if(szIP[i] == '.')
				{
					szIP[i + 1] = '*';
					break;
				}
				else
				{
					szIP[i] = 0;
				}
			}

			LV_ITEM    lvitemAdd = {0};
			lvitemAdd.mask = LVIF_TEXT|LVIF_PARAM;
			lvitemAdd.pszText = (LPTSTR)xServerName.c_str();
			lvitemAdd.lParam = nServerID;

			/*
			m_xListCtrl.InsertColumn(nColumnCount++, "房间名称", LVCFMT_CENTER, 200);
			m_xListCtrl.InsertColumn(nColumnCount++, "版本", LVCFMT_CENTER, 80);
			m_xListCtrl.InsertColumn(nColumnCount++, "房间IP", LVCFMT_CENTER, 150);
			m_xListCtrl.InsertColumn(nColumnCount++, "是否加密", LVCFMT_CENTER, 100);
			m_xListCtrl.InsertColumn(nColumnCount++, "在线人数", LVCFMT_CENTER, 100);
			m_xListCtrl.InsertColumn(nColumnCount++, "延迟", LVCFMT_CENTER, 80);
			*/

			int nRow = m_xListCtrl.InsertItem(&lvitemAdd);
			if(strlen(pszVersion) == 0)
			{
				m_xListCtrl.SetItemText(nRow, 1, "2.09.46");
			}
			else
			{
				m_xListCtrl.SetItemText(nRow, 1, pszVersion);
			}
			m_xListCtrl.SetItemText(nRow, 2, pszIp);
			m_xListCtrl.SetItemText(nRow, 3, strlen(pszPassword) > 0 ? "加密" : "未加密");
			char szNumber[10] = {0};
			m_xListCtrl.SetItemText(nRow, 4, itoa(nOnline, szNumber, 10));
			m_xListCtrl.SetItemText(nRow, 5, "*");

			//	add task
			m_xPingThread.AddTask(pszIp);

			pServerItem = pServerItem->next;
		}
	} while (0);

	cJSON_Delete(pRoot);
	pRoot = NULL;
}


void GameRoomDlg::DoLoginServer(int _nServerID, const char *_pszPassword)
{
	m_nLoginServerID = _nServerID;
	if(NULL != _pszPassword)
	{
		m_xLoginServerPsw = _pszPassword;
	}

	//	加入服务器
	char szUrl[MAX_PATH] = {0};
	sprintf(szUrl, "%s/getgsaddr?id=%d&password=%s",
		m_xRoomRequestUrl.c_str(),
		m_nLoginServerID,
		m_xLoginServerPsw.c_str());
	BMHttpManager::GetInstance()->DoGetRequestSync(szUrl, fastdelegate::bind(&GameRoomDlg::OnGsAddrResult, this));
}

void GameRoomDlg::OnBnRefreshClicked()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateServerList();
}

void GameRoomDlg::OnGsAddrResult(const char *_pData, size_t _uLen)
{
	if(0 == _uLen)
	{
		//	timeout
		ErrorMsgBox("连接游戏服务器超时");
		return;
	}

	cJSON* pRoot = cJSON_Parse(_pData);
	if(NULL == pRoot)
	{
		ErrorMsgBox("解析json失败");
		return;
	}

	int nRet = cJSON_GetObjectItem(pRoot, "Result")->valueint;
	if(0 == nRet)
	{
		const char* pszAddr = cJSON_GetObjectItem(pRoot, "Msg")->valuestring;
		if(NULL != pszAddr)
		{
			char szIp[20] = {0};
			char szPort[10] = {0};
			bool bReadIp = true;

			for(int i = 0; i < strlen(pszAddr); ++i)
			{
				if(pszAddr[i] == ':')
				{
					bReadIp = false;
				}
				else
				{
					if(bReadIp)
					{
						szIp[strlen(szIp)] = pszAddr[i];
					}
					else
					{
						szPort[strlen(szPort)] = pszAddr[i];
					}
				}
			}

			if(szPort[0] != 0 &&
				!bReadIp)
			{
				int nPort = atoi(szPort);
				Launcher_Client(szIp, nPort, "", "");
			}
		}
	}
	else
	{
		char szMsg[MAX_PATH] = {0};
		sprintf(szMsg, "验证失败，可能是由于密码错误。");

		const char* pszErr = cJSON_GetObjectItem(pRoot, "Msg")->valuestring;
		if(NULL != pszErr)
		{
			strcat(szMsg, pszErr);
		}
		ErrorMsgBox(pszErr);
	}
}