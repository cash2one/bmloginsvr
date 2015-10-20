// SIBHelperDlg.h : 头文件
//

#pragma once

#include "SimpleIni.h"

void ExeWaitForEnd(const char* _pszRunCmd);
BOOL IsDirectory(LPCTSTR pstrPath);
BOOL DeleteFolder(LPCTSTR pstrFolder);


// CSIBHelperDlg 对话框
class CSIBHelperDlg : public CDialog
{
// 构造
public:
	CSIBHelperDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SIBHELPER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	void OnClose();


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	bool PackSIB(const char* _pszPath, const char* _pszFileName);
	bool ExtraSIB(const char* _pszFileName, const char* _pszPath);
	bool PackLua(const char* _pszPath, const char* _pszPsw = NULL);
	bool PackINI(const char* _pszPath, const char* _pszPsw = NULL);
	bool GenMapInfo(const char* _pszPath, const char* _pszPsw = NULL);
	bool PackMap(const char* _pszDestMap, const char* _pszSrcPath, const char* _pszDestPath);

	void LoadSettings();
	void SetValue(UINT _uCtrlID, const char* _pszKey);

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();

private:
	CSimpleIniA m_xIniFile;
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
};
