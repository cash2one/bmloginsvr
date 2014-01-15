#ifndef _INC_MAINWND_
#define _INC_MAINWND_
//////////////////////////////////////////////////////////////////////////
#include <ObjBase.h>
#include <duilib\uilib.h>
#ifdef _DEBUG
#pragma comment(lib, "duilib.lib")
#else
#pragma comment(lib, "duilib_d.lib")
#endif
using namespace DuiLib;
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

private:
	CPaintManagerUI m_pm;
	CButtonUI* m_pBtnClose;
	CButtonUI* m_pBtnMin;
	CButtonUI* m_pBtnSend;
	CRichEditUI* m_pReOut;
};

//////////////////////////////////////////////////////////////////////////
#endif