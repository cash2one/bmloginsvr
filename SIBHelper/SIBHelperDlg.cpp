// SIBHelperDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SIBHelper.h"
#include "SIBHelperDlg.h"
#include ".\sibhelperdlg.h"
#include "SimpleIni.h"
#include <ZipArchive.h>
#include "ByteBuffer.h"
#include "../../CommonModule/DataEncryptor.h"
#include "MirMap.h"

#include <Shlwapi.h>
#include <list>
#include <set>
#include <direct.h>

#pragma comment(lib, "shlwapi.lib")

#ifdef _DEBUG
#pragma comment(lib, "ZipArchive_D.lib")
#else
#pragma comment(lib, "ZipArchive.lib")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSIBHelperDlg 对话框



CSIBHelperDlg::CSIBHelperDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSIBHelperDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSIBHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSIBHelperDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CSIBHelperDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CSIBHelperDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CSIBHelperDlg::OnBnClickedButton9)
END_MESSAGE_MAP()


// CSIBHelperDlg 消息处理程序

BOOL CSIBHelperDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	char szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, sizeof(szPath) - 1);
	PathRemoveFileSpec(szPath);
	strcat(szPath, "\\cfg.ini");

	if(!PathFileExists(szPath))
	{
		HANDLE hFile = CreateFile(szPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		CloseHandle(hFile);
	}

	LoadSettings();
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CSIBHelperDlg::LoadSettings()
{
	char szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, sizeof(szPath) - 1);
	PathRemoveFileSpec(szPath);
	strcat(szPath, "\\cfg.ini");

	CEdit* pEdit = NULL;
	m_xIniFile.LoadFile(szPath);
	//	解包 sib文件路径
	pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT1));
	const char* pszValue = NULL;

	if(pEdit)
	{
		pszValue = m_xIniFile.GetValue("SETTINGS", "EXTRA_SIBPATH");

		if(pszValue)
		{
			pEdit->SetWindowText(pszValue);
		}
	}

	pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT2));
	if(pEdit)
	{
		pszValue = m_xIniFile.GetValue("SETTINGS", "EXTRA_PATH");

		if(pszValue)
		{
			pEdit->SetWindowText(pszValue);
		}
	}

	pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT3));
	if(pEdit)
	{
		pszValue = m_xIniFile.GetValue("SETTINGS", "PACK_PATH");

		if(pszValue)
		{
			pEdit->SetWindowText(pszValue);
		}
	}

	pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT4));
	if(pEdit)
	{
		pszValue = m_xIniFile.GetValue("SETTINGS", "PACK_SIBPATH");

		if(pszValue)
		{
			pEdit->SetWindowText(pszValue);
		}
	}

	pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT5));
	if(pEdit)
	{
		pszValue = m_xIniFile.GetValue("SETTINGS", "SCRIPT_PATH");

		if(pszValue)
		{
			pEdit->SetWindowText(pszValue);
		}
	}

	pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT5));
	if(pEdit)
	{
		pszValue = m_xIniFile.GetValue("SETTINGS", "SCRIPT_PATH");

		if(pszValue)
		{
			pEdit->SetWindowText(pszValue);
		}
	}

	pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT7));
	if(pEdit)
	{
		pszValue = m_xIniFile.GetValue("SETTINGS", "SCRIPT_PSW");

		if(pszValue)
		{
			pEdit->SetWindowText(pszValue);
		}
	}

	pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT6));
	if(pEdit)
	{
		pszValue = m_xIniFile.GetValue("SETTINGS", "MAPINFO_PATH");

		if(pszValue)
		{
			pEdit->SetWindowText(pszValue);
		}
	}

	pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT8));
	if(pEdit)
	{
		pszValue = m_xIniFile.GetValue("SETTINGS", "MAPINFO_PSW");

		if(pszValue)
		{
			pEdit->SetWindowText(pszValue);
		}
	}

	pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT9));
	if(pEdit)
	{
		pszValue = m_xIniFile.GetValue("SETTINGS", "PACKMAP_MAP");

		if(pszValue)
		{
			pEdit->SetWindowText(pszValue);
		}
	}

	pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT10));
	if(pEdit)
	{
		pszValue = m_xIniFile.GetValue("SETTINGS", "PACKMAP_SRC");

		if(pszValue)
		{
			pEdit->SetWindowText(pszValue);
		}
	}

	pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT11));
	if(pEdit)
	{
		pszValue = m_xIniFile.GetValue("SETTINGS", "PACKMAP_DEST");

		if(pszValue)
		{
			pEdit->SetWindowText(pszValue);
		}
	}
}

void CSIBHelperDlg::SetValue(UINT _uCtrlID, const char* _pszKey)
{
	CString xValue;
	CEdit* pEdit = static_cast<CEdit*>(GetDlgItem(_uCtrlID));

	if(pEdit)
	{
		pEdit->GetWindowText(xValue);
		m_xIniFile.SetValue("SETTINGS", _pszKey, xValue);
	}
}

void CSIBHelperDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

BOOL CSIBHelperDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_CLOSE)
	{
		char szPath[MAX_PATH];
		GetModuleFileName(NULL, szPath, sizeof(szPath) - 1);
		PathRemoveFileSpec(szPath);
		strcat(szPath, "\\cfg.ini");

		m_xIniFile.SaveFile(szPath);
	}

	return __super::PreTranslateMessage(pMsg);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSIBHelperDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CSIBHelperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSIBHelperDlg::OnOK()
{
	bool bExePack = false;
	bool bExeExtra = false;
	bool bExeLua = false;
	bool bGenMapInfo = false;
	bool bPackMap = false;
	bool bPackIni = false;

	CButton* pBtn = static_cast<CButton*>(GetDlgItem(IDC_CHECK1));

	if(pBtn)
	{
		if(pBtn->GetCheck())
		{
			bExeExtra = true;
		}
	}

	pBtn = static_cast<CButton*>(GetDlgItem(IDC_CHECK2));

	if(pBtn)
	{
		if(pBtn->GetCheck())
		{
			bExePack = true;
		}
	}

	pBtn = static_cast<CButton*>(GetDlgItem(IDC_CHECK3));

	if(pBtn)
	{
		if(pBtn->GetCheck())
		{
			bExeLua = true;
		}
	}

	pBtn = static_cast<CButton*>(GetDlgItem(IDC_CHECK4));

	if(pBtn)
	{
		if(pBtn->GetCheck())
		{
			bGenMapInfo = true;
		}
	}

	pBtn = static_cast<CButton*>(GetDlgItem(IDC_CHECK5));

	if(pBtn)
	{
		if(pBtn->GetCheck())
		{
			bPackMap = true;
		}
	}

	pBtn = static_cast<CButton*>(GetDlgItem(IDC_CHECK7));

	if(pBtn)
	{
		if(pBtn->GetCheck())
		{
			bPackIni = true;
		}
	}

	CString xParam0;
	CString xParam1;
	CString xParam2;

	if(bExePack)
	{
		CEdit* pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT3));
		if(pEdit)
		{
			pEdit->GetWindowText(xParam0);
		}
		pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT4));
		if(pEdit)
		{
			pEdit->GetWindowText(xParam1);
		}
		
		PackSIB(xParam0, xParam1);
	}
	if(bExeExtra)
	{
		CEdit* pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT1));
		if(pEdit)
		{
			pEdit->GetWindowText(xParam0);
		}
		pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT2));
		if(pEdit)
		{
			pEdit->GetWindowText(xParam1);
		}

		ExtraSIB(xParam0, xParam1);
	}
	if(bExeLua)
	{
		CEdit* pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT5));
		if(pEdit)
		{
			pEdit->GetWindowText(xParam0);
		}
		pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT7));
		if(pEdit)
		{
			pEdit->GetWindowText(xParam1);
		}

		CButton* pEncryptBjt = (CButton*)GetDlgItem(IDC_CHECK6);

		if(PackLua(xParam0, xParam1.IsEmpty() ? NULL : xParam1, pEncryptBjt->GetCheck() != 0))
		{
			AfxMessageBox("打包Lua脚本完成");
		}
	}
	if(bGenMapInfo)
	{
		CEdit* pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT6));
		if(pEdit)
		{
			pEdit->GetWindowText(xParam0);
		}
		pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT8));
		if(pEdit)
		{
			pEdit->GetWindowText(xParam1);
		}

		GenMapInfo(xParam0, xParam1.IsEmpty() ? NULL : xParam1);
	}
	if(bPackMap)
	{
		CEdit* pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT9));
		if(pEdit)
		{
			pEdit->GetWindowText(xParam0);
		}
		pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT10));
		if(pEdit)
		{
			pEdit->GetWindowText(xParam1);
		}
		pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT11));
		if(pEdit)
		{
			pEdit->GetWindowText(xParam2);
		}

		PackMap(xParam0, xParam1, xParam2);
	}
	if(bPackIni)
	{
		CEdit* pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT5));
		if(pEdit)
		{
			pEdit->GetWindowText(xParam0);
		}
		pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT7));
		if(pEdit)
		{
			pEdit->GetWindowText(xParam1);
		}

		PackINI(xParam0, xParam1.IsEmpty() ? NULL : xParam1);
	}
}

bool CSIBHelperDlg::PackSIB(const char* _pszPath, const char* _pszFileName)
{
	char szCmd[MAX_PATH * 2];

	if(strlen(_pszPath) > 0 &&
		strlen(_pszFileName) > 0)
	{
		sprintf(szCmd, "SIBPacker_tt.exe %s %s", _pszPath, _pszFileName);

		UINT uRet = WinExec(szCmd, SW_SHOWNORMAL);

		if(uRet <= 31)
		{
			sprintf(szCmd, "Error code: %d",
				uRet);
			AfxMessageBox(szCmd);
		}
	}
	return true;
}

bool CSIBHelperDlg::ExtraSIB(const char* _pszFileName, const char* _pszPath)
{
	char szCmd[MAX_PATH * 2];

	if(strlen(_pszPath) > 0 &&
		strlen(_pszFileName) > 0)
	{
		sprintf(szCmd, "SIBExtractor_tt.exe %s %s", _pszFileName, _pszPath);

		UINT uRet = WinExec(szCmd, SW_SHOWNORMAL);

		if(uRet <= 31)
		{
			sprintf(szCmd, "Error code: %d",
				uRet);
			AfxMessageBox(szCmd);
		}
	}
	return true;
}

bool CSIBHelperDlg::PackLua(const char* _pszPath, const char* _pszPsw /* = NULL */, bool _bEncryptBjt /* = false */)
{
	if(!PathFileExists(_pszPath))
	{
		return false;
	}

	CFileFind xFind;
	CString xFindPath = _pszPath;

	if(xFindPath.IsEmpty())
	{
		return false;
	}

	BOOL bNeedEncrypt = FALSE;
	if(_pszPsw != NULL)
	{
		if(strlen(_pszPsw) > 0)
		{
			bNeedEncrypt = TRUE;
			SetValue(IDC_EDIT7, "SCRIPT_PSW");
		}
	}

	xFindPath += "\\*.lua";

	BOOL bExist = xFind.FindFile(xFindPath);
	CString xRunParam;
	PROCESS_INFORMATION pi;
	STARTUPINFO si = {sizeof(si)};
	si.wShowWindow = SW_HIDE;
	std::list<CString> xAllBBTList;
	std::vector<CString> xAllFiles;
	CString xWorkingPath;

	while(bExist)
	{
		bExist = xFind.FindNextFile();

		if(!xFind.IsDots() &&
			!xFind.IsDirectory())
		{
			//	generate bbt
			xRunParam.Format("luac.exe -o %s\\%s.bbt %s",
				xFind.GetRoot(), xFind.GetFileTitle(), xFind.GetFilePath());

			if(xWorkingPath.IsEmpty())
			{
				xWorkingPath = xFind.GetRoot();
			}

			BOOL bRet = CreateProcess(NULL, xRunParam.GetBuffer(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
			if(TRUE == bRet)
			{
				WaitForSingleObject(pi.hThread, INFINITE);
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);

				xRunParam.Format("%s\\%s.bbt",
					xFind.GetRoot(), xFind.GetFileTitle());
				xAllBBTList.push_back(xRunParam);
			}
			else
			{
				AfxMessageBox("Can't execute Luac.exe...");
			}

			//	generate bjt
			xRunParam.Format("luajit.exe -b %s %s/%s.bjt",
				xFind.GetFilePath(), xFind.GetRoot(), xFind.GetFileTitle());

			if(xWorkingPath.IsEmpty())
			{
				xWorkingPath = xFind.GetRoot();
			}

			bRet = CreateProcess(NULL, xRunParam.GetBuffer(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
			if(TRUE == bRet)
			{
				WaitForSingleObject(pi.hThread, INFINITE);
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);

				xRunParam.Format("%s\\%s.bjt",
					xFind.GetRoot(), xFind.GetFileTitle());
				xAllBBTList.push_back(xRunParam);
			}
			else
			{
				AfxMessageBox("Can't execute luajit.exe...");
			}

			xAllFiles.push_back(xFind.GetFileTitle());
		}
	}
	//	remove all server file
	char szPath[MAX_PATH];

	for(int i = 0; i < xAllFiles.size(); ++i)
	{
		char szFirst5[6] = {0};
		memcpy(szFirst5, (const char*)xAllFiles[i], 5);
		if(0 == stricmp(szFirst5, "react") ||
			0 == stricmp(szFirst5, "quest"))
		{
			//	client, do not remove
		}
		else
		{
			//	server file, remove bbt
			sprintf(szPath, "%s/%s.bbt",
				_pszPath, (const char*)xAllFiles[i]);
			DeleteFile(szPath);
		}
	}

	//	encrypt all bjt file
	if(_bEncryptBjt)
	{
		for(int i = 0; i < xAllFiles.size(); ++i)
		{
			char szFirst5[6] = {0};
			memcpy(szFirst5, (const char*)xAllFiles[i], 5);
			if(0 == stricmp(szFirst5, "react") ||
				0 == stricmp(szFirst5, "quest"))
			{
				//	client, do not encrypt
			}
			else
			{
				//	server file, encrypt
				sprintf(szPath, "%s/%s.bjt",
					_pszPath, (const char*)xAllFiles[i]);
				DataEncryptor::DoEncryptFile(szPath);
			}
		}
	}

	/*for(int i = 0; i < 200; ++i)
	{
		sprintf(szPath, "%s/scene%d.bbt",
			xWorkingPath, i);
		DeleteFile(szPath);
	}
	sprintf(szPath, "%s/world.bbt",
		xWorkingPath);
	DeleteFile(szPath);
	sprintf(szPath, "%s/db.bbt",
		xWorkingPath);
	DeleteFile(szPath);*/

	bool bResult = true;


	//	zip bbt
	if(bNeedEncrypt)
	{
		xRunParam.Format("7z a -tzip -p%s %s\\dog.idx %s\\*.bbt",
			_pszPsw, xWorkingPath, xWorkingPath);
	}
	else
	{
		xRunParam.Format("7z a -tzip %s\\dog.idx %s\\*.bbt",
			xWorkingPath, xWorkingPath);
	}
	
	BOOL bRet = CreateProcess(NULL, xRunParam.GetBuffer(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	if(TRUE == bRet)
	{
		WaitForSingleObject(pi.hThread, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else
	{
		AfxMessageBox("Can't execute 7z.exe...");
		bResult = false;
	}

	//	zip bjt
	if(bNeedEncrypt)
	{
		xRunParam.Format("7z a -tzip -p%s %s\\dog.idx %s\\*.bjt",
			_pszPsw, xWorkingPath, xWorkingPath);
	}
	else
	{
		xRunParam.Format("7z a -tzip %s\\dog.idx %s\\*.bjt",
			xWorkingPath, xWorkingPath);
	}

	bRet = CreateProcess(NULL, xRunParam.GetBuffer(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	if(TRUE == bRet)
	{
		WaitForSingleObject(pi.hThread, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else
	{
		AfxMessageBox("Can't execute 7z.exe...");
		bResult = false;
	}

	if(!xAllBBTList.empty())
	{
		std::list<CString>::const_iterator begIter = xAllBBTList.begin();
		std::list<CString>::const_iterator endIter = xAllBBTList.end();

		for(begIter;
			begIter != endIter;
			++begIter)
		{
			DeleteFile(*begIter);
		}
	}

	return bResult;
}

bool CSIBHelperDlg::PackINI(const char* _pszPath, const char* _pszPsw /* = NULL */)
{
	if(!PathFileExists(_pszPath))
	{
		return false;
	}

	CFileFind xFind;
	CString xFindPath = _pszPath;

	if(xFindPath.IsEmpty())
	{
		return false;
	}

	CString xRunParam;
	BOOL bNeedEncrypt = FALSE;
	if(_pszPsw != NULL)
	{
		if(strlen(_pszPsw) > 0)
		{
			bNeedEncrypt = TRUE;
			SetValue(IDC_EDIT7, "SCRIPT_PSW");
		}
	}

	//	encrypt itemgrade and suit
	{
		char szPath[MAX_PATH];
		char szDest[MAX_PATH];

		//	itemgrade
		sprintf(szPath, "%s/itemgrade.ini", _pszPath);
		sprintf(szDest, "%s/sparta.bmd", _pszPath);
		if(PathFileExists(szPath))
		{
			if(CopyFile(szPath, szDest, FALSE))
			{
				DataEncryptor::DoEncryptFile(szDest);
			}
		}

		//	suit
		sprintf(szPath, "%s/suit.ini", _pszPath);
		sprintf(szDest, "%s/aegean.bmd", _pszPath);
		if(PathFileExists(szPath))
		{
			if(CopyFile(szPath, szDest, FALSE))
			{
				DataEncryptor::DoEncryptFile(szDest);
			}
		}
	}

	if(bNeedEncrypt)
	{
		xRunParam.Format("7z a -tzip -p%s %s\\Mouse.idx %s\\*.ini %s\\*.bmd",
			_pszPsw, _pszPath, _pszPath, _pszPath);
	}
	else
	{
		xRunParam.Format("7z a -tzip %s\\Mouse.idx %s\\*.ini %s\\*.bmd",
			_pszPath, _pszPath, _pszPath);
	}

	PROCESS_INFORMATION pi;
	STARTUPINFO si = {sizeof(si)};
	BOOL bRet = CreateProcess(NULL, xRunParam.GetBuffer(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	if(TRUE == bRet)
	{
		WaitForSingleObject(pi.hThread, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else
	{
		AfxMessageBox("Can't execute 7z.exe...");
	}

	return true;
}

bool CSIBHelperDlg::GenMapInfo(const char* _pszPath, const char* _pszPsw /* = NULL */)
{
	if(!PathFileExists(_pszPath))
	{
		return false;
	}

	//	Load config data
	ByteBuffer xBufRunMap;
	xBufRunMap.Reset();

	int nRet = GetPrivateProfileInt("LOADMAP", "TOTAL", 0, _pszPath);
	if(nRet == 0)
	{
		//return false;
	}

	xBufRunMap << nRet;

	char szName[20];
	szName[0] = 0;
	char szKey[5];

	for(int i = 0; i <  nRet; ++i)
	{
		szName[0] = 0;
		itoa(i, szKey, 10);
		GetPrivateProfileString("LOADMAP", szKey, "", szName, sizeof(szName), _pszPath);
		xBufRunMap << i << szName;
	}
	//	instance map
	ByteBuffer xBufInstanceMap;
	xBufInstanceMap.Reset();

	nRet = GetPrivateProfileInt("INSTANCEMAP", "TOTAL", 0, _pszPath);
	if(nRet == 0)
	{
		//return false;
	}

	xBufInstanceMap << nRet;

	for(int i = 100; i <  100 + nRet; ++i)
	{
		szName[0] = 0;
		itoa(i, szKey, 10);
		GetPrivateProfileString("INSTANCEMAP", szKey, "", szName, sizeof(szName), _pszPath);
		xBufInstanceMap << i << szName;
	}

	//////////////////////////////////////////////////////////////////////////

	CFileFind xFind;
	CString xFindPath = _pszPath;

	if(xFindPath.IsEmpty())
	{
		return false;
	}

	BOOL bNeedEncrypt = FALSE;
	if(_pszPsw != NULL)
	{
		if(strlen(_pszPsw) > 0)
		{
			bNeedEncrypt = TRUE;
			SetValue(IDC_EDIT8, "MAPINFO_PSW");
		}
	}

	CZipArchive xZipFile;
	bool bRet = false;
	char szWorkingPath[MAX_PATH];
	strcpy(szWorkingPath, _pszPath);
	PathRemoveFileSpec(szWorkingPath);

	char szZipFile[MAX_PATH];
	strcpy(szZipFile, szWorkingPath);
	strcat(szZipFile, "\\cat.idx");

	if(PathFileExists(szZipFile))
	{
		DeleteFile(szZipFile);

		xZipFile.Open(szZipFile, 0x0004);
		if(bNeedEncrypt)
		{
			xZipFile.SetPassword(_pszPsw);
		}
		xZipFile.Close();
	}
	else
	{
		xZipFile.Open(szZipFile, 0x0004);
		if(bNeedEncrypt)
		{
			xZipFile.SetPassword(_pszPsw);
		}
		xZipFile.Close();
	}

	if(xZipFile.Open(szZipFile))
	{
		if(bNeedEncrypt)
		{
			xZipFile.SetPassword(_pszPsw);
		}

		char szFile[20];
		strcpy(szFile, "runmap");
		CZipFileHeader zHeaderRun;
		zHeaderRun.SetFileName(szFile);
		xZipFile.OpenNewFile(zHeaderRun);
		xZipFile.WriteNewFile(xBufRunMap.GetBuffer(), xBufRunMap.GetLength());
		xZipFile.CloseNewFile();

		strcpy(szFile, "instancemap");
		CZipFileHeader zHeaderIns;
		zHeaderIns.SetFileName(szFile);
		xZipFile.OpenNewFile(zHeaderIns);
		xZipFile.WriteNewFile(xBufInstanceMap.GetBuffer(), xBufInstanceMap.GetLength());
		xZipFile.CloseNewFile();

		bRet = true;
		xZipFile.Close();
	}

	if(bRet)
	{
		MessageBox("OK", "MSG");
	}
	else
	{
		MessageBox("FAILED", "MSG");
	}

	return true;
}

void CSIBHelperDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//	浏览SIB文件
	CString xFilePath;

	CFileDialog xFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, "SIB Files (*.sib)|*.sib|All Files (*.*)|*.*||", NULL);
	if(xFileDlg.DoModal())
	{
		xFilePath = xFileDlg.GetPathName();
	}

	if(!xFilePath.IsEmpty())
	{
		CEdit* pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT1));

		if(pEdit)
		{
			pEdit->SetWindowText(xFilePath);
		}

		SetValue(IDC_EDIT1, "EXTRA_SIBPATH");
	}
}

void CSIBHelperDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//	浏览SIB释放目录
	char szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "请选择需要释放的目录：";   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		static_cast<CEdit*>(GetDlgItem(IDC_EDIT2))->SetWindowText(szPath);

		SetValue(IDC_EDIT2, "EXTRA_PATH");
	}
}

void CSIBHelperDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	//	浏览SIB打包目录
	char szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "请选择需要打包的目录：";   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		static_cast<CEdit*>(GetDlgItem(IDC_EDIT3))->SetWindowText(szPath);

		SetValue(IDC_EDIT3, "PACK_PATH");
	}
}

void CSIBHelperDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString xFilePath;

	CFileDialog xFileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, "SIB Files (*.sib)|*.sib||", NULL);
	if(xFileDlg.DoModal())
	{
		xFilePath = xFileDlg.GetPathName();

		if(xFileDlg.GetFileExt() != "sib")
		{
			xFilePath += ".sib";
		}
	}

	if(!xFilePath.IsEmpty())
	{
		CEdit* pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT4));

		if(pEdit)
		{
			pEdit->SetWindowText(xFilePath);

			SetValue(IDC_EDIT4, "PACK_SIBPATH");
		}
	}
}

void CSIBHelperDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//	浏览LUA脚本目录
	char szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "请选择Lua文件的目录：";   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		static_cast<CEdit*>(GetDlgItem(IDC_EDIT5))->SetWindowText(szPath);

		SetValue(IDC_EDIT5, "SCRIPT_PATH");
	}
}

void CSIBHelperDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	char szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, sizeof(szPath) - 1);
	PathRemoveFileSpec(szPath);
	strcat(szPath, "\\cfg.ini");

	m_xIniFile.SaveFile(szPath);

	OnCancel();
}

void CSIBHelperDlg::OnClose()
{
	char szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, sizeof(szPath) - 1);
	PathRemoveFileSpec(szPath);
	strcat(szPath, "\\cfg.ini");

	m_xIniFile.SaveFile(szPath);

	__super::OnClose();
}

void CSIBHelperDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	//	浏览地图配置文件
	CString xFilePath;

	CFileDialog xFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, "INI Files (*.ini)|*.ini|All Files (*.*)|*.*||", NULL);
	if(xFileDlg.DoModal())
	{
		xFilePath = xFileDlg.GetPathName();
	}

	if(!xFilePath.IsEmpty())
	{
		CEdit* pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT6));

		if(pEdit)
		{
			pEdit->SetWindowText(xFilePath);
		}

		SetValue(IDC_EDIT6, "MAPINFO_PATH");
	}
}

void CSIBHelperDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CString xFilePath;

	CFileDialog xFileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, "MAP Files (*.map)|*.map||", NULL);
	if(xFileDlg.DoModal())
	{
		xFilePath = xFileDlg.GetPathName();

		if(xFileDlg.GetFileExt() != "map")
		{
			xFilePath += ".map";
		}
	}

	if(!xFilePath.IsEmpty())
	{
		CEdit* pEdit = static_cast<CEdit*>(GetDlgItem(IDC_EDIT9));

		if(pEdit)
		{
			pEdit->SetWindowText(xFilePath);

			SetValue(IDC_EDIT9, "PACKMAP_MAP");
		}
	}
}

void CSIBHelperDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	char szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "请选择Lua文件的目录：";   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		static_cast<CEdit*>(GetDlgItem(IDC_EDIT10))->SetWindowText(szPath);

		SetValue(IDC_EDIT10, "PACKMAP_SRC");
	}
}

void CSIBHelperDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	char szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "请选择Lua文件的目录：";   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		static_cast<CEdit*>(GetDlgItem(IDC_EDIT11))->SetWindowText(szPath);

		SetValue(IDC_EDIT11, "PACKMAP_DEST");
	}
}

bool CSIBHelperDlg::PackMap(const char* _pszDestMap, const char* _pszSrcPath, const char* _pszDestPath)
{
	//char szT[MAX_PATH];
	//GetCurrentDirectory(MAX_PATH, szT);
	//AfxMessageBox(szT);
	if(NULL == _pszDestMap ||
		NULL == _pszSrcPath ||
		NULL == _pszDestPath)
	{
		return false;
	}

	if(strlen(_pszDestPath) == 0 ||
		strlen(_pszDestMap) == 0 ||
		strlen(_pszSrcPath) == 0)
	{
		return false;
	}

	char szWorkingPath[MAX_PATH];
	strcpy(szWorkingPath, _pszDestPath);
	strcat(szWorkingPath, "/working");

	if(!PathFileExists(szWorkingPath))
	{
		mkdir(szWorkingPath);
	}

	char szWorkingIni[MAX_PATH];
	sprintf(szWorkingIni, "%s/map.ini", szWorkingPath);

	TileMap xMap;
	if(!xMap.LoadMap(_pszDestMap))
	{
		return false;
	}

	MapInfo info;
	xMap.GetMapInfo(info);
	std::set<int> xSets;
	MapTile* pTile = NULL;
	char szBuf[MAX_PATH];
	bool bHaveItems = false;

	//	tiles
	for(int i = 0; i < info.nRow; ++i)
	{
		for(int j = 0; j < info.nCol; ++j)
		{
			pTile = xMap.GetCellData(i, j);

			if((pTile->wBkImg & (~0x8000)) > 0)
			{
				int nValue = (pTile->wBkImg & (~0x8000)) - 1;
				if(nValue > 30000)
				{
					//assert("b" == NULL);
				}
				xSets.insert(nValue);
			}
		}
	}

	if(!xSets.empty())
	{
		FILE* pfile = fopen(szWorkingIni, "w");
		if(pfile == NULL)
		{
			return false;
		}

		fprintf(pfile, "list b:\n");
		bHaveItems = true;

		for(std::set<int>::const_iterator begiter = xSets.begin();
			begiter != xSets.end();
			++begiter)
		{
			sprintf(szBuf, "%d\n", *begiter);
			fprintf(pfile, szBuf);
		}

		fclose(pfile);
	}

	if(bHaveItems)
	{
		CString xSrcFile = _pszSrcPath;
		xSrcFile += "/tiles.wil";

		CString xDestFile = _pszDestPath;
		xDestFile += "/tiles.sib";

		CString xOutputDir = szWorkingPath;
		xOutputDir += "/tiles";
		if(!PathFileExists(xOutputDir))
		{
			mkdir(xOutputDir);
		}

		CString xOutputFile = szWorkingPath;
		xOutputFile += "/tiles.sib";

		if(PathFileExists(xSrcFile) &&
			PathFileExists(xDestFile))
		{
			//	extract wil
			char szCmd[512] = {0};

			//	extract sib
			sprintf(szCmd, "sibextractor_tt.exe \"%s\" \"%s\"",
				xDestFile, xOutputDir);
			ExeWaitForEnd(szCmd);

			sprintf(szCmd, "wilextractorex.exe \"%s\" \"%s\" \"%s\"",
				szWorkingIni, _pszSrcPath, xOutputDir);
			ExeWaitForEnd(szCmd);

			//	pack
			sprintf(szCmd, "sibpacker_tt.exe \"%s\" \"%s\"",
				xOutputDir, xOutputFile);
			ExeWaitForEnd(szCmd);

			//	delete
			DeleteFolder(xOutputDir);
		}
	}

	//	smtiles
	bHaveItems = false;
	xSets.clear();

	for(int i = 0; i < info.nRow; ++i)
	{
		for(int j = 0; j < info.nCol; ++j)
		{
			pTile = xMap.GetCellData(i, j);

			if((pTile->wMidImg & (~0x8000)) > 0)
			{
				int nValue = (pTile->wMidImg & (~0x8000)) - 1;
				xSets.insert(nValue);
			}
		}
	}

	if(!xSets.empty())
	{
		FILE* pfile = fopen(szWorkingIni, "w");
		if(pfile == NULL)
		{
			return false;
		}

		fprintf(pfile, "list m:\n");
		bHaveItems = true;

		for(std::set<int>::const_iterator begiter = xSets.begin();
			begiter != xSets.end();
			++begiter)
		{
			sprintf(szBuf, "%d\n", *begiter);
			fprintf(pfile, szBuf);
		}

		fclose(pfile);
	}

	if(bHaveItems)
	{
		CString xSrcFile = _pszSrcPath;
		xSrcFile += "/smtiles.wil";

		CString xDestFile = _pszDestPath;
		xDestFile += "/smtiles.sib";

		CString xOutputDir = szWorkingPath;
		xOutputDir += "/smtiles";
		if(!PathFileExists(xOutputDir))
		{
			mkdir(xOutputDir);
		}

		CString xOutputFile = szWorkingPath;
		xOutputFile += "/smtiles.sib";

		if(PathFileExists(xSrcFile) &&
			PathFileExists(xDestFile))
		{
			//	extract wil
			char szCmd[512] = {0};

			//	extract sib
			sprintf(szCmd, "sibextractor_tt.exe \"%s\" \"%s\"",
				xDestFile, xOutputDir);
			ExeWaitForEnd(szCmd);

			sprintf(szCmd, "wilextractorex.exe \"%s\" \"%s\" \"%s\"",
				szWorkingIni, _pszSrcPath, xOutputDir);
			ExeWaitForEnd(szCmd);

			//	pack
			sprintf(szCmd, "sibpacker_tt.exe \"%s\" \"%s\"",
				xOutputDir, xOutputFile);
			ExeWaitForEnd(szCmd);

			//	delete
			DeleteFolder(xOutputDir);
		}
	}

	//	objects
	for(int i = 0; i < 40; ++i)
	{
		bHaveItems = false;
		xSets.clear();
		int nNumber = i;

		for(int i = 0; i < info.nRow; ++i)
		{
			for(int j = 0; j < info.nCol; ++j)
			{
				pTile = xMap.GetCellData(i, j);

				if((pTile->wFrImg & (~0x8000)) > 0 &&
					pTile->bArea == nNumber)
				{
					int nValue = (pTile->wFrImg & (~0x8000));

					char bAni = pTile->bAniFrame;
					if((bAni & 0x80) > 0)
					{
						bAni &= (~0x80);
					}

					if(bAni > 0)
					{
						int nFrBase = nValue;
						int nAniFrames = bAni;
						for(int k = 0; k < nAniFrames; ++k)
						{
							nValue = nFrBase + k;
							--nValue;
							xSets.insert(nValue);
						}
					}
					else
					{
						--nValue;
						xSets.insert(nValue);
					}
				}
			}
		}

		if(!xSets.empty())
		{
			FILE* pfile = fopen(szWorkingIni, "w");
			if(pfile == NULL)
			{
				return false;
			}

			fprintf(pfile, "list o %d:\n", nNumber);
			bHaveItems = true;

			for(std::set<int>::const_iterator begiter = xSets.begin();
				begiter != xSets.end();
				++begiter)
			{
				sprintf(szBuf, "%d\n", *begiter);
				fprintf(pfile, szBuf);
			}

			fclose(pfile);
		}

		if(bHaveItems)
		{
			CString xSrcFile = _pszSrcPath;

			if(0 == nNumber)
			{
				xSrcFile += "/objects.wil";
			}
			else
			{
				xSrcFile.AppendFormat("/objects%d.wil", nNumber + 1);
			}

			CString xDestFile = _pszDestPath;
			if(0 == nNumber)
			{
				xDestFile += "/objects.sib";
			}
			else
			{
				xDestFile.AppendFormat("/objects%d.sib", nNumber + 1);
			}

			CString xOutputDir = szWorkingPath;
			if(0 == nNumber)
			{
				xOutputDir += "/objects";
			}
			else
			{
				xOutputDir.AppendFormat("/objects%d", nNumber + 1);
			}
			if(!PathFileExists(xOutputDir))
			{
				mkdir(xOutputDir);
			}

			CString xOutputFile = szWorkingPath;
			if(0 == nNumber)
			{
				xOutputFile += "/objects.sib";
			}
			else
			{
				xOutputFile.AppendFormat("/objects%d.sib", nNumber + 1);
			}

			if(PathFileExists(xSrcFile))
			{
				char szCmd[512] = {0};

				//	extract sib
				if(PathFileExists(xDestFile))
				{
					sprintf(szCmd, "sibextractor_tt.exe \"%s\" \"%s\"",
						xDestFile, xOutputDir);
					ExeWaitForEnd(szCmd);
				}

				//	extract wil
				
				sprintf(szCmd, "WilExtractorEx.exe \"%s\" \"%s\" \"%s\"",
					szWorkingIni, _pszSrcPath, xOutputDir);
				ExeWaitForEnd(szCmd);

				//	pack
				sprintf(szCmd, "sibpacker_tt.exe \"%s\" \"%s\"",
					xOutputDir, xOutputFile);
				ExeWaitForEnd(szCmd);

				//	delete
				DeleteFolder(xOutputDir);
			}
		}
	}

	AfxMessageBox("完成!");

	return true;
}

void ExeWaitForEnd(const char* _pszRunCmd)
{
	STARTUPINFO si = {sizeof(si)};
	PROCESS_INFORMATION pi = {0};

	char szRunParam[512];
	strcpy(szRunParam, _pszRunCmd);

	CreateProcess(
		NULL,
		szRunParam,
		NULL,           // process security attributes
		NULL,           // primary thread security attributes
		0,              // handles are inherited
		0,              // creation flags
		NULL,           // use parent's environment
		NULL,           // use parent's current directory
		&si,   // STARTUPINFO pointer
		&pi);   // receives PROCESS_INFORMATION

	// Wait for the processs to finish
	DWORD rc = WaitForSingleObject(
		pi.hProcess, // process handle
		INFINITE);
	
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

/*判断一个路径是否是已存在的目录*/
BOOL IsDirectory(LPCTSTR pstrPath)
{
	if (NULL == pstrPath)
	{
		return FALSE;
	}

	/*去除路径末尾的反斜杠*/
	CString strPath = pstrPath;
	if (strPath.Right(1) == _T('\\'))
	{
		strPath.Delete(strPath.GetLength()-1);
	}

	CFileFind finder;
	BOOL bRet = finder.FindFile(strPath);
	if (!bRet)
	{
		return FALSE;
	}

	/*判断该路径是否是目录*/
	finder.FindNextFile();
	bRet = finder.IsDirectory();
	finder.Close();
	return bRet;
}

/*判断一个路径是否是已存在的目录*/
/*删除目录及目录中的所有内容*/
BOOL DeleteFolder(LPCTSTR pstrFolder)
{
	if ((NULL == pstrFolder))
	{
		return FALSE;
	}

	/*检查输入目录是否是合法目录*/
	if (!IsDirectory(pstrFolder))
	{
		return FALSE;
	}

	/*创建源目录中查找文件的通配符*/
	CString strWildcard(pstrFolder);
	if (strWildcard.Right(1) != _T('\\'))
	{
		strWildcard += _T("\\");
	}
	strWildcard += _T("*.*");

	/*打开文件查找，查看源目录中是否存在匹配的文件*/
	/*调用FindFile后，必须调用FindNextFile才能获得查找文件的信息*/
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		/*查找下一个文件*/
		bWorking = finder.FindNextFile();

		/*跳过当前目录“.”和上一级目录“..”*/
		if (finder.IsDots())
		{
			continue;
		}

		/*得到当前目录的子文件的路径*/
		CString strSubFile = finder.GetFilePath();

		/*判断当前文件是否是目录,*/
		/*如果是目录，递归调用删除目录,*/
		/*否则，直接删除文件*/
		if (finder.IsDirectory())
		{
			if (!DeleteFolder(strSubFile))
			{
				finder.Close();
				return FALSE;
			}
		}
		else
		{
			if (!DeleteFile(strSubFile))
			{
				finder.Close();
				return FALSE;
			}
		}

	} /*while (bWorking)*/

	/*关闭文件查找*/
	finder.Close();

	/*删除空目录*/
	return RemoveDirectory(pstrFolder);
}