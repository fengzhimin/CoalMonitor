
// ClientCodeDlg.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientCodeDlg 对话框
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientCodeDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClientCodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//登录
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
			//初始情况下未连接串口 禁止读写操作
			m_login_menu.EnableMenuItem(ID_READCARCARD, MF_GRAYED); 
			m_login_menu.EnableMenuItem(ID_WRITECARCARD, MF_GRAYED);
			//禁止开始视频和截图功能和录像
			m_login_menu.EnableMenuItem(ID_START_OR_STOP, MF_GRAYED); 
			m_login_menu.EnableMenuItem(ID_CAPTURE, MF_GRAYED); 
			m_login_menu.EnableMenuItem(ID_START_OR_STOP_RECORD, MF_GRAYED);
		}
	}
}

//关于
void CClientCodeDlg::OnAbout()
{
	CAboutDlg _aboutDlg;
	_aboutDlg.DoModal();
}

//配置
void CClientCodeDlg::OnConfig()
{
	CConfigDlg _configDlg;
	_configDlg.DoModal();
}

//退出
void CClientCodeDlg::OnClose()
{
	CDialog::OnCancel();
}

//取消
void CClientCodeDlg::OnLogout()
{
	if(MessageBox(L"是否要注销当前账号？", L"提示", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
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

//测试按钮
void CClientCodeDlg::OnBnClickedTextClientcodeButton()
{
	//if(_deviceOperation.Init())
	//{
	//	_deviceOperation.ReadCarCardInfo(this, IDC_CARCARDINFO_CLIENTCODE_EDIT);
	//}
	//else
	//	MessageBox(L"设备初始化失败！");

	//TCP客户端
	/*
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);

	//AF_INET表示TCP连接 
	//初始化连接与端口号 
	SOCKADDR_IN addrSrv; 
	addrSrv.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");

	//本机地址,服务器在本机开启 
	addrSrv.sin_family=AF_INET;  
	addrSrv.sin_port=htons(6000);
	//设置端口号
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	//连接服务器
	char recvBuf[50];  
	recv(sockClient,recvBuf,50,0);
	//接受数据 
	send(sockClient,"hello",strlen("hello")+1,0);
	//发送数据
	closesocket(sockClient);
	//关闭连接
	
	SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);  
	SOCKADDR_IN addrSrv; 
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);  
	addrSrv.sin_family=AF_INET;  
	addrSrv.sin_port=htons(6000); 
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//绑定端口  
	listen(sockSrv,5); 
	SOCKADDR_IN addrClient;//连接上的客户端IP地址  
	int len=sizeof(SOCKADDR);  
	while(1)  
	{ 
		SOCKET sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len); //接受客户端连接,获取客户端的IP地址   
		char sendBuf[50]; 
		sprintf_s(sendBuf,"Welcome %s to here!\n",inet_ntoa(addrClient.sin_addr));//组合消息发送出去   
		send(sockConn,sendBuf,strlen(sendBuf)+1,0);//发送消息到客户端   
		char recvBuf[50]; 
		recv(sockConn,recvBuf,50,0);//接受客户端消息   
		printf("%s\n",recvBuf); 
		//closesocket(sockConn);//断开连接  
	} 
	*/
}


//登录或退出摄像头
void CClientCodeDlg::OnLoginOrLogout()
{
	if(!m_bIsLogin)
	{
		if(_deviceOperation.Init() && _deviceOperation.LoginVideo(2))
		{
			m_bIsLogin = true;
			m_login_menu.EnableMenuItem(ID_START_OR_STOP, MF_ENABLED); 
			m_login_menu.EnableMenuItem(ID_CAPTURE, MF_ENABLED);
			m_login_menu.ModifyMenuW(ID_LOGIN_OR_LOGOUT, MF_BYCOMMAND, ID_LOGIN_OR_LOGOUT, L"注销");
			GetDlgItem(IDC_VIDEO_ONE_CLIENTCODE_STATIC)->ShowWindow(SW_SHOW);
		}
		else
			MessageBox(L"注册摄像头失败！");
	}
	else
	{
		if(m_bIsPlaying)
		{
			MessageBox(L"请先关闭摄像头！");
			return ;
		}
		if(_deviceOperation.LogoutVideo(2))
		{
			m_bIsLogin = false;
			m_login_menu.EnableMenuItem(ID_START_OR_STOP, MF_GRAYED); 
			m_login_menu.EnableMenuItem(ID_CAPTURE, MF_GRAYED); 
			m_login_menu.ModifyMenuW(ID_LOGIN_OR_LOGOUT, MF_BYCOMMAND, ID_LOGIN_OR_LOGOUT, L"登录");
			GetDlgItem(IDC_VIDEO_ONE_CLIENTCODE_STATIC)->ShowWindow(SW_HIDE);
		}
	}
}

//开始或暂停播放
void CClientCodeDlg::OnStartOrStop()
{
	if(!m_bIsPlaying)
	{
		 _deviceOperation.ClientInfo[2].hPlayWnd = hWnd;
		 if(_deviceOperation.StartPlay(2))
		 {
			m_login_menu.EnableMenuItem(ID_START_OR_STOP_RECORD, MF_ENABLED);
			m_bIsPlaying = TRUE;
			m_login_menu.ModifyMenuW(ID_START_OR_STOP, MF_BYCOMMAND, ID_START_OR_STOP, L"停止");
		 }
	}
	else
	{
		if(m_bIsRecording)
		{
			MessageBox(L"请先关闭录像功能！");
			return ;
		}
		if(_deviceOperation.StopPlay(2))
		{
			m_login_menu.EnableMenuItem(ID_START_OR_STOP_RECORD, MF_GRAYED);
			m_bIsPlaying = FALSE;
			m_login_menu.ModifyMenuW(ID_START_OR_STOP, MF_BYCOMMAND, ID_START_OR_STOP, L"开始");
		}
		else
			 MessageBox(L"关闭摄像头成功！");
	}
}

//抓图
void CClientCodeDlg::OnCapture()
{
	if(_deviceOperation.CDeviceCapture(2, L"picture"))
		MessageBox(L"抓图成功！");
	else
		MessageBox(L"抓图失败！");
}

//开启或暂停录像
void CClientCodeDlg::OnStartOrStopRecord()
{
	if(m_bIsRecording)
	{
		//停止录像
		if(_deviceOperation.CDeviceStopRecord(2))
		{
			m_bIsRecording = false;
			MessageBox(L"停止录像成功！");
			m_login_menu.ModifyMenuW(ID_START_OR_STOP_RECORD, MF_BYCOMMAND, ID_START_OR_STOP_RECORD, L"录像");
		}
		else
			MessageBox(L"停止录像失败！");
	}
	else
	{
		//开始录像
		if(_deviceOperation.CDeviceStartRecord(2, L"video"))
		{
			m_bIsRecording = true;
			MessageBox(L"开始录像！");
			m_login_menu.ModifyMenuW(ID_START_OR_STOP_RECORD, MF_BYCOMMAND, ID_START_OR_STOP_RECORD, L"停止录像");
		}
		else
			MessageBox(L"开始录像失败！");
	}
}


//摄像头配置
void CClientCodeDlg::OnVideoConfig()
{
	CVideoConfig _videoConfig;
	_videoConfig.DoModal();
}

//配置对话框退出
void CClientCodeDlg::OnConfigExit()
{
	CDialog::OnCancel();
}
