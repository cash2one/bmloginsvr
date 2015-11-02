#pragma once


// CPotentialDlg 对话框

class CPotentialDlg : public CDialog
{
	DECLARE_DYNAMIC(CPotentialDlg)

public:
	CPotentialDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPotentialDlg();

// 对话框数据
	enum { IDD = IDD_DLG_POTENTIAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};
