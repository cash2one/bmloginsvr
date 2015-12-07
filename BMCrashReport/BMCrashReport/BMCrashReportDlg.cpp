// BMCrashReportDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BMCrashReport.h"
#include "BMCrashReportDlg.h"
#include "../../../../CommonModule/CommandLineHelper.h"
#include "../../../../CommonModule/ZipDataReader.h"
#include <direct.h>
#include <Shlwapi.h>

#ifdef _DEBUG
#pragma comment(lib, "ziparchive_d.lib")
#else
#pragma comment(lib, "ziparchive.lib")
#endif
#pragma comment(lib, "shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
const char* GetRootPath()
{
	static char s_szRootPath[MAX_PATH] = {0};
	if(s_szRootPath[0] == 0)
	{
		GetModuleFileName(NULL, s_szRootPath, sizeof(s_szRootPath));
		PathRemoveFileSpec(s_szRootPath);
	}
	return s_szRootPath;
}
//////////////////////////////////////////////////////////////////////////


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


// CBMCrashReportDlg 对话框




CBMCrashReportDlg::CBMCrashReportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMCrashReportDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBMCrashReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBMCrashReportDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_USER_HYPERLINK_CLICKED, &CBMCrashReportDlg::OnHyperLinkClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CBMCrashReportDlg 消息处理程序

BOOL CBMCrashReportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

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
	CoInitialize(NULL);

	CommandLineHelper xCmdHelper;
	bool bParseSuccess = false;

	//	params
	const char* pszDumpFile = NULL;
	const char* pszTitle = NULL;
	const char* pszErrorCode = NULL;
	DWORD dwErrorCode = 0;
	const char* pszErrorAddr = NULL;
	DWORD dwErrorAddr = 0;

	do 
	{
		if(!xCmdHelper.InitParam())
		{
			break;
		}
		pszDumpFile = xCmdHelper.GetParam("dumpfile");
		if(NULL == pszDumpFile)
		{
			break;
		}
		pszTitle = xCmdHelper.GetParam("title");
		if(NULL == pszTitle)
		{
			break;
		}
		pszErrorCode = xCmdHelper.GetParam("errorcode");
		if(NULL == pszErrorCode)
		{
			break;
		}
		dwErrorCode = atoi(pszErrorCode);
		if(0 == dwErrorCode)
		{
			break;
		}
		pszErrorAddr = xCmdHelper.GetParam("erroraddr");
		if(NULL == pszErrorAddr)
		{
			break;
		}
		dwErrorAddr = atoi(pszErrorAddr);
		if(0 == dwErrorAddr)
		{
			break;
		}

		bParseSuccess = true;
	} while (false);

	if(!bParseSuccess)
	{
		AfxMessageBox("无效的参数", MB_ICONERROR | MB_OK);
		EndDialog(0);
		return FALSE;
	}

	//	复制变量
	m_xDumpfilePath = pszDumpFile;

	//	刷新描述
	char szDumfile[MAX_PATH];
	strcpy(szDumfile, pszDumpFile);
	const char* pszReturnedPath = PathFindFileName(szDumfile);

	char szDesc[512] = {0};
	sprintf(szDesc, "    %s 遇到问题需要关闭。给您造成的不便深表歉意。为了改善游戏的体验和稳定性，请点击\"打开报告位置\"按钮，将选中的[%s]文件发送至开发者邮箱553056120@qq.com，并点击提交报告按钮来提交错误报告。", pszTitle, pszReturnedPath);
	GetDlgItem(IDC_STATIC_TITLE)->SetWindowText(szDesc);

	sprintf(szDesc, "%s 错误报告助手", pszTitle);
	SetWindowText(szDesc);

	CButton* pSendReportCheckBox = (CButton*)GetDlgItem(IDC_CHECK1);
	if(pSendReportCheckBox)
	{
		pSendReportCheckBox->SetCheck(1);
	}

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	if(pEdit)
	{
		pEdit->SetLimitText(256);
	}

	//	设置超链接
	m_xHyperLinkOpenReportDirectory.SubclassDlgItem(IDC_STATIC_OPENREPORT, this);
	m_xHyperLinkViewReport.SubclassDlgItem(IDC_STATIC_VIEWREPORT, this);

	//	错误说明什么的
	sprintf(szDesc, "错误地址：0x%08X 错误码：0x%08X", dwErrorAddr, dwErrorCode);
	GetDlgItem(IDC_STATIC_ERRORINFO)->SetWindowText(szDesc);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBMCrashReportDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBMCrashReportDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBMCrashReportDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CBMCrashReportDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)

	{

		switch(pMsg->wParam)

		{

		case VK_RETURN:

			return TRUE;

		case VK_ESCAPE:

			return TRUE;

		default:

			break;

		}

	}

	return __super::PreTranslateMessage(pMsg);
}

void CBMCrashReportDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	char szExecuteCmd[MAX_PATH * 2];
	sprintf(szExecuteCmd, "explorer /select,%s", m_xDumpfilePath.c_str());
	WinExec(szExecuteCmd, SW_NORMAL);
}

LRESULT CBMCrashReportDlg::OnHyperLinkClicked(WPARAM wParam, LPARAM lParam)
{
	HWND hSender = (HWND)wParam;

	if(hSender == m_xHyperLinkOpenReportDirectory.GetSafeHwnd())
	{
		OnBnClickedButton1();
	}
	else if(hSender == m_xHyperLinkViewReport.GetSafeHwnd())
	{
		ViewReport();
	}

	return S_OK;
}

void CBMCrashReportDlg::ViewReport()
{
	if(m_xDumpfilePath.empty())
	{
		return;
	}

	if(!PathFileExists(m_xDumpfilePath.c_str()))
	{
		AfxMessageBox("错误报告不存在");
		return;
	}

	const char* pszPath = PathFindFileName(m_xDumpfilePath.c_str());
	//	remove extension
	char szFilename[MAX_PATH] = {0};
	strcpy(szFilename, pszPath);
	PathRemoveExtension(szFilename);

	char* pZipData = NULL;
	size_t uZipDataLength = 0;

	char szOpenFile[MAX_PATH] = {0};
	sprintf(szOpenFile, "%s.log", szFilename);

	if(READZIP_OK == ReadZipData(m_xDumpfilePath.c_str(), szOpenFile, (BYTE**)&pZipData, &uZipDataLength, ""))
	{
		HANDLE hFile = INVALID_HANDLE_VALUE;

		do 
		{
			char * pszTmpDir = getenv("TEMP");

			if(NULL == pszTmpDir ||
				strlen(pszTmpDir) == 0){
					break;
			}

			sprintf(szOpenFile, "%s/bmcrashreport", pszTmpDir);
			if(!PathFileExists(szOpenFile))
			{
				if(0 != mkdir(szOpenFile))
				{
					break;
				}
			}

			sprintf(szOpenFile, "%s/bmcrashreport/%s.log", pszTmpDir, szFilename);
			hFile = CreateFile(szOpenFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if(INVALID_HANDLE_VALUE == hFile)
			{
				int nLastError = GetLastError();
				sprintf(szOpenFile, "打开报告失败，错误码：%d", nLastError);
				AfxMessageBox(szOpenFile);
				break;
			}

			DWORD dwWrite = 0;
			if(!WriteFile(hFile, pZipData, uZipDataLength, &dwWrite, NULL))
			{
				break;
			}
		} while (false);

		delete []pZipData;
		pZipData = NULL;

		if(INVALID_HANDLE_VALUE != hFile)
		{
			CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;

			//	open it
			char szCmd[MAX_PATH];
			sprintf(szCmd, "notepad.exe %s", szOpenFile);
			WinExec(szCmd, SW_SHOW);
		}
	}
	else
	{
		AfxMessageBox("无法打开报告");
	}
}