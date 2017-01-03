
// ClientCodeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientCode.h"
#include "ClientCodeDlg.h"
#include "afxdialogex.h"
#include "ConfigDlg.h"
#include "Login.h"
#include "OperateDataBase.h"
#include "ReadCarCard.h"
#include "WriteCarCard.h"
#include "VideoConfig.h"
#include <Winsock2.h>  
#pragma comment(lib,"ws2_32.lib") 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientCodeDlg �Ի���
CClientCodeDlg::CClientCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientCodeDlg::IDD, pParent)
	, m_carCardInfo_string(_T(""))
{
	m_bIsLogin = false;
	m_bIsPlaying = false;
	m_bIsRecording = false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CARCARDINFO_CLIENTCODE_EDIT, m_carCardInfo_string);
}

BEGIN_MESSAGE_MAP(CClientCodeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_LOGIN, &CClientCodeDlg::OnLogin)
	ON_COMMAND(ID_ABOUT, &CClientCodeDlg::OnAbout)
	ON_COMMAND(ID_CONFIG, &CClientCodeDlg::OnConfig)
	ON_COMMAND(ID_CLOSE, &CClientCodeDlg::OnClose)
	ON_COMMAND(ID_LOGOUT, &CClientCodeDlg::OnLogout)
	ON_COMMAND(ID_READCARCARD, &CClientCodeDlg::OnReadcarcard)
	ON_COMMAND(ID_WRITECARCARD, &CClientCodeDlg::OnWritecarcard)
	ON_COMMAND(ID_SERIAL, &CClientCodeDlg::OnSerial)
	ON_COMMAND(ID_PARASET, &CClientCodeDlg::OnParaset)
	ON_COMMAND(IDC_TEXT_CLIENTCODE_BUTTON, &CClientCodeDlg::OnBnClickedTextClientcodeButton)
	ON_COMMAND(ID_LOGIN_OR_LOGOUT, &CClientCodeDlg::OnLoginOrLogout)
	ON_COMMAND(ID_START_OR_STOP, &CClientCodeDlg::OnStartOrStop)
	ON_COMMAND(ID_CAPTURE, &CClientCodeDlg::OnCapture)
	ON_COMMAND(ID_START_OR_STOP_RECORD, &CClientCodeDlg::OnStartOrStopRecord)
	ON_COMMAND(ID_VIDEO_CONFIG, &CClientCodeDlg::OnVideoConfig)
	ON_COMMAND(ID_CONFIG_EXIT, &CClientCodeDlg::OnConfigExit)
END_MESSAGE_MAP()


BOOL CClientCodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	hWnd = ::GetDlgItem(this->m_hWnd, IDC_VIDEO_ONE_CLIENTCODE_STATIC);  
	m_logout_menu.LoadMenu(IDR_MENU_LOGIN);
	this->SetMenu(&m_logout_menu);
	GetDlgItem(IDC_VIDEO_ONE_CLIENTCODE_STATIC)->ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control

}

void CClientCodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientCodeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CClientCodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��¼
void CClientCodeDlg::OnLogin()
{
	CLogin _login;
	if(_login.DoModal() == IDOK)
	{
		m_logout_menu.DestroyMenu();
		CString _serial_Config = L"";
		ifFind = finder.FindFile(_T("./Config.ini"));  
		if( ifFind )  
		{  
			::GetPrivateProfileStringW(_T("Serial Config Info"),_T("Config"),_T("FALSE"),_serial_Config.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
			_serial_Config.ReleaseBuffer();  
			if(_serial_Config == L"TRUE")
				m_login_menu.LoadMenu(IDR_MENU_CONFIGURE);
			else
				m_login_menu.LoadMenu(IDR_MENU_MAIN);
		}
		else
			m_login_menu.LoadMenu(IDR_MENU_MAIN);

		this->SetMenu(&m_login_menu);
		if(_serial_Config == L"TRUE")
		{
			//��ʼ�����δ���Ӵ��� ��ֹ��д����
			m_login_menu.EnableMenuItem(ID_READCARCARD, MF_GRAYED); 
			m_login_menu.EnableMenuItem(ID_WRITECARCARD, MF_GRAYED);
			//��ֹ��ʼ��Ƶ�ͽ�ͼ���ܺ�¼��
			m_login_menu.EnableMenuItem(ID_START_OR_STOP, MF_GRAYED); 
			m_login_menu.EnableMenuItem(ID_CAPTURE, MF_GRAYED); 
			m_login_menu.EnableMenuItem(ID_START_OR_STOP_RECORD, MF_GRAYED);
		}
	}
}

//����
void CClientCodeDlg::OnAbout()
{
	CAboutDlg _aboutDlg;
	_aboutDlg.DoModal();
}

//����
void CClientCodeDlg::OnConfig()
{
	CConfigDlg _configDlg;
	_configDlg.DoModal();
}

//�˳�
void CClientCodeDlg::OnClose()
{
	CDialog::OnCancel();
}

//ȡ��
void CClientCodeDlg::OnLogout()
{
	if(MessageBox(L"�Ƿ�Ҫע����ǰ�˺ţ�", L"��ʾ", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
	{
		m_login_menu.DestroyMenu();
		m_logout_menu.LoadMenu(IDR_MENU_LOGIN);
		this->SetMenu(&m_logout_menu);
	}
}


void CClientCodeDlg::OnReadcarcard()
{
	CReadCarCard _readCarCard;
	_readCarCard.DoModal();
}


void CClientCodeDlg::OnWritecarcard()
{
	CWriteCarCard _writeCarCard;
	_writeCarCard.DoModal();
}

void CClientCodeDlg::OnSerial()
{
	delete m_serial;
	m_serial = new CSerial();
	m_serial->Create(IDD_SERIAL_DIALOG);
	m_serial->ShowWindow(SW_SHOWNORMAL);
}

void CClientCodeDlg::OnParaset()
{
	//CParaSet _paraSet;
	//_paraSet.DoModal();
}

//���԰�ť
void CClientCodeDlg::OnBnClickedTextClientcodeButton()
{
	//if(_deviceOperation.Init())
	//{
	//	_deviceOperation.ReadCarCardInfo(this, IDC_CARCARDINFO_CLIENTCODE_EDIT);
	//}
	//else
	//	MessageBox(L"�豸��ʼ��ʧ�ܣ�");

	//TCP�ͻ���
	/*
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);

	//AF_INET��ʾTCP���� 
	//��ʼ��������˿ں� 
	SOCKADDR_IN addrSrv; 
	addrSrv.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");

	//������ַ,�������ڱ������� 
	addrSrv.sin_family=AF_INET;  
	addrSrv.sin_port=htons(6000);
	//���ö˿ں�
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	//���ӷ�����
	char recvBuf[50];  
	recv(sockClient,recvBuf,50,0);
	//�������� 
	send(sockClient,"hello",strlen("hello")+1,0);
	//��������
	closesocket(sockClient);
	//�ر�����
	
	SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);  
	SOCKADDR_IN addrSrv; 
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);  
	addrSrv.sin_family=AF_INET;  
	addrSrv.sin_port=htons(6000); 
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//�󶨶˿�  
	listen(sockSrv,5); 
	SOCKADDR_IN addrClient;//�����ϵĿͻ���IP��ַ  
	int len=sizeof(SOCKADDR);  
	while(1)  
	{ 
		SOCKET sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len); //���ܿͻ�������,��ȡ�ͻ��˵�IP��ַ   
		char sendBuf[50]; 
		sprintf_s(sendBuf,"Welcome %s to here!\n",inet_ntoa(addrClient.sin_addr));//�����Ϣ���ͳ�ȥ   
		send(sockConn,sendBuf,strlen(sendBuf)+1,0);//������Ϣ���ͻ���   
		char recvBuf[50]; 
		recv(sockConn,recvBuf,50,0);//���ܿͻ�����Ϣ   
		printf("%s\n",recvBuf); 
		//closesocket(sockConn);//�Ͽ�����  
	} 
	*/
}


//��¼���˳�����ͷ
void CClientCodeDlg::OnLoginOrLogout()
{
	if(!m_bIsLogin)
	{
		if(_deviceOperation.Init() && _deviceOperation.LoginVideo(2))
		{
			m_bIsLogin = true;
			m_login_menu.EnableMenuItem(ID_START_OR_STOP, MF_ENABLED); 
			m_login_menu.EnableMenuItem(ID_CAPTURE, MF_ENABLED);
			m_login_menu.ModifyMenuW(ID_LOGIN_OR_LOGOUT, MF_BYCOMMAND, ID_LOGIN_OR_LOGOUT, L"ע��");
			GetDlgItem(IDC_VIDEO_ONE_CLIENTCODE_STATIC)->ShowWindow(SW_SHOW);
		}
		else
			MessageBox(L"ע������ͷʧ�ܣ�");
	}
	else
	{
		if(m_bIsPlaying)
		{
			MessageBox(L"���ȹر�����ͷ��");
			return ;
		}
		if(_deviceOperation.LogoutVideo(2))
		{
			m_bIsLogin = false;
			m_login_menu.EnableMenuItem(ID_START_OR_STOP, MF_GRAYED); 
			m_login_menu.EnableMenuItem(ID_CAPTURE, MF_GRAYED); 
			m_login_menu.ModifyMenuW(ID_LOGIN_OR_LOGOUT, MF_BYCOMMAND, ID_LOGIN_OR_LOGOUT, L"��¼");
			GetDlgItem(IDC_VIDEO_ONE_CLIENTCODE_STATIC)->ShowWindow(SW_HIDE);
		}
	}
}

//��ʼ����ͣ����
void CClientCodeDlg::OnStartOrStop()
{
	if(!m_bIsPlaying)
	{
		 _deviceOperation.ClientInfo[2].hPlayWnd = hWnd;
		 if(_deviceOperation.StartPlay(2))
		 {
			m_login_menu.EnableMenuItem(ID_START_OR_STOP_RECORD, MF_ENABLED);
			m_bIsPlaying = TRUE;
			m_login_menu.ModifyMenuW(ID_START_OR_STOP, MF_BYCOMMAND, ID_START_OR_STOP, L"ֹͣ");
		 }
	}
	else
	{
		if(m_bIsRecording)
		{
			MessageBox(L"���ȹر�¼���ܣ�");
			return ;
		}
		if(_deviceOperation.StopPlay(2))
		{
			m_login_menu.EnableMenuItem(ID_START_OR_STOP_RECORD, MF_GRAYED);
			m_bIsPlaying = FALSE;
			m_login_menu.ModifyMenuW(ID_START_OR_STOP, MF_BYCOMMAND, ID_START_OR_STOP, L"��ʼ");
		}
		else
			 MessageBox(L"�ر�����ͷ�ɹ���");
	}
}

//ץͼ
void CClientCodeDlg::OnCapture()
{
	if(_deviceOperation.CDeviceCapture(2, L"picture"))
		MessageBox(L"ץͼ�ɹ���");
	else
		MessageBox(L"ץͼʧ�ܣ�");
}

//��������ͣ¼��
void CClientCodeDlg::OnStartOrStopRecord()
{
	if(m_bIsRecording)
	{
		//ֹͣ¼��
		if(_deviceOperation.CDeviceStopRecord(2))
		{
			m_bIsRecording = false;
			MessageBox(L"ֹͣ¼��ɹ���");
			m_login_menu.ModifyMenuW(ID_START_OR_STOP_RECORD, MF_BYCOMMAND, ID_START_OR_STOP_RECORD, L"¼��");
		}
		else
			MessageBox(L"ֹͣ¼��ʧ�ܣ�");
	}
	else
	{
		//��ʼ¼��
		if(_deviceOperation.CDeviceStartRecord(2, L"video"))
		{
			m_bIsRecording = true;
			MessageBox(L"��ʼ¼��");
			m_login_menu.ModifyMenuW(ID_START_OR_STOP_RECORD, MF_BYCOMMAND, ID_START_OR_STOP_RECORD, L"ֹͣ¼��");
		}
		else
			MessageBox(L"��ʼ¼��ʧ�ܣ�");
	}
}


//����ͷ����
void CClientCodeDlg::OnVideoConfig()
{
	CVideoConfig _videoConfig;
	_videoConfig.DoModal();
}

//���öԻ����˳�
void CClientCodeDlg::OnConfigExit()
{
	CDialog::OnCancel();
}
