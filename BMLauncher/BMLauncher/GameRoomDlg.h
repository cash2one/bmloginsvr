#pragma once

#include "PingThread.h"
#include <string>
using std::string;

#define WM_USER_PINGRESULT (WM_USER + 1)

// GameRoomDlg 对话框

class GameRoomDlg : public CDialog
{
	DECLARE_DYNAMIC(GameRoomDlg)

public:
	GameRoomDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GameRoomDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_GAMEROOM };

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

public:
	void UpdateServerList();
	void OnServerListResult(const char* _pData, size_t _uLen);
	void OnGsAddrResult(const char *_pData, size_t _uLen);
	void DoLoginServer(int _nServerID, const char* _pszPassword);
	void ErrorMsgBox(const char* _pszText)
	{
		MessageBox(_pszText, "错误", MB_ICONERROR | MB_OK);
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnDestroy();
	afx_msg LRESULT OnUserMsgPingResult(WPARAM _wParam, LPARAM _lParam);
	DECLARE_MESSAGE_MAP()

public:
	string m_xRoomRequestUrl;
	int m_nLoginServerID;
	string m_xLoginServerPsw;

private:
	CListCtrl m_xListCtrl;
	CEdit m_xEditPassword;
	PingThread m_xPingThread;

public:
	afx_msg void OnBnJoinClicked();
	afx_msg void OnBnRefreshClicked();
};
