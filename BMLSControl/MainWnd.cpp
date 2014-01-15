#include "MainWnd.h"
//////////////////////////////////////////////////////////////////////////
MainWnd::MainWnd()
{
	m_pBtnMin = m_pBtnClose = m_pBtnSend = NULL;
	m_pReOut = NULL;
}

MainWnd::~MainWnd()
{

}

LRESULT MainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRet = 0;

	if(uMsg == WM_CREATE)
	{
		m_pm.Init(m_hWnd);
		CDialogBuilder db;
		CControlUI* pRoot = db.Create(__T("BMLSControl.xml"), NULL, NULL, &m_pm);
		if(NULL == pRoot)
		{
			assert("Dialog xml parse error." == NULL);
		}
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);

		return 0;
	}
	else if(uMsg == WM_DESTROY)
	{
		PostQuitMessage(0);
	}
	else if(uMsg == WM_ERASEBKGND)
	{
		return 1;
	}
	else if(uMsg == WM_NCPAINT)
	{
		return 0;
	}
	else if(uMsg == WM_NCCALCSIZE)
	{
		return 0;
	}
	else if(uMsg == WM_NCACTIVATE)
	{
		if(!IsIconic(m_hWnd))
		{
			if(wParam == 0)
			{
				lRet = 1;
			}
			else
			{
				lRet = 0;
			}
		}
	}
	else if(uMsg == WM_NCHITTEST)
	{
		POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);

		RECT rcCaption = m_pm.GetCaptionRect();
		if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
				if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
					_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
					_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
					return HTCAPTION;
			}

			return HTCLIENT;
	}
	else if(uMsg == WM_CLOSE)
	{
		lRet = 0;
	}

	if(m_pm.MessageHandler(uMsg, wParam, lParam, lRet))
	{
		return lRet;
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void MainWnd::Notify(TNotifyUI& msg)
{
	if(0 == strcmp(msg.sType, "click"))
	{
		if( msg.pSender == m_pBtnMin ) 
		{ 
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; 
		}
		else if(msg.pSender == m_pBtnClose)
		{
			PostQuitMessage(0);
			return;
		}
		else if(msg.pSender == m_pBtnSend)
		{
			CEditUI* pEdit = (CEditUI*)m_pm.FindControl("inputedit");
			if(NULL == pEdit)
			{
				return;
			}
			OnSendCommand(pEdit->GetText());
			pEdit->SetText("");
			m_pReOut->SetFocus();
			pEdit->SetFocus();
		}
	}
	else if(0 == strcmp(msg.sType, "windowinit"))
	{
		m_pBtnClose = (CButtonUI*)m_pm.FindControl("closebtn");
		m_pBtnMin = (CButtonUI*)m_pm.FindControl("minbtn");
		m_pBtnSend = (CButtonUI*)m_pm.FindControl("sendbtn");
		m_pReOut = (CRichEditUI*)m_pm.FindControl("editoutput");

		m_pReOut->AppendText("BackMIR LS Control...\rEnter connect to use this tool");
		m_pReOut->AppendText("\rEnter @help for more information");
	}
}

void MainWnd::OnSendCommand(const char* _pszCommand)
{
	char szCmd[20] = {0};
	char szParam0[50] = {0};
	char szParam1[50] = {0};

	int nScaned = sscanf(_pszCommand, "@%s %s %s", szCmd, szParam0, szParam1);
	if(nScaned >= 1)
	{
		_strlwr(szCmd);
		_strlwr(szParam0);
		_strlwr(szParam1);

		int nCmdLen = strlen(szCmd);
		int nParam0 = strlen(szParam0);
		int nParam1 = strlen(szParam1);

		bool bParsed = false;

		if(0 == strcmp(szCmd, "new"))
		{
			if(nScaned == 3)
			{
				if(nParam0 > 0 &&
					nParam0 < 15 &&
					nParam1 > 0 &&
					nParam1 < 15)
				{
					//	OK
					bParsed = true;
				}
			}

			if(!bParsed)
			{
				m_pReOut->AppendText("\r语法:@new [account] [password]");
			}
		}
		else if(0 == strcmp(szCmd, "del"))
		{
			if(nScaned == 2)
			{
				if(nParam0 > 0 &&
					nParam0 < 15)
				{
					//	OK
					bParsed = true;
				}
			}

			if(!bParsed)
			{
				m_pReOut->AppendText("\r语法:@del [name]");
			}
		}
		else if(0 == strcmp(szCmd, "help"))
		{
			m_pReOut->AppendText("\r语法说明:命令中的[]不包含 例如@new abcde abced"
				"\r添加账户:@new [account] [password]"
				"\r删除账户:@del [name]"
				);
		}
	}
}