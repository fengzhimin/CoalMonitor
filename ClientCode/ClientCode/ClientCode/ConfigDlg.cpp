// ConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientCode.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"
#include "Log.h"

extern Log m_log;

// CConfigDlg 对话框

IMPLEMENT_DYNAMIC(CConfigDlg, CDialogEx)

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigDlg::IDD, pParent)
	, m_ip(_T(""))
	, m_database(_T(""))
	, m_loginName(_T(""))
	, m_passwd(_T(""))
{
	m_font_static.CreatePointFont(160, L"宋体");
	m_font_edit.CreatePointFont(150, L"宋体");
	m_font_button.CreatePointFont(190, L"宋体");
}

CConfigDlg::~CConfigDlg()
{
	m_font_static.DeleteObject();
	m_font_edit.DeleteObject();
	m_font_button.DeleteObject();
}

BOOL CConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_SET_CONFIG_STATIC)->SetFont(&m_font_button);
	GetDlgItem(IDC_IP_CONFIG_STATIC)->SetFont(&m_font_static);
	GetDlgItem(IDC_DATABASE_CONFIG_STATIC)->SetFont(&m_font_static);
	GetDlgItem(IDC_LOGINNAME_CONFIG_STATIC)->SetFont(&m_font_static);
	GetDlgItem(IDC_PASSWD_CONFIG_STATIC)->SetFont(&m_font_static);
	GetDlgItem(IDC_IP_CONFIG_EDIT)->SetFont(&m_font_edit);
	GetDlgItem(IDC_DATABASE_CONFIG_EDIT)->SetFont(&m_font_edit);
	GetDlgItem(IDC_LOGINNAME_CONFIG_EDIT)->SetFont(&m_font_edit);
	GetDlgItem(IDC_PASSWD_CONFIG_EDIT)->SetFont(&m_font_edit);
	GetDlgItem(IDC_SAVE_CONFIG_BUTTON)->SetFont(&m_font_button);
	GetDlgItem(IDC_CANCEL_CONFIG_BUTTON)->SetFont(&m_font_button);

	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
	BOOL ifFind = finder.FindFile(_T("./Config.ini"));  
	if( ifFind )  
	{  
		::GetPrivateProfileStringW(_T("Database connection Info"),_T("IP"),_T(""),m_ip.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		::GetPrivateProfileStringW(_T("Database connection Info"),_T("Database"),_T(""),m_database.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		::GetPrivateProfileStringW(_T("Database connection Info"),_T("UID"),_T(""),m_loginName.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		::GetPrivateProfileStringW(_T("Database connection Info"),_T("PWD"),_T(""),m_passwd.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));    
		//在这里必须ReleaseBuffer()，否则无法再后面进行字符串的连接  
		m_ip.ReleaseBuffer();  
		m_database.ReleaseBuffer();  
		m_loginName.ReleaseBuffer();  
		m_passwd.ReleaseBuffer(); 

		UpdateData(FALSE);
	} 
	else
		m_log.WriteErrorLog(L"读取配置文件错误!");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IP_CONFIG_EDIT, m_ip);
	DDX_Text(pDX, IDC_DATABASE_CONFIG_EDIT, m_database);
	DDX_Text(pDX, IDC_LOGINNAME_CONFIG_EDIT, m_loginName);
	DDX_Text(pDX, IDC_PASSWD_CONFIG_EDIT, m_passwd);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SAVE_CONFIG_BUTTON, &CConfigDlg::OnBnClickedSaveButton)
	ON_BN_CLICKED(IDC_CANCEL_CONFIG_BUTTON, &CConfigDlg::OnBnClickedCancelButton)
END_MESSAGE_MAP()


// CConfigDlg 消息处理程序


void CConfigDlg::OnBnClickedSaveButton()
{
	UpdateData(TRUE);

	::WritePrivateProfileStringW(_T("Database connection Info"),_T("IP"),m_ip,_T("./Config.ini"));  
	::WritePrivateProfileStringW(_T("Database connection Info"),_T("Database"),m_database,_T("./Config.ini"));  
	::WritePrivateProfileStringW(_T("Database connection Info"),_T("UID"),m_loginName,_T("./Config.ini"));  
	::WritePrivateProfileStringW(_T("Database connection Info"),_T("PWD"),m_passwd,_T("./Config.ini")); 
	
	CDialog::OnOK();
}


void CConfigDlg::OnBnClickedCancelButton()
{
	CDialog::OnCancel();
}
