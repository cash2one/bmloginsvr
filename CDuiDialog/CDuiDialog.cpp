#include "StdAfx.h"

#include "CDuiDialog.h"
//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDuiDialog, CDialog)
	//ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	//ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	//ON_WM_SETCURSOR()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &CDuiDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDuiDialog::OnBnClickedCancel)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////
CDuiDialog::CDuiDialog(UINT _nIDTemplate, CWnd* _pParent, const char* _pszXmlFile)
	:CDialog(_nIDTemplate, _pParent)
{
	m_xXmlFile = _pszXmlFile;
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BOOL CDuiDialog::PreTranslateMessage(MSG* pMsg)
{
	//m_pm.MessageHandler(pMsg->message, pMsg->wParam, pMsg->lParam, );
	if(m_pm.TranslateMessage(pMsg))
	{
		//	don't dispatch this message
		return TRUE;
	}

	/*LRESULT lRet = 0;
	if(m_pm.MessageHandler(pMsg->message, pMsg->wParam, pMsg->lParam, lRet))
	{
		return TRUE;
	}*/

	//	WM_CHAR must be dispatched to support rich edit
	/*if(pMsg->message == WM_CHAR)
	{
		LRESULT lRet = 0;
		m_pm.MessageHandler(pMsg->message, pMsg->wParam, pMsg->lParam, lRet);
	}*/

	return __super::PreTranslateMessage(pMsg);
}

LRESULT CDuiDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//return __super::WindowProc(message, wParam, lParam);
	//	input windows message into Duilib
	LRESULT lRet = 0;

	if(FilterDUIMessage(message, wParam, lParam, lRet))
	{
		//	Processed, don't let m_pm to process it
		lRet = __super::WindowProc(message, wParam, lParam);
	}
	else
	{
		if(!m_pm.MessageHandler(message, wParam, lParam, lRet))
		{
			lRet = __super::WindowProc(message, wParam, lParam);
		}
		else
		{
			//	some event has been processed by m_pm, so don't invoke function below again.
			/*bool bFiltered = false;
			switch(message)
			{
			case WM_SETCURSOR:
				{
					bFiltered = true;
				}break;
			default:
				{
					bFiltered = false;
				}break;
			}

			if(!bFiltered)
			{
				__super::WindowProc(message, wParam, lParam);
			}*/
			if(FilterMFCMessage(message, wParam, lParam, lRet))
			{
				//	nothing
			}
			else
			{
				__super::WindowProc(message, wParam, lParam);
			}
		}
	}

	return lRet;
}

int CDuiDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int nRet = __super::OnCreate(lpCreateStruct);

	if(!InitializeDuiSkin())
	{
		DUI__Trace("InitializeDuiSkin failed...You must specify the resource path and resource xml file first before creating the dialog.");
		return -1;
	}

	return nRet;
}

void CDuiDialog::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		//dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//	Do nothing
		//CDialog::OnPaint();
	}
}

void CDuiDialog::OnClose()
{
	//
	OnCancel();
}

/*BOOL CDuiDialog::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//return TRUE;
	HCURSOR hCursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	::SetCursor(hCursor);
	return TRUE;
	return __super::OnSetCursor(pWnd, nHitTest, message);
}*/

void CDuiDialog::Notify(TNotifyUI& msg)
{
	if(msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
		OnWindowInit(msg);
	}
}

bool CDuiDialog::FilterDUIMessage(UINT _uMsg, WPARAM _wParam, LPARAM _lParam, LRESULT& _lRet)
{
	return false;
}

bool CDuiDialog::FilterMFCMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& _lRet)
{
	bool bFiltered = false;

	switch(message)
	{
	case WM_SETCURSOR:
		{
			bFiltered = true;
		}break;
	default:
		{
			bFiltered = false;
		}break;
	}

	return bFiltered;
}

void CDuiDialog::OnWindowInit(TNotifyUI& _msg)
{
	
}











bool CDuiDialog::InitializeDuiSkin()
{
	if(m_xXmlFile.empty())
	{
		return false;
	}

	m_pm.Init(GetSafeHwnd());
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(m_xXmlFile.c_str(), (UINT)0, NULL, &m_pm);
	if(NULL == pRoot)
	{
		DUI__Trace("Failed to parse XML[%s]", CPaintManagerUI::GetResourcePath() + m_xXmlFile.c_str());
	}
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	return true;
}


void CDuiDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDuiDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
}
