#pragma once

#include "../../CommonModule/SimpleIni.h"
#include "../../CommonModule/ObjectData.h"

// CItemEditDlg 对话框

class CItemEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CItemEditDlg)

public:
	CItemEditDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CItemEditDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	bool LoadConfigFile(const char* _pszFilename);

private:
	CSimpleIniA m_xIniFile;
};
