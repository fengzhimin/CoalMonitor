// Login.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientCode.h"
#include "Login.h"
#include "afxdialogex.h"
#include "Log.h"

extern Log m_log;

// CLogin 对话框

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogin::IDD, pParent)
	, m_loginName(_T(""))
	, m_passwd(_T(""))
	, m_remember_check(FALSE)
{
	m_font_static.CreatePointFont(160, L"宋体");
	m_font_edit.CreatePointFont(150, L"宋体");
	m_font_button.CreatePointFont(190, L"宋体");
}

CLogin::~CLogin()
{
	m_font_static.DeleteObject();
	m_font_edit.DeleteObject();
	m_font_button.DeleteObject();
}

BOOL CLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_LOGINNAME_LOGIN_STATIC)->SetFont(&m_font_static);
	GetDlgItem(IDC_PASSWD_LOGIN_STATIC)->SetFont(&m_font_static);
	GetDlgItem(IDC_LOGINNAME_LOGIN_EDIT)->SetFont(&m_font_edit);
	GetDlgItem(IDC_PASSWD_LOGIN_EDIT)->SetFont(&m_font_edit);
	GetDlgItem(IDC_LOGIN_LOGIN_BUTTON)->SetFont(&m_font_button);
	GetDlgItem(IDC_CANCEL_LOGIN_BUTTON)->SetFont(&m_font_button);

	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
	BOOL ifFind = finder.FindFile(_T("./Config.ini"));  
	if( ifFind )  
	{  
		CString _rememberPasswd = L"";
		::GetPrivateProfileStringW(_T("Remember password Info"),_T("RememberPasswd"),_T("FALSE"),_rememberPasswd.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
		_rememberPasswd.ReleaseBuffer();  
		if(_rememberPasswd == "TRUE")
		{
			m_loginName = L"root";
			m_passwd = L"root";
			m_remember_check = true;
		}
		else
		{
			m_remember_check = false;
		}

		UpdateData(FALSE);
	} 
	else
		m_log.WriteErrorLog(L"读取配置文件错误!");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOGINNAME_LOGIN_EDIT, m_loginName);
	DDX_Text(pDX, IDC_PASSWD_LOGIN_EDIT, m_passwd);
	DDX_Check(pDX, IDC_REMEMBER_LOGIN_CHECK, m_remember_check);
	DDX_Control(pDX, IDC_LOGINNAME_LOGIN_EDIT, m_loginName_Control);
	DDX_Control(pDX, IDC_PASSWD_LOGIN_EDIT, m_passwd_Control);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
	ON_BN_CLICKED(IDC_LOGIN_LOGIN_BUTTON, &CLogin::OnBnClickedLoginLoginButton)
	ON_BN_CLICKED(IDC_CANCEL_LOGIN_BUTTON, &CLogin::OnBnClickedCancelLoginButton)
END_MESSAGE_MAP()


// CLogin 消息处理程序


void CLogin::OnBnClickedLoginLoginButton()
{
	UpdateData();
	if(m_loginName != L"")
	{
		if(m_passwd != L"")
		{
			if(m_loginName == L"root" && m_passwd == L"root")
			{
				CString _rememberPasswd = L"";
				if(m_remember_check)
					_rememberPasswd = L"TRUE";
				else
					_rememberPasswd = L"FALSE";

				::WritePrivateProfileStringW(_T("Remember password Info"),_T("RememberPasswd"),_rememberPasswd,_T("./Config.ini"));  
				MessageBox(L"登录成功！");
				CDialog::OnOK();
			}
			else
			{
				MessageBox(L"用户名或密码错误！");
				m_loginName = m_passwd = L"";
				UpdateData(FALSE);
				m_loginName_Control.SetFocus();
			}
		}
		else
		{
			MessageBox(L"请输入密码！");
			m_passwd_Control.SetFocus();
		}
	}
	else
	{
		MessageBox(L"请输入用户名！");
		m_loginName_Control.SetFocus();
	}
}


void CLogin::OnBnClickedCancelLoginButton()
{
	CDialog::OnCancel();
}

