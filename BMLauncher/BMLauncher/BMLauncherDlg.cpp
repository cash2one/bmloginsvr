// BMLauncherDlg.cpp : 实现文件
//

#include "stdafx.h"

#pragma comment(lib,"Ws2_32.lib")
#include <winsock2.h>

#include "BMLauncher.h"
#include "BMLauncherDlg.h"
#include "LauncherUtils.h"
#include "SimpleIni.h"
#include "BMHttpManager.h"
#include "EasyUtils.h"
#include "cJSON.h"

#include <fastdelegate.h>
#include <FastDelegateBind.h>

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PAGE_SINGLE				0
#define PAGE_ONLINESERVER		1
#define PAGE_ONLINECLIENT		2
#define PAGE_DONATE				3
#define PAGE_REGISTER			4

//////////////////////////////////////////////////////////////////////////
using namespace DuiLib;

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


// CBMLauncherDlg 对话框




CBMLauncherDlg::CBMLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDuiDialog(CBMLauncherDlg::IDD, pParent, "skin.xml")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBMLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDuiDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBMLauncherDlg, CDuiDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CBMLauncherDlg 消息处理程序

BOOL CBMLauncherDlg::OnInitDialog()
{
	CDuiDialog::OnInitDialog();

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
	LoadConfig();

	//	下载登陆文件
	DownloadConfigFile();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBMLauncherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDuiDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBMLauncherDlg::OnPaint()
{
	CDuiDialog::OnPaint();
	/*if (IsIconic())
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
	}*/
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBMLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBMLauncherDlg::DownloadConfigFile()
{
	char szDestPath[MAX_PATH] = {0};
	GetRootPath(szDestPath);
	strcat(szDestPath, "/config/url.ini");

	if(!PathFileExists(szDestPath))
	{
#ifdef _DEBUG
		if(!EasyDownloadFile("http://sryanyuan.github.io/web/config/url_d.ini", szDestPath))
#else
		if(!EasyDownloadFile("http://sryanyuan.github.io/web/config/url.ini", szDestPath))
#endif
		{
			MessageBox("无法下载配置文件", "错误", MB_ICONERROR | MB_OK);
			return;
		}
	}

	char* pContent = EasyReadFile(szDestPath);
	if(NULL == pContent)
	{
		MessageBox("无法读取配置文件", "错误", MB_ICONERROR | MB_OK);
		return;
	}

	CSimpleIniA xIniFile;
	if(SI_OK != xIniFile.LoadFile(szDestPath))
	{
		MessageBox("无法解析配置文件", "错误", MB_ICONERROR | MB_OK);
		delete[] pContent;
		return;
	}

	const char* pszRegUrl = xIniFile.GetValue("CONFIG", "REGURL");
	if(NULL != pszRegUrl)
	{
		m_xRegHttpAddr = pszRegUrl;
	}

	delete[] pContent;
	pContent = NULL;
}

void CBMLauncherDlg::LoadConfig()
{
	CSimpleIniA xIniFile;

	char szPath[MAX_PATH];
	GetRootPath(szPath);
	strcat(szPath, "/launcher.ini");
	if(!PathFileExists(szPath))
	{
		return;
	}

	SI_Error ret = xIniFile.LoadFile(szPath);
	if(SI_OK != ret)
	{
		return;
	}

	const char* pszValue = xIniFile.GetValue("config", "port_single", "");
	if(NULL != pszValue &&
		0 != strlen(pszValue))
	{
		m_pm.FindControl("edit_port_single")->SetText(pszValue);
	}

	pszValue = xIniFile.GetValue("config", "ip_onlineserver", "");
	if(NULL != pszValue &&
		0 != strlen(pszValue))
	{
		m_pm.FindControl("edit_ip_onlineserver")->SetText(pszValue);
	}

	pszValue = xIniFile.GetValue("config", "port_onlineserver", "");
	if(NULL != pszValue &&
		0 != strlen(pszValue))
	{
		m_pm.FindControl("edit_port_onlineserver")->SetText(pszValue);
	}

	pszValue = xIniFile.GetValue("config", "ip_onlineclient", "");
	if(NULL != pszValue &&
		0 != strlen(pszValue))
	{
		m_pm.FindControl("edit_ip_onlineclient")->SetText(pszValue);
	}

	pszValue = xIniFile.GetValue("config", "port_onlineclient", "");
	if(NULL != pszValue &&
		0 != strlen(pszValue))
	{
		m_pm.FindControl("edit_port_onlineclient")->SetText(pszValue);
	}

	pszValue = xIniFile.GetValue("config", "account_onlineclient", "");
	if(NULL != pszValue &&
		0 != strlen(pszValue))
	{
		m_pm.FindControl("edit_account_onlineclient")->SetText(pszValue);
	}

	pszValue = xIniFile.GetValue("config", "password_onlineclient", "");
	if(NULL != pszValue &&
		0 != strlen(pszValue))
	{
		m_pm.FindControl("edit_password_onlineclient")->SetText(pszValue);
	}
}

void CBMLauncherDlg::SaveConfig()
{
	CSimpleIniA xIniFile;

	char szPath[MAX_PATH];
	GetRootPath(szPath);
	strcat(szPath, "/launcher.ini");

	xIniFile.SetValue("config", "port_single", m_pm.FindControl("edit_port_single")->GetText());

	xIniFile.SetValue("config", "ip_onlineserver", m_pm.FindControl("edit_ip_onlineserver")->GetText());

	xIniFile.SetValue("config", "port_onlineserver", m_pm.FindControl("edit_port_onlineserver")->GetText());

	xIniFile.SetValue("config", "ip_onlineclient", m_pm.FindControl("edit_ip_onlineclient")->GetText());

	xIniFile.SetValue("config", "port_onlineclient", m_pm.FindControl("edit_port_onlineclient")->GetText());

	xIniFile.SetValue("config", "account_onlineclient", m_pm.FindControl("edit_account_onlineclient")->GetText());

	xIniFile.SetValue("config", "password_onlineclient", m_pm.FindControl("edit_password_onlineclient")->GetText());

	xIniFile.SaveFile(szPath);
}


//////////////////////////////////////////////////////////////////////////
void CBMLauncherDlg::Notify(TNotifyUI& msg)
{
	const char* pszSenderName = msg.pSender->GetName();
	CTabLayoutUI* pLayout = (CTabLayoutUI*)m_pm.FindControl("modelayout");
	int nCurSel = pLayout->GetCurSel();

	if(msg.sType == DUI_MSGTYPE_CLICK)
	{
		if(0 == _tcscmp(pszSenderName, "button_quit")) {
			SaveConfig();
			OnCancel();
		}
		else if(0 == _tcscmp(pszSenderName, "option_single"))
		{
			SwitchTabLayout(PAGE_SINGLE);
		}
		else if(0 == _tcscmp(pszSenderName, "option_onlineserver"))
		{
			SwitchTabLayout(PAGE_ONLINESERVER);
		}
		else if(0 == _tcscmp(pszSenderName, "option_onlineclient"))
		{
			SwitchTabLayout(PAGE_ONLINECLIENT);
		}
		else if(0 == _tcscmp(pszSenderName, "option_donate"))
		{
			SwitchTabLayout(PAGE_DONATE);
		}
		else if(0 == _tcscmp(pszSenderName, "option_register"))
		{
			SwitchTabLayout(PAGE_REGISTER);
		}
		else if(0 == _tcscmp(pszSenderName, "button_login_single") ||
			0 == _tcscmp(pszSenderName, "button_login_onlineserver") ||
			0 == _tcscmp(pszSenderName, "button_login_onlineclient"))
		{
			DoLogin(nCurSel);
		}
		else if(0 == _tcscmp(pszSenderName, "button_diagnosis"))
		{
			DoDiagnosis();
		}
		else if(0 == _tcscmp(pszSenderName, "button_patcher"))
		{
			char szPath[MAX_PATH];
			GetRootPath(szPath);
			strcat(szPath, "\\patcher\\*.zip");
			Launcher_CheckPatcher(szPath);
		}
		else if(0 == _tcscmp(pszSenderName, "button_mailverify"))
		{
			DoMailVerify();
		}
		else if(0 == _tcscmp(pszSenderName, "button_reg"))
		{
			DoRegAccount();
		}
	}

	__super::Notify(msg);
}


void CBMLauncherDlg::SwitchTabLayout(int _nTabPage)
{
	CTabLayoutUI* pLayout = (CTabLayoutUI*)m_pm.FindControl("modelayout");
	if(NULL == pLayout)
	{
		return;
	}

	pLayout->SelectItem(_nTabPage);

	if(_nTabPage == PAGE_DONATE)
	{
		static bool s_bLoaded = false;

		if(!s_bLoaded)
		{
			CWebBrowserUI* pWeb = (CWebBrowserUI*)m_pm.FindControl("browser");
			pWeb->Navigate2("http://sryanyuan.github.io/web");
			s_bLoaded = true;
		}
	}
}

void CBMLauncherDlg::DoMailVerify()
{
	const char* pszMailAddr = m_pm.FindControl("edit_reg_mailverify")->GetText();
	if(NULL == pszMailAddr ||
		strlen(pszMailAddr) == 0 ||
		strlen(pszMailAddr) > 49)
	{
		MessageBox("非法的邮件地址", "错误", MB_ICONERROR | MB_OK);
		return;
	}

	string xMailAddr = pszMailAddr;
	if(string::npos == xMailAddr.find('@'))
	{
		MessageBox("非法的邮件地址", "错误", MB_ICONERROR | MB_OK);
		return;
	}

	if(m_xRegHttpAddr.empty())
	{
		MessageBox("无法获得请求地址", "错误", MB_ICONERROR | MB_OK);
		return;
	}

	char szUrl[MAX_PATH];
	sprintf(szUrl, "%s/mailverify?mail=%s", m_xRegHttpAddr.c_str(), pszMailAddr);

	if(!BMHttpManager::GetInstance()->DoGetRequestSync(szUrl, fastdelegate::bind(&CBMLauncherDlg::OnMailVerifyResult, this)))
	{
		if(m_xRegHttpAddr.empty())
		{
			MessageBox("请求超时或失败", "错误", MB_ICONERROR | MB_OK);
		}
	}
}

void CBMLauncherDlg::OnMailVerifyResult(const char* _pData, size_t _uLen)
{
	cJSON* pRoot = cJSON_Parse(_pData);
	if(NULL == pRoot)
	{
		MessageBox("无法解析json数据", "错误", MB_ICONERROR | MB_OK);
		return;
	}

	cJSON* pResult = cJSON_GetObjectItem(pRoot, "errcode");
	int nRet = pResult->valueint;

	cJSON* pErrMsg = cJSON_GetObjectItem(pRoot, "errmsg");
	const char* pszMsg = pErrMsg->valuestring;

	if(0 != nRet)
	{
		char szErrMsg[MAX_PATH];
		sprintf(szErrMsg, "请求失败:%s", pszMsg);
		ErrorMsgBox(szErrMsg);
		return;
	}
	else
	{
		MessageBox("请求成功，请注意登陆邮箱查收邮件，若没有查收到，可能再垃圾箱内，或选择再次验证", "提示", MB_OK | MB_ICONINFORMATION);

		m_pm.FindControl("edit_reg_regkey")->SetText(pszMsg);
	}
}

void CBMLauncherDlg::DoRegAccount()
{
	if(m_xRegHttpAddr.empty())
	{
		MessageBox("无法获得请求地址", "错误", MB_ICONERROR | MB_OK);
		return;
	}

	const char* pszRegKey = m_pm.FindControl("edit_reg_regkey")->GetText();
	const char* pszAccount = m_pm.FindControl("edit_reg_account")->GetText();
	const char* pszPassword = m_pm.FindControl("edit_reg_password")->GetText();
	const char* pszMailAddr = m_pm.FindControl("edit_reg_mailverify")->GetText();

	if(NULL == pszMailAddr ||
		strlen(pszMailAddr) == 0 ||
		strlen(pszMailAddr) > 49)
	{
		MessageBox("非法的邮件地址", "错误", MB_ICONERROR | MB_OK);
		return;
	}
	string xMailAddr = pszMailAddr;
	if(string::npos == xMailAddr.find('@'))
	{
		MessageBox("非法的邮件地址", "错误", MB_ICONERROR | MB_OK);
		return;
	}


	if(NULL == pszRegKey ||
		NULL == pszAccount ||
		NULL == pszPassword)
	{
		ErrorMsgBox("请填入密钥、账户、密码");
		return;
	}

	if(strlen(pszRegKey) == 0 ||
		strlen(pszRegKey) > 49)
	{
		ErrorMsgBox("密钥输入错误");
		return;
	}

	if(strlen(pszAccount) < 5 ||
		strlen(pszAccount) > 19)
	{
		ErrorMsgBox("账户输入错误(5-19字符)");
		return;
	}

	if(strlen(pszPassword) < 5 ||
		strlen(pszPassword) > 19)
	{
		ErrorMsgBox("密码输入错误(5-19字符)");
		return;
	}

	char szUrl[MAX_PATH];
	sprintf(szUrl, "%s/regaccount?mail=%s&key=%s&account=%s&password=%s", m_xRegHttpAddr.c_str(), pszMailAddr, pszRegKey, pszAccount, pszPassword);

	if(!BMHttpManager::GetInstance()->DoGetRequestSync(szUrl, fastdelegate::bind(&CBMLauncherDlg::OnRegAccountResult, this)))
	{
		ErrorMsgBox("请求失败或超时");
	}
}

void CBMLauncherDlg::OnRegAccountResult(const char* _pData, size_t _uLen)
{
	cJSON* pRoot = cJSON_Parse(_pData);
	if(NULL == pRoot)
	{
		MessageBox("无法解析json数据", "错误", MB_ICONERROR | MB_OK);
		return;
	}

	cJSON* pResult = cJSON_GetObjectItem(pRoot, "errcode");
	int nRet = pResult->valueint;

	cJSON* pErrMsg = cJSON_GetObjectItem(pRoot, "errmsg");
	const char* pszMsg = pErrMsg->valuestring;

	if(0 != nRet)
	{
		char szErrMsg[MAX_PATH];
		sprintf(szErrMsg, "请求失败:%s", pszMsg);
		ErrorMsgBox(szErrMsg);
		return;
	}
	else
	{
		MessageBox("请求已提交，请注意登陆邮箱查收邮件，若没有查收到，可能在垃圾箱内", "提示", MB_OK | MB_ICONINFORMATION);
	}
}

void CBMLauncherDlg::DoLogin(int _nPage)
{
	if(_nPage == PAGE_SINGLE)
	{
		//	test port
		const char* pszPort = m_pm.FindControl("edit_port_single")->GetText();
		int nPort = atoi(pszPort);

		Launcher_Single(nPort);
	}
	else if(_nPage == PAGE_ONLINESERVER)
	{
		const char* pszIp = m_pm.FindControl("edit_ip_onlineserver")->GetText();
		const char* pszPort = m_pm.FindControl("edit_port_onlineserver")->GetText();
		int nPort = atoi(pszPort);

		Launcher_Server(pszIp, nPort);
	}
	else if(_nPage == PAGE_ONLINECLIENT)
	{
		const char* pszIp = m_pm.FindControl("edit_ip_onlineclient")->GetText();
		const char* pszPort = m_pm.FindControl("edit_port_onlineclient")->GetText();
		int nPort = atoi(pszPort);
		const char* pszAccount = m_pm.FindControl("edit_account_onlineclient")->GetText();
		const char* pszPassword = m_pm.FindControl("edit_password_onlineclient")->GetText();

		Launcher_Client(pszIp, nPort, pszAccount, pszPassword);
	}
}

void CBMLauncherDlg::DoDiagnosis()
{
	//	ch path
	char szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, sizeof(szPath));
	PathRemoveFileSpec(szPath);

	for(int i = 0; i < strlen(szPath); ++i)
	{
		if(0 == szPath[i])
		{
			break;
		}

		if(szPath[i] < 0)
		{
			MessageBox("检测到中文路径，请不要将游戏放在带有中文的路径中。   Exp:c:\\新建文件夹\\backmir.exe 就存在着中文。也不要直接放在桌面上。",
				"错误",
				MB_OK | MB_ICONERROR);
			return;
		}
	}

	int nColorDepth = CheckDisplayColorDepth();
	if(32 != nColorDepth)
	{
		MessageBox("检测到当前显示颜色数不为32位。请在桌面上点击右键，选择屏幕分辨率，在设置页面上点击高级设置，列出所有模式，然后选中当前分辨率下的真彩色（32位），点击确定即可。",
			"错误",
			MB_OK | MB_ICONERROR);
		return;
	}

	MessageBox("您的运行环境一切正常，若无法运行，请检测游戏端口是否被占用。出现E6错误，请关闭I7处理器的超线程技术。",
		"提示",
		MB_OK | MB_ICONINFORMATION);
}