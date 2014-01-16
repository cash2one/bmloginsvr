#include "MainWnd.h"
#include "LSControlProto/LSControlProto.pb.h"
#include "ClientSocket.h"

#ifdef _DEBUG
#pragma comment(lib, "libprotobuf_d.lib")
#else
#pragma comment(lib, "libprotobuf.lib")
#endif
//////////////////////////////////////////////////////////////////////////
MainWnd* g_pMainWnd = NULL;
ByteBuffer g_xBuffer;
//////////////////////////////////////////////////////////////////////////
MainWnd::MainWnd()
{
	m_pBtnMin = m_pBtnClose = m_pBtnSend = NULL;
	m_pReOut = NULL;
	m_nConnectStat = 0;
	g_pMainWnd = this;
	m_xParser.SetHandlePacket(&MainWnd::OnFullMsg);
	ZeroMemory(m_szVerifyCode, sizeof(m_szVerifyCode));
	g_xClientSocket.InitSocket(NULL);
}

MainWnd::~MainWnd()
{
	g_pMainWnd = NULL;
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
	else if(uMsg == WM_SOCKMSG)
	{
		OnSocketMsg(wParam, lParam);
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
	else if(uMsg == WM_TIMER)
	{
		OnTimer((UINT)wParam);
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

			if(!IsConnected())
			{
				return;
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

			if(!IsConnected())
			{
				return;
			}
		}
		else if(0 == strcmp(szCmd, "connect"))
		{
			if(nScaned == 3)
			{
				//	OK
				bParsed = true;
			}

			if(!bParsed)
			{
				m_pReOut->AppendText("\r语法:@connect [gamesvr addr][code]");
			}
			else
			{
				if(!IsConnected() &&
					!IsConnecting())
				{
					int nIP[5] = {0};
					if(5 == sscanf(szParam0, "%d.%d.%d.%d:%d", &nIP[0], &nIP[1], &nIP[2], &nIP[3], &nIP[4]))
					{
						char szIP[20];
						sprintf(szIP, "%d.%d.%d.%d", nIP[0], nIP[1], nIP[2], nIP[3]);
						g_xClientSocket.ConnectToServer(m_hWnd, szIP, nIP[4]);
						strcpy(m_szVerifyCode, szParam1);

						SetTimer(m_hWnd, TIMER_CONNECTTIMEOUT, 15000, NULL);
						m_pReOut->AppendText("\r正在连接服务器...");
						SetConnecting();
					}
				}
			}
		}
		else if(0 == strcmp(szCmd, "help"))
		{
			m_pReOut->AppendText("\r语法说明:命令中的[]不包含 例如@new abcde abced"
				"\r连接服务器:@connect [code]"
				"\r添加账户:@new [account] [password]"
				"\r删除账户:@del [name]"
				);
		}
	}
}

void MainWnd::OnTimer(UINT _uTimerID)
{
	if(_uTimerID == TIMER_CONNECTTIMEOUT)
	{
		KillTimer(m_hWnd, _uTimerID);
		ResetConnectStat();
		m_pReOut->AppendText("\r连接服务器失败");
		g_xClientSocket.DisconnectToServer();
	}
	else if(_uTimerID == TIMER_VERIFYTIMEOUT)
	{
		KillTimer(m_hWnd, _uTimerID);
		ResetConnectStat();
		g_xClientSocket.DisconnectToServer();
		m_pReOut->AppendText("\r身份验证超时");
	}
}

bool MainWnd::SendProtoBuf(UINT _uOpcode, const char* _pData, unsigned int _uLen)
{
	UINT uPacketLength = 0;
	UINT uHeadLength = 0;

	//	create head
	char szHeadBuf[1024] = {0};
	LSControlProto::LSCHead xHead;
	xHead.set_opcode(_uOpcode);
	if(!xHead.SerializeToArray(szHeadBuf, sizeof(szHeadBuf)))
	{
		return false;
	}

	uHeadLength = xHead.ByteSize();
	uPacketLength = 4 + 1 + uHeadLength + _uLen;

	g_xBuffer.Reset();
	g_xBuffer << (int)htonl(uPacketLength) << (char)uHeadLength;
	g_xBuffer.Write(szHeadBuf, uHeadLength);
	g_xBuffer.Write(_pData, _uLen);
	g_xClientSocket.SendToServer((const char*)g_xBuffer.GetBuffer(), g_xBuffer.GetLength());

	return true;
}

void MainWnd::OnSocketMsg(WPARAM _wParam, LPARAM _lParam)
{
	static char s_szBuf[1024 * 5];
	SOCKET s = (SOCKET)_wParam;

	if(WSAGETSELECTERROR(_lParam))
	{
		closesocket(s);
		m_pReOut->AppendText("\r与服务器断开连接");
		ResetConnectStat();
		return;
	}

	switch(WSAGETSELECTEVENT(_lParam))
	{
	case FD_CONNECT:
		{
			KillTimer(m_hWnd, TIMER_CONNECTTIMEOUT);
			SetConnected();
			m_pReOut->AppendText("\r连接服务器成功，正在进行身份验证");

			SetTimer(m_hWnd, TIMER_VERIFYTIMEOUT, 15000, NULL);
			//	registe
			g_xBuffer.Reset();
			g_xBuffer << (int)0;
			g_xBuffer << (int)10001;
			g_xBuffer << (int)101;
			g_xBuffer << (int)0;
			const char* pszAddr = "127.0.0.1:1111";
			g_xBuffer << (char)strlen(pszAddr);
			g_xBuffer.Write(pszAddr, strlen(pszAddr));
			//	update length
			int len = htonl(g_xBuffer.GetLength());
			unsigned char* pForceWritePtr = const_cast<unsigned char*>(g_xBuffer.GetBuffer());
			*(int*)pForceWritePtr = len;
			g_xClientSocket.SendToServer((const char*)g_xBuffer.GetBuffer(), g_xBuffer.GetLength());

			//	verify
			LSControlProto::LSCCtrlVerifyReq req;
			req.set_verifycode(m_szVerifyCode);
			char szBuf[1024];
			if(req.SerializeToArray(szBuf, sizeof(szBuf)))
			{
				SendProtoBuf(LSControlProto::PKG_RegistAccountReq, szBuf, req.ByteSize());
			}
		}break;
	case FD_READ:
		{
			int nRead = recv(s, s_szBuf, sizeof(s_szBuf), 0);
			if(nRead != 0)
			{
				m_xParser.OnSocketData(s_szBuf, nRead);
			}
		}break;
	case FD_CLOSE:
		{
			closesocket(s);
			m_pReOut->AppendText("\r与服务器断开连接");
			ResetConnectStat();
		}break;
	}
}

void __stdcall MainWnd::OnFullMsg(const void* _pData, unsigned int _uLen)
{
	if(NULL == g_pMainWnd)
	{
		return;
	}

	//	parse
	const char* pData = (const char*)_pData;
	char cProtoHeadLength = *(pData + 4);

	LSControlProto::LSCHead head;
	if(!head.ParseFromArray(pData + 5, cProtoHeadLength))
	{
		return;
	}

	const char* pLeftData = pData + 4 + 1 + cProtoHeadLength;
	int nLeftLength = _uLen - 4 - 4 - cProtoHeadLength;

	CRichEditUI* pOutputWnd = g_pMainWnd->m_pReOut;

	switch(head.opcode())
	{
	case LSControlProto::PKG_RegistAccountAck:
		{
			LSControlProto::LSCRegistAccountAck ack;
			if(!ack.ParseFromArray(pLeftData, nLeftLength))
			{
				return;
			}
			if(ack.result())
			{
				pOutputWnd->AppendText("\r添加账号成功");
			}
			else
			{
				pOutputWnd->AppendText("\r添加账号失败");
			}
		}break;
	case LSControlProto::PKG_CtrlVerifyAck:
		{
			LSControlProto::LSCCtrlVerifyAck ack;
			if(!ack.result())
			{
				pOutputWnd->AppendText("\r验证失败");
				g_xClientSocket.DisconnectToServer();
				g_pMainWnd->ResetConnectStat();
			}
			else
			{
				pOutputWnd->AppendText("\r验证成功");
				KillTimer(m_hWnd, TIMER_VERIFYTIMEOUT);
			}
		}break;
	}
}