// ItemEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SIBHelper.h"
#include "ItemEditDlg.h"


// CItemEditDlg 对话框

IMPLEMENT_DYNAMIC(CItemEditDlg, CDialog)

CItemEditDlg::CItemEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemEditDlg::IDD, pParent)
{

}

CItemEditDlg::~CItemEditDlg()
{
}

void CItemEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CItemEditDlg::OnInitDialog()
{
	return __super::OnInitDialog();
}

//////////////////////////////////////////////////////////////////////////
bool CItemEditDlg::LoadConfigFile(const char* _pszFilename)
{
	if(!m_xIniFile.LoadFile(_pszFilename))
	{
		return false;
	}

	return false;
}


BEGIN_MESSAGE_MAP(CItemEditDlg, CDialog)
END_MESSAGE_MAP()


// CItemEditDlg 消息处理程序
