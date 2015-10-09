// BMLauncherDlg.h : 头文件
//

#pragma once

#include "CDuiDialog.h"
#include <string>

using std::string;

// CBMLauncherDlg 对话框
class CBMLauncherDlg : public CDuiDialog
{
// 构造
public:
	CBMLauncherDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BMLAUNCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	virtual void Notify(DuiLib::TNotifyUI& msg);

public:
	void SwitchTabLayout(int _nTabPage);
	void DoLogin(int _nPage);
	void DoDiagnosis();
	void DoMailVerify();
	void OnMailVerifyResult(const char* _pData, size_t _uLen);
	void DoRegAccount();
	void OnRegAccountResult(const char* _pData, size_t _uLen);
	void OnGsAddrResult(const char *_pData, size_t _uLen);
	void OnOnlineRoomClicked();
	void OnGetBattleNetAddress();

	void LoadConfig();
	void SaveConfig();
	void DownloadConfigFile();

	void ErrorMsgBox(const char* _pszText)
	{
		MessageBox(_pszText, "错误", MB_ICONERROR | MB_OK);
	}

// 实现
protected:
	HICON m_hIcon;

	//	http请求地址
	string m_xRegHttpAddr;
	//	游戏大厅的请求地址
	string m_xGameRoomHttpAddr;
	//	战网IP
	string m_xBattleNetIP;
	string m_xBattleNetPort;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
