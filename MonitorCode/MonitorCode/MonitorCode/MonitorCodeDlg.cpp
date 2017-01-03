
// MonitorCodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MonitorCode.h"
#include "MonitorCodeDlg.h"
#include "afxdialogex.h"
#include "OperateDataBase.h"
#include "ShowDiffInfo.h"
#include "HCNetSDK.h"
#pragma comment(lib, "HCNetSDK.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define WM_MYUPDATEDATA WM_USER+103
#define WM_ENABLEBUTTON WM_USER+104
#define SKIN    "skin/AquaOS.ssk"

#define MAX_LISTEN_NUM 5  
#define SEND_BUF_SIZE 100  
#define RECV_BUF_SIZE 100   

Log m_log;
CString m_config;   //存储配置文件路径
int m_port;    
CString m_carCardID;   //用于接收站点发来的违规车卡ID
HWND hwnd;  
SOCKET sockConn;
static DWORD WINAPI  ListenClient(void *pArg);

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


// CMonitorCodeDlg 对话框

CMonitorCodeDlg::CMonitorCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMonitorCodeDlg::IDD, pParent)
	, m_cargoSize_string(_T(""))
	, m_carCargoSize_string(_T(""))
	, m_cargoWeight_string(_T(""))
	, m_weightPlace_string(_T(""))
	, m_inOut_string(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	for(int i = 0; i < 5; i++)
		m_bFullScreen[i] = FALSE;
	hThread = NULL;
	m_font_static.CreatePointFont(200, L"宋体");
	m_font_edit.CreatePointFont(180, L"宋体");
	m_log.WriteOperateLog(L"登录系统！");
}

CMonitorCodeDlg::~CMonitorCodeDlg()
{
	m_font_static.DeleteObject();
	m_font_edit.DeleteObject();
	m_log.WriteOperateLog(L"注销系统！");
}

void CMonitorCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SELECT_COMBO, m_select_control);
	DDX_Text(pDX, IDC_CARGOSIZE_EDIT, m_cargoSize_string);
	DDX_Text(pDX, IDC_CARGOCARSIZE_EDIT, m_carCargoSize_string);
	DDX_Text(pDX, IDC_CARGOWEIGHT_EDIT, m_cargoWeight_string);
	DDX_Text(pDX, IDC_WEIGHTPLACE_EDIT, m_weightPlace_string);
	DDX_Text(pDX, IDC_INOUT_EDIT, m_inOut_string);
}

BEGIN_MESSAGE_MAP(CMonitorCodeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_COMMAND(ID_LOADCONFIG, &CMonitorCodeDlg::OnLoadconfig)
	ON_STN_DBLCLK(IDC_VIDEO4_STATIC, &CMonitorCodeDlg::OnStnDblclickVideo4Static)
	ON_STN_DBLCLK(IDC_VIDEO1_STATIC, &CMonitorCodeDlg::OnStnDblclickVideo1Static)
	ON_STN_DBLCLK(IDC_VIDEO2_STATIC, &CMonitorCodeDlg::OnStnDblclickVideo2Static)
	ON_STN_DBLCLK(IDC_VIDEO3_STATIC, &CMonitorCodeDlg::OnStnDblclickVideo3Static)
	ON_STN_DBLCLK(IDC_VIDEO5_STATIC, &CMonitorCodeDlg::OnStnDblclickVideo5Static)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_SELECT_COMBO, &CMonitorCodeDlg::OnCbnSelchangeSelectCombo)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_MYUPDATEDATA,OnUpdateMyData) 
	ON_MESSAGE(WM_ENABLEBUTTON,OnEnableButton) 
	ON_BN_CLICKED(IDC_DETAIL_BUTTON, &CMonitorCodeDlg::OnBnClickedDetailButton)
END_MESSAGE_MAP()


// CMonitorCodeDlg 消息处理程序

BOOL CMonitorCodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	HICON m_hIcon= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	skinppLoadSkin(SKIN);//项目下的皮肤文件
	ShowWindow(SW_MAXIMIZE);

	CRect rect;    
	GetClientRect(&rect);     //取客户区大小  
	old.x=rect.right-rect.left;
	old.y=rect.bottom-rect.top;
	
	hwnd = m_hWnd;
	//GetClientRect(&m_ClientRect);     //取客户区大小  
	m_newFont.CreateFont (   
            60, // nHeight 字体高度  
            20, // nWidth 字体宽度  
            0, // nEscapement 显示角度  
            0, // nOrientation 字体角度  
            FW_HEAVY, // nWeight 字体磅值  
            FALSE, // bItalic 是否倾斜  
            FALSE, // bUnderline 是否斜体  
            0, // cStrikeOut 是否加删除线  
            ANSI_CHARSET, // nCharSet 指定字符集  
            OUT_DEFAULT_PRECIS, // nOutPrecision 指定输出精度  
            CLIP_DEFAULT_PRECIS, // nClipPrecision 指定剪切精度  
            DEFAULT_QUALITY, // nQuality 指定输出质量  
            DEFAULT_PITCH | FF_SWISS, /*nPitchAndFamily 字符族*/_T("隶书")); 
	m_nTextX_Down = 10;   
    m_pOldFont = NULL; 
	SetTimer(0,200,NULL);   

	hWnd[0] = ::GetDlgItem(this->m_hWnd, IDC_VIDEO1_STATIC); 
	hWnd[1] = ::GetDlgItem(this->m_hWnd, IDC_VIDEO2_STATIC); 
	hWnd[2] = ::GetDlgItem(this->m_hWnd, IDC_VIDEO3_STATIC); 
	hWnd[3] = ::GetDlgItem(this->m_hWnd, IDC_VIDEO4_STATIC); 
	hWnd[4] = ::GetDlgItem(this->m_hWnd, IDC_VIDEO5_STATIC); 

	//默认所有控件不显示
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //列出所有控件 
	int woc;
	while(hwndChild)    
	{    
		woc=::GetDlgCtrlID(hwndChild);//取得ID
		GetDlgItem(woc)->ShowWindow(SW_HIDE);
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);   
	}

	for(int i = 1011; i <= 1017; i++)
	{
		skinppSetNoSkinHwnd(GetDlgItem(i)->m_hWnd, true);
		GetDlgItem(i)->SetFont(&m_font_static);
	}
	for(int i = 1005; i <= 1010; i++)
	{
		skinppSetNoSkinHwnd(GetDlgItem(i)->m_hWnd, true);
		GetDlgItem(i)->SetFont(&m_font_edit);
	}

	GetDlgItem(IDC_VIDEO1_STATIC)->GetWindowRect(m_rcWnd[0]);
	GetDlgItem(IDC_VIDEO2_STATIC)->GetWindowRect(m_rcWnd[1]);
	GetDlgItem(IDC_VIDEO3_STATIC)->GetWindowRect(m_rcWnd[2]);
	GetDlgItem(IDC_VIDEO4_STATIC)->GetWindowRect(m_rcWnd[3]);
	GetDlgItem(IDC_VIDEO5_STATIC)->GetWindowRect(m_rcWnd[4]);
	for(int i = 0; i< 5; i++)
		ScreenToClient(m_rcWnd[i]);
	m_rcScreen=CRect(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMonitorCodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMonitorCodeDlg::OnPaint()
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
		CPaintDC   dc(this);        
		LOGFONT logFont;   
		// 获取m_newFont字体的LOGFONT结构   
		m_newFont.GetLogFont(&logFont);
		m_pOldFont = (CFont*)dc.SelectObject(&m_newFont); 
		dc.SetBkMode(TRANSPARENT); //设置背景为透明！  
		 // 设置文本颜色为红色 
        dc.SetTextColor(RGB(255,0,0));         
        // 在指定位置输出文本   
        dc.TextOut(m_nTextX_Down,m_ClientRect.Height()-abs(logFont.lfHeight)-10,_T("奥博特科技公司监控站点"));   

		dc.SelectObject(m_pOldFont);  
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMonitorCodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMonitorCodeDlg::ReSize()
{
	float fsp[2];
	POINT Newp; //获取现在对话框的大小
	CRect recta;    
	GetClientRect(&recta);     //取客户区大小  
	Newp.x=recta.right-recta.left;
	Newp.y=recta.bottom-recta.top;
	fsp[0]=(float)Newp.x/old.x;
	fsp[1]=(float)Newp.y/old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint,TLPoint; //左上角
	CPoint OldBRPoint,BRPoint; //右下角
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //列出所有控件  
	while(hwndChild)    
	{    
		woc=::GetDlgCtrlID(hwndChild);//取得ID
		GetDlgItem(woc)->GetWindowRect(Rect);  
		ScreenToClient(Rect);  
		OldTLPoint = Rect.TopLeft(); 
		OldBRPoint = Rect.BottomRight(); 
		//当放大控件的时候，将下拉框的高度保持不变
		if(woc == IDC_SELECT_COMBO)
		{
			TLPoint.x = long(OldTLPoint.x*fsp[0]);  
			TLPoint.y = long(OldTLPoint.y*fsp[1]);  
			BRPoint.x = long(OldBRPoint.x*fsp[0]);  
			BRPoint.y = long(OldTLPoint.y*fsp[1]+OldBRPoint.y-OldTLPoint.y); 
		}
		else
		{
			TLPoint.x = long(OldTLPoint.x*fsp[0]);  
			TLPoint.y = long(OldTLPoint.y*fsp[1]);  
			BRPoint.x = long(OldBRPoint.x*fsp[0]);  
			BRPoint.y = long(OldBRPoint.y*fsp[1]); 
		} 
		Rect.SetRect(TLPoint,BRPoint);  
		GetDlgItem(woc)->MoveWindow(Rect,TRUE);
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);    
	}
	old=Newp;
}



void CMonitorCodeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType==SIZE_RESTORED||nType==SIZE_MAXIMIZED)  
    {  
		ReSize(); 
		CRect rect;    
		GetClientRect(&rect);     //取客户区大小  
		old.x=rect.right-rect.left;
		old.y=rect.bottom-rect.top;
    } 
	// TODO: 在此处添加消息处理程序代码
}


bool CMonitorCodeDlg::LoginVideo(int _camera)
{
	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
	memset(&DeviceInfoTmp,0,sizeof(NET_DVR_DEVICEINFO_V30));
	USES_CONVERSION;
	char *_videoIP = T2A(m_videoIP[_camera]);
	char *_videoUser = T2A(m_videoUser[_camera]);
	char *_videoPwd = T2A(m_videoPwd[_camera]);
	
	if(!NET_DVR_Init())
		MessageBox(L"摄像头SDK初始化失败！", L"错误",MB_OK); 
	LONG lLoginID = NET_DVR_Login_V30(_videoIP, m_videoPort[_camera], _videoUser, _videoPwd, &DeviceInfoTmp);
	if(lLoginID == -1)
	{
		DWORD err=NET_DVR_GetLastError();
		CString m_csErr;
        m_csErr.Format(L"登录摄像头出错，错误代码%d",err);
		MessageBox(m_csErr, L"错误", MB_OK);
		return FALSE;
	}
    m_videoDeviceInfo[_camera].lLoginID = lLoginID;
	m_videoDeviceInfo[_camera].iDeviceChanNum = DeviceInfoTmp.byChanNum;
    m_videoDeviceInfo[_camera].iIPChanNum = DeviceInfoTmp.byIPChanNum;
    m_videoDeviceInfo[_camera].iStartChan  = DeviceInfoTmp.byStartChan;
    m_videoDeviceInfo[_camera].iIPStartChan  = DeviceInfoTmp.byStartDChan;
	m_iCurChanIndex[_camera] = DeviceInfoTmp.byStartChan;
	
	//设置时间
	CTime CurTime = CTime::GetCurrentTime();
	NET_DVR_TIME _curTime;
	_curTime.dwYear = CurTime.GetYear();
	_curTime.dwMonth = CurTime.GetMonth();
	_curTime.dwDay = CurTime.GetDay();
	_curTime.dwHour = CurTime.GetHour();
	_curTime.dwMinute = CurTime.GetMinute();
	_curTime.dwSecond = CurTime.GetSecond();
	if(!NET_DVR_SetDVRConfig(m_videoDeviceInfo[_camera].lLoginID, NET_DVR_SET_TIMECFG, NULL, &_curTime, sizeof(NET_DVR_TIME)))
	{
		DWORD err=NET_DVR_GetLastError();
		CString m_csErr;
        m_csErr.Format(L"设置时间出错，错误代码%d",err);
		MessageBox(m_csErr, L"错误", MB_OK);
	}

	return TRUE;
}

bool CMonitorCodeDlg::LogoutVideo(int _camera)
{
	if(NET_DVR_Logout_V30(m_videoDeviceInfo[_camera].lLoginID))
	{
		NET_DVR_Cleanup();
		m_iCurChanIndex[_camera] = -1;
		return true;
	}
	else
		return false;
}

bool CMonitorCodeDlg::StartPlay(int _camera)
{
	ClientInfo[_camera].lChannel     = m_iCurChanIndex[_camera];
	ClientInfo[_camera].lLinkMode    = 0;
	ClientInfo[_camera].byProtoType = 0;
    ClientInfo[_camera].sMultiCastIP = NULL;
	TRACE("Channel number:%d\n",ClientInfo[_camera].lChannel);

	m_lPlayHandle[_camera] = NET_DVR_RealPlay_V30(m_videoDeviceInfo[_camera].lLoginID,&ClientInfo[_camera],NULL,NULL,TRUE);
	if(-1 == m_lPlayHandle[_camera])
	{
		DWORD err=NET_DVR_GetLastError();
		CString m_csErr;
        m_csErr.Format(L"播放出错，错误代码%d",err);
		MessageBox(m_csErr, L"错误", MB_OK);
		return false;
	}

	return true;
}


bool CMonitorCodeDlg::StopPlay(int _camera)
{
	if(m_lPlayHandle[_camera] != -1)
	{
		if(!NET_DVR_StopRealPlay(m_lPlayHandle[_camera]))
			return false;
		m_lPlayHandle[_camera]=-1;
	}
	else
		return false;

	return true;
}

void CMonitorCodeDlg::OnLoadconfig()
{
	LPCTSTR exTable = _T("文本文件 (*.ini)|*.ini|所有文件 (*.*)|*.*||");
    LPCTSTR defExtName = _T("txt");
    CFileDialog dlgOpenFiles(TRUE, defExtName, NULL, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, exTable);
    if(dlgOpenFiles.DoModal() == IDOK)
	{
		DWORD dw = WaitForSingleObject(hThread, 0);
		if(dw == WAIT_TIMEOUT)    //线程正在运行
		{
			TerminateThread(hThread, 0);    //强制结束

			if(hThread != NULL)
			{
				CloseHandle(hThread);    //关闭线程句柄，可能线程已结束，但句柄还没释放
				hThread = NULL;
			}
		}


		m_config = dlgOpenFiles.GetPathName();
		for(int i = 0; i < 5; i++)
		{
			CString _temp = L"";
			CString _IP = L"IP";
			CString _Port = L"Port";
			CString _User = L"User";
			CString _Pwd = L"Pwd";
			_temp.Format(L"%d", i+1);
			_IP += _temp;
			_Port += _temp;
			_User += _temp;
			_Pwd += _temp;
			::GetPrivateProfileString(_T("Video Config Info"),_IP,_T(""),m_videoIP[i].GetBuffer(MAX_PATH),MAX_PATH,m_config);
			::GetPrivateProfileString(_T("Video Config Info"),_Port,_T(""),_temp.GetBuffer(MAX_PATH),MAX_PATH,m_config);  
			m_videoPort[i] = _ttoi(_temp);
			::GetPrivateProfileString(_T("Video Config Info"),_User,_T(""),m_videoUser[i].GetBuffer(MAX_PATH),MAX_PATH,m_config);  
			::GetPrivateProfileString(_T("Video Config Info"),_Pwd,_T(""),m_videoPwd[i].GetBuffer(MAX_PATH),MAX_PATH,m_config);
		}

		//所有控件显示
		HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //列出所有控件 
		int woc;
		while(hwndChild)    
		{    
			woc=::GetDlgCtrlID(hwndChild);//取得ID
			GetDlgItem(woc)->ShowWindow(SW_SHOW);
			hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);   
		}

		for(int i = 0; i < 5; i++)
		{
			if(LoginVideo(i))
			{
				ClientInfo[i].hPlayWnd = hWnd[i];
				StartPlay(i);
			}
		}
		
		//开启线程监听客户端程序
		hThread = CreateThread(NULL,0,ListenClient,NULL,0,&m_threadID);
	}
}

//开启监听
static DWORD WINAPI  ListenClient(void *pArg)
{
	SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);  
	SOCKADDR_IN addrSrv; 
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);  
	addrSrv.sin_family=AF_INET;  
	CString _port;
	::GetPrivateProfileString(_T("NetWork Config Info"),_T("Monitor_Port"),_T("7000"),_port.GetBuffer(MAX_PATH),MAX_PATH,m_config);
	_port.ReleaseBuffer();
	m_port = _ttoi(_port);
	addrSrv.sin_port=htons(m_port); 
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//绑定端口  
	listen(sockSrv, MAX_LISTEN_NUM); 
	SOCKADDR_IN addrClient;//连接上的客户端IP地址  
	int len=sizeof(SOCKADDR);  

	while(1)  
	{ 
		sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&len); //接受客户端连接,获取客户端的IP地址    
		char recvBuf[RECV_BUF_SIZE]; 
		memset(recvBuf, '\0', RECV_BUF_SIZE);
		recv(sockConn,recvBuf,RECV_BUF_SIZE,0);//接收车卡ID   
		USES_CONVERSION;
		m_carCardID = A2T(recvBuf);
		::SendMessageW(hwnd, WM_MYUPDATEDATA, FALSE, 0);
		memset(recvBuf, '\0', RECV_BUF_SIZE);
		recv(sockConn,recvBuf,RECV_BUF_SIZE,0);//接收是否违规信息
		//判断是否违规
		if(!strcmp(recvBuf, "error"))
		{
			CString _error;
			::GetPrivateProfileString(_T("Monitor Place Info"),_T("Place"),_T("1"),_error.GetBuffer(MAX_PATH),MAX_PATH,m_config);
			_error.ReleaseBuffer();
			_error = L"检查站点 " + _error + L" 有车辆违规！\r\n违规车卡ID： " + m_carCardID;
			::SendMessageW(hwnd, WM_ENABLEBUTTON, TRUE, 0);
			AfxMessageBox(_error);
		}
		else
		{
			//没有进行违规操作
			closesocket(sockConn);//断开连接  
		}
	} 
}

LRESULT CMonitorCodeDlg::OnUpdateMyData(WPARAM wParam, LPARAM lParam)  
{
	for(int i = 1005; i <= 1017; i++)
		GetDlgItem(i)->ShowWindow(SW_HIDE);
	m_select_control.ResetContent();
	OperateDataBase _operateDataBase;
	tb_CarCheckInfo _carCheckInfo = _operateDataBase.getLastRecordByCarCardID(m_carCardID);
	m_cargoSize_string = _carCheckInfo.cargoSize;
	m_carCargoSize_string = _carCheckInfo.cargoCarSize;
	m_cargoWeight_string.Format(L"%f", _carCheckInfo.cargoWeight);
	m_weightPlace_string = L"监控站点：" + _carCheckInfo.weighPlace;
	if(_carCheckInfo.inOut)
		m_inOut_string = L"进站点";
	else
		m_inOut_string = L"出站点";
	m_select_control.AddString(L"本次记录");
	m_select_control.AddString(L"上次记录");
	m_select_control.SetCurSel(m_select_control.FindString(-1, L"本次记录"));
	
	for(int i = 1005; i <= 1017; i++)
		GetDlgItem(i)->ShowWindow(SW_SHOW);
	UpdateData(wParam); 
	return 1;
}

LRESULT CMonitorCodeDlg::OnEnableButton(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDC_DETAIL_BUTTON)->EnableWindow(wParam);

	return 1;
}

void CMonitorCodeDlg::ShowFullScreen(int _videoID, int _carmera)
{
	GetDlgItem(_videoID)->GetWindowRect(m_rcWnd[_carmera]);
	ScreenToClient(m_rcWnd[_carmera]);
	::SetWindowPos(GetDlgItem(_videoID)->m_hWnd,NULL,m_rcScreen.left,m_rcScreen.top,m_rcScreen.Width(),m_rcScreen.Height(),SWP_SHOWWINDOW);
	UpdateWindow();
	if(_videoID != IDC_VIDEO1_STATIC)
		GetDlgItem(IDC_VIDEO1_STATIC)->ShowWindow(SW_HIDE);
	if(_videoID != IDC_VIDEO2_STATIC)
		GetDlgItem(IDC_VIDEO2_STATIC)->ShowWindow(SW_HIDE);
	if(_videoID != IDC_VIDEO3_STATIC)
		GetDlgItem(IDC_VIDEO3_STATIC)->ShowWindow(SW_HIDE);
	if(_videoID != IDC_VIDEO4_STATIC)
		GetDlgItem(IDC_VIDEO4_STATIC)->ShowWindow(SW_HIDE);
	if(_videoID != IDC_VIDEO5_STATIC)
		GetDlgItem(IDC_VIDEO5_STATIC)->ShowWindow(SW_HIDE);
	//隐藏信息控件
	for(int i = 1005; i <= 1018; i++)
		GetDlgItem(i)->ShowWindow(SW_HIDE);
}


void CMonitorCodeDlg::ShowNormal(int _videoID, int _carmera)
{

	GetDlgItem(_videoID)->SetWindowPos(NULL,m_rcWnd[_carmera].left,m_rcWnd[_carmera].top,m_rcWnd[_carmera].Width(),m_rcWnd[_carmera].Height(),SWP_SHOWWINDOW);
	UpdateWindow();
	if(_videoID != IDC_VIDEO1_STATIC)
		GetDlgItem(IDC_VIDEO1_STATIC)->ShowWindow(SW_SHOW);
	if(_videoID != IDC_VIDEO2_STATIC)
		GetDlgItem(IDC_VIDEO2_STATIC)->ShowWindow(SW_SHOW);
	if(_videoID != IDC_VIDEO3_STATIC)
		GetDlgItem(IDC_VIDEO3_STATIC)->ShowWindow(SW_SHOW);
	if(_videoID != IDC_VIDEO4_STATIC)
		GetDlgItem(IDC_VIDEO4_STATIC)->ShowWindow(SW_SHOW);
	if(_videoID != IDC_VIDEO5_STATIC)
		GetDlgItem(IDC_VIDEO5_STATIC)->ShowWindow(SW_SHOW);
	//显示信息控件
	for(int i = 1005; i <= 1018; i++)
		GetDlgItem(i)->ShowWindow(SW_SHOW);
}

void CMonitorCodeDlg::OnStnDblclickVideo4Static()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bFullScreen[3]=!m_bFullScreen[3];
	//g_pMainDlg->FullScreen(m_bFullScreen);
	if (m_bFullScreen[3])
	{
		ShowFullScreen(IDC_VIDEO4_STATIC, 3);
	}
	else
	{
		ShowNormal(IDC_VIDEO4_STATIC, 3);
	}
}

void CMonitorCodeDlg::OnStnDblclickVideo1Static()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bFullScreen[0]=!m_bFullScreen[0];
	//g_pMainDlg->FullScreen(m_bFullScreen);
	if (m_bFullScreen[0])
	{
		ShowFullScreen(IDC_VIDEO1_STATIC, 0);
	}
	else
	{
		ShowNormal(IDC_VIDEO1_STATIC, 0);
	}
}


void CMonitorCodeDlg::OnStnDblclickVideo2Static()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bFullScreen[1]=!m_bFullScreen[1];
	//g_pMainDlg->FullScreen(m_bFullScreen);
	if (m_bFullScreen[1])
	{
		ShowFullScreen(IDC_VIDEO2_STATIC, 1);
	}
	else
	{
		ShowNormal(IDC_VIDEO2_STATIC, 1);
	}
}


void CMonitorCodeDlg::OnStnDblclickVideo3Static()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bFullScreen[2]=!m_bFullScreen[2];
	//g_pMainDlg->FullScreen(m_bFullScreen);
	if (m_bFullScreen[2])
	{
		ShowFullScreen(IDC_VIDEO3_STATIC, 2);
	}
	else
	{
		ShowNormal(IDC_VIDEO3_STATIC, 2);
	}

}


void CMonitorCodeDlg::OnStnDblclickVideo5Static()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bFullScreen[4]=!m_bFullScreen[4];
	//g_pMainDlg->FullScreen(m_bFullScreen);
	if (m_bFullScreen[4])
	{
		ShowFullScreen(IDC_VIDEO5_STATIC, 4);
	}
	else
	{
		ShowNormal(IDC_VIDEO5_STATIC, 4);
	}
}


void CMonitorCodeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(nIDEvent == 0)
	{
		CClientDC dc(this); // device context for painting   
        // 设置m_newFont对象的字体为当前字体，并将之前的字体指针保存到m_pOldFont   
		
		GetClientRect(&m_ClientRect);     
 
		m_nTextX_Down += 5;      
		if (m_nTextX_Down > m_ClientRect.Width() - 300)   
			m_nTextX_Down = 10;   

		LOGFONT logFont;   
		// 获取m_newFont字体的LOGFONT结构   
		m_newFont.GetLogFont(&logFont);
		//当放大画面后停止刷新流动字体
		if(!(m_bFullScreen[0] || m_bFullScreen[1] || m_bFullScreen[2] || m_bFullScreen[3] || m_bFullScreen[4]))
			InvalidateRect(CRect(0, m_ClientRect.Height()-abs(logFont.lfHeight)-20, m_ClientRect.Width(), m_ClientRect.Height()), TRUE);
	}
	CDialogEx::OnTimer(nIDEvent);
}

//选择上一次车证信息
void CMonitorCodeDlg::OnCbnSelchangeSelectCombo()
{
	for(int i = 1005; i <= 1017; i++)
		GetDlgItem(i)->ShowWindow(SW_HIDE);
	OperateDataBase _operateDataBase;
	tb_CarCheckInfo _carCheckInfo;
	if(m_select_control.GetCurSel() == m_select_control.FindString(-1, L"本次记录"))
	{
		_carCheckInfo = _operateDataBase.getLastRecordByCarCardID(m_carCardID);
		m_cargoSize_string = _carCheckInfo.cargoSize;
		m_carCargoSize_string = _carCheckInfo.cargoCarSize;
		m_cargoWeight_string.Format(L"%f", _carCheckInfo.cargoWeight);
		m_weightPlace_string = L"监控站点：" + _carCheckInfo.weighPlace;
		if(_carCheckInfo.inOut)
			m_inOut_string = L"进站点";
		else
			m_inOut_string = L"出站点";
	}
	else
	{
		_carCheckInfo = _operateDataBase.getPrevRecordByCarCardID(m_carCardID);
		m_cargoSize_string = _carCheckInfo.cargoSize;
		m_carCargoSize_string = _carCheckInfo.cargoCarSize;
		m_cargoWeight_string.Format(L"%f", _carCheckInfo.cargoWeight);
		m_weightPlace_string = L"监控站点：" + _carCheckInfo.weighPlace;
		if(_carCheckInfo.inOut)
			m_inOut_string = L"进站点";
		else
			m_inOut_string = L"出站点";
	}
	
	for(int i = 1005; i <= 1017; i++)
		GetDlgItem(i)->ShowWindow(SW_SHOW);
	UpdateData(FALSE); 
}


HBRUSH CMonitorCodeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if(pWnd->GetDlgCtrlID() >= 1011 && pWnd->GetDlgCtrlID() <= 1017)   
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,120));
	}
	else if(pWnd->GetDlgCtrlID() >= 1005 && pWnd->GetDlgCtrlID() <= 1010)
	{	
		pDC->SetTextColor(RGB(120,0,0));  
	}

	return (HBRUSH)GetStockObject(HOLLOW_BRUSH);   
}


void CMonitorCodeDlg::OnBnClickedDetailButton()
{
	ShowDiffInfo _showDiffInfo;
	//m_carCardID = L"000000001000000001000004";
	_showDiffInfo.m_carCard = m_carCardID;
	OperateDataBase _operateDataBase;
	tb_CarCheckInfo _carCheckInfo1, _carCheckInfo2;
	_carCheckInfo1 = _operateDataBase.getLastRecordByCarCardID(m_carCardID);
	_carCheckInfo2 = _operateDataBase.getPrevRecordByCarCardID(m_carCardID);
	//上次记录数据
	_showDiffInfo.m_cargoSize1 = _carCheckInfo2.cargoCarSize;
	_showDiffInfo.m_cargoWeight1.Format(L"%f", _carCheckInfo2.cargoWeight);
	_showDiffInfo.m_cargoDensity1.Format(L"%f", _carCheckInfo2.cargoDensity);
	_showDiffInfo.m_deductWeight1.Format(L"%f", _carCheckInfo2.deductWeight);
	_showDiffInfo.m_emptyCarSize1 = _carCheckInfo2.emptyCarSize;
	_showDiffInfo.m_carCargoSize1 = _carCheckInfo2.cargoCarSize;
	_showDiffInfo.m_emptyCarWeight1.Format(L"%f", _carCheckInfo2.emptyCarWeight);
	_showDiffInfo.m_carWeight1.Format(L"%f", _carCheckInfo2.CarWeight);
	//本次记录数据
	_showDiffInfo.m_cargoSize2 = _carCheckInfo1.cargoCarSize;
	_showDiffInfo.m_cargoWeight2.Format(L"%f", _carCheckInfo1.cargoWeight);
	_showDiffInfo.m_cargoDensity2.Format(L"%f", _carCheckInfo1.cargoDensity);
	_showDiffInfo.m_deductWeight2.Format(L"%f", _carCheckInfo1.deductWeight);
	_showDiffInfo.m_emptyCarSize2 = _carCheckInfo1.emptyCarSize;
	_showDiffInfo.m_carCargoSize2 = _carCheckInfo1.cargoCarSize;
	_showDiffInfo.m_emptyCarWeight2.Format(L"%f", _carCheckInfo1.emptyCarWeight);
	_showDiffInfo.m_carWeight2.Format(L"%f", _carCheckInfo1.CarWeight);

	//当有车辆违规操作时，由人工判断后向监控站点返回一个消息
	char sendBuf[SEND_BUF_SIZE];
	memset(sendBuf, '\0', SEND_BUF_SIZE);
	if(_showDiffInfo.DoModal() == IDOK)
		strcpy(sendBuf, "error");
	else
		strcpy(sendBuf, "abnormal");
	send(sockConn, sendBuf, SEND_BUF_SIZE, 0);

	closesocket(sockConn);

	GetDlgItem(IDC_DETAIL_BUTTON)->EnableWindow(FALSE);
}
