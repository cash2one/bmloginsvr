// PotentialDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SIBHelper.h"
#include "PotentialDlg.h"
#include "../../../../CommonModule/PotentialAttribHelper.h"


// CPotentialDlg 对话框

IMPLEMENT_DYNAMIC(CPotentialDlg, CDialog)

CPotentialDlg::CPotentialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPotentialDlg::IDD, pParent)
{

}

CPotentialDlg::~CPotentialDlg()
{
}

void CPotentialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPotentialDlg, CDialog)
END_MESSAGE_MAP()


// CPotentialDlg 消息处理程序
void CPotentialDlg::OnOK()
{
	CString xPotentialBase;
	GetDlgItem(IDC_EDIT1)->GetWindowText(xPotentialBase);

	CString xPotentialOffset;
	GetDlgItem(IDC_EDIT2)->GetWindowText(xPotentialOffset);

	CString xPotentialIndex;
	GetDlgItem(IDC_EDIT9)->GetWindowText(xPotentialIndex);

	int nPotentialBase = atoi(xPotentialBase);
	int nPotentialOffset = atoi(xPotentialOffset);
	int nPotentialIndex = atoi(xPotentialIndex);
	char szValue[15] = {0};

	if(0 == nPotentialBase &&
		0 == nPotentialOffset)
	{
		GetDlgItem(IDC_EDIT3)->SetWindowText("");
	}
	else
	{
		DWORD dwValue = PotentialAttribHelper::MakePotential(nPotentialBase, nPotentialOffset, nPotentialIndex, 0);
		GetDlgItem(IDC_EDIT3)->SetWindowText(itoa(dwValue, szValue, 10));
	}

	CString xPotential;
	GetDlgItem(IDC_EDIT4)->GetWindowText(xPotential);
	DWORD dwPotential = atoi(xPotential);

	GetDlgItem(IDC_EDIT5)->SetWindowText(itoa(PotentialAttribHelper::GetPotentialBase(dwPotential), szValue, 10));
	GetDlgItem(IDC_EDIT6)->SetWindowText(itoa(PotentialAttribHelper::GetPotentialOffset(dwPotential), szValue, 10));
	GetDlgItem(IDC_EDIT7)->SetWindowText(itoa(PotentialAttribHelper::GetPotentialTime(dwPotential), szValue, 10));
	GetDlgItem(IDC_EDIT8)->SetWindowText(itoa(PotentialAttribHelper::GetPotentialIndex(dwPotential), szValue, 10));
}