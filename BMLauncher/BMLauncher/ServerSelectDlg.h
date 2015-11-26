#ifndef _INC_SERVERSELECTDLG_
#define _INC_SERVERSELECTDLG_
//////////////////////////////////////////////////////////////////////////
#include <uilib.h>
#include <string>
#include <vector>
//////////////////////////////////////////////////////////////////////////
using namespace DuiLib;
//////////////////////////////////////////////////////////////////////////
struct ServerItemInfo
{
	int nId;
	std::string xServerAddr;
	std::string xServerName;
};
typedef std::vector<ServerItemInfo> ServerItemInfoVector;
//////////////////////////////////////////////////////////////////////////
class BMServerSelectDlg : public CWindowWnd, public INotifyUI
{
public:
	BMServerSelectDlg();
	virtual ~BMServerSelectDlg();

public:
	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/)
	{
		//delete this;
	};
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg);

public:
	int LoadServerListData(const char* _pszJsonData);
	void CreateServerButtons();
	const char* GetSelectedServerAddress()		{return m_xSelectedServerAddress.c_str();}

protected:
	CPaintManagerUI m_pm;
	ServerItemInfoVector m_xServerItems;
	std::string m_xSelectedServerAddress;
};
//////////////////////////////////////////////////////////////////////////
#endif