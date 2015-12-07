// BMCrashReportDlg.h : 头文件
//

#pragma once

#include <string>
#include "HyperLink.h"

// CBMCrashReportDlg 对话框
class CBMCrashReportDlg : public CDialog
{
// 构造
public:
	CBMCrashReportDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BMCRASHREPORT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void ViewReport();

protected:


	std::string m_xDumpfilePath; 
	CHyperLink m_xHyperLinkOpenReportDirectory;
	CHyperLink m_xHyperLinkViewReport;

public:
	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT OnHyperLinkClicked(WPARAM wParam, LPARAM lParam);
};
