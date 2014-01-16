#ifndef _INC_MAINWND_
#define _INC_MAINWND_
//////////////////////////////////////////////////////////////////////////
#include <ObjBase.h>
#include <duilib\uilib.h>
#include "DataParser.h"
#ifdef _DEBUG
#pragma comment(lib, "duilib.lib")
#else
#pragma comment(lib, "duilib_d.lib")
#endif
using namespace DuiLib;
//////////////////////////////////////////////////////////////////////////
#define TIMER_CONNECTTIMEOUT	(1)
#define TIMER_VERIFYTIMEOUT		(2)

#define WM_SOCKMSG				(WM_USER + 2)

#define SAFE_DELETE(p)			if(p){delete p;p=NULL;}
//////////////////////////////////////////////////////////////////////////
class MainWnd : public CWindowWnd, public INotifyUI
{
public:
	MainWnd();
	virtual ~MainWnd();

public:
	virtual LPCTSTR GetWindowClassName() const			{return __T("MyWndCls");}
	virtual UINT GetClassStyle()const					{return CS_DBLCLKS | UI_CLASSSTYLE_FRAME;}
	virtual void OnFinalMessage(HWND hWnd)				{delete this;}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg);

private:
	void OnSendCommand(const char* _pszCommand);
	void OnSocketMsg(WPARAM _wParam, LPARAM _lParam);
	void OnTimer(UINT _uTimerID);

private:
	inline bool IsConnected()							{return m_nConnectStat == 2;}
	inline bool IsConnecting()							{return m_nConnectStat == 1;}
	inline void SetConnecting()							{m_nConnectStat = 1;}
	inline void SetConnected()							{m_nConnectStat = 2;}
	inline void ResetConnectStat()						{m_nConnectStat = 0;}

	bool SendProtoBuf(UINT _uOpcode, const char* _pData, unsigned int _uLen);

private:
	static void __stdcall OnFullMsg(const void* _pData, unsigned int _uLen);

private:
	CPaintManagerUI m_pm;
	CButtonUI* m_pBtnClose;
	CButtonUI* m_pBtnMin;
	CButtonUI* m_pBtnSend;
	CRichEditUI* m_pReOut;

	//	For net
	int m_nConnectStat;
	DataParser m_xParser;
	char m_szVerifyCode[50];
};

//////////////////////////////////////////////////////////////////////////
#endif