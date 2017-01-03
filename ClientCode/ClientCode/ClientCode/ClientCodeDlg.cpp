
// ClientCodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientCode.h"
#include "ClientCodeDlg.h"
#include "afxdialogex.h"
#include "ConfigDlg.h"
#include "Login.h"
#include "Log.h"
#include "ReadCarCard.h"
#include "WriteCarCard.h"
#include "VideoConfig.h"
#include "HRIVLprSdk.h"
#pragma comment(lib,"HRIVLprSdk.lib")
#include <Winsock2.h>  
#pragma comment(lib,"ws2_32.lib") 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_MYUPDATEDATA WM_USER+102

CString m_skin = L"";   //皮肤
#define MAX_LISTEN_NUM 5  
#define SEND_BUF_SIZE 100  
#define RECV_BUF_SIZE 100  
//商户卡ID为车卡信息里的前MERCHANT_SIZE个字符
//合同卡ID为车卡信息里的前AGREEMENT_SIZE个字符
#define MERCHANT_SIZE  9   //商户卡ID的长度
#define AGREEMENT_SIZE 18  //合同卡ID的长度

OperateDataBase _operateDataBase;
CString m_Monitor_IP;   //人工监控站点的IP
int m_Monitor_Port;     //人工监控站点的端口
CString m_site;    //站点编号(1-6)
HWND m_hwnd;
Log m_log;
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

static DWORD WINAPI  ListenCar(void *pArg);

// CClientCodeDlg 对话框
CClientCodeDlg::CClientCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientCodeDlg::IDD, pParent)
	, m_style_string(_T(""))
	, m_carCardID_string(_T(""))
	, m_emptyCarSize_string(_T(""))
	, m_cargoSize_string(_T(""))
	, m_cargoCarSize_string(_T(""))
	, m_emptyCarWeight_string(_T(""))
	, m_carWeight_string(_T(""))
	, m_cargoWeight_string(_T(""))
	, m_cargoDensity_string(_T(""))
	, m_deductWeight_string(_T(""))
	, m_weightPlace_string(_T(""))
	, m_abnormal_string(_T(""))
	, m_inOut_string(_T(""))
	, m_recordTime_string(_T(""))
	, m_agreementA_string(_T(""))
	, m_agreementB_string(_T(""))
	, m_reason1(false)
	, m_reason2(false)
	, m_reason3(false)
	, m_reason4(false)
	, m_reason5(false)
{
	m_bIsLogin = false;
	m_bIsPlaying = false;
	m_bIsRecording = false;
	m_isConfirmNoCarCard = false;
	m_reasonNumber = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_font_static.CreatePointFont(250, L"宋体");
	m_font_edit.CreatePointFont(200, L"宋体");
	m_log.WriteOperateLog(L"登录系统！");
}

CClientCodeDlg::~CClientCodeDlg()
{
	m_font_static.DeleteObject();
	m_font_edit.DeleteObject();
	m_log.WriteOperateLog(L"注销系统！");
}

void CClientCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CLIENTCODE_STYLE_EDIT, m_style_string);
	DDX_Text(pDX, IDC_CLIENTCODE_CARCARDID_EDIT, m_carCardID_string);
	DDX_Text(pDX, IDC_CLIENTCODE_EMPTYCARSIZE_EDIT, m_emptyCarSize_string);
	DDX_Text(pDX, IDC_CLIENTCODE_CARGOSIZE_EDIT, m_cargoSize_string);
	DDX_Text(pDX, IDC_CLIENTCODE_CARGOCARSIZE_EDIT, m_cargoCarSize_string);
	DDX_Text(pDX, IDC_CLIENTCODE_EMPTYCARWEIGHT_EDIT, m_emptyCarWeight_string);
	DDX_Text(pDX, IDC_CLIENTCODE_CARWIEIGHT_EDIT, m_carWeight_string);
	DDX_Text(pDX, IDC_CLIENTCODE_CARGOWEIGHT_EDIT, m_cargoWeight_string);
	DDX_Text(pDX, IDC_CLIENTCODE_CARGODENSITY_EDIT, m_cargoDensity_string);
	DDX_Text(pDX, IDC_CLIENTCODE_DEDUCTWEIGHT_EDIT, m_deductWeight_string);
	DDX_Text(pDX, IDC_CLIENTCODE_WEIGHTPLACE_EDIT, m_weightPlace_string);
	DDX_Text(pDX, IDC_CLIENTCODE_ABNORMAL_EDIT, m_abnormal_string);
	DDX_Text(pDX, IDC_CLIENTCODE_INOUT_EDIT, m_inOut_string);
	DDX_Text(pDX, IDC_CLIENTCODE_RECORDTIME_EDIT, m_recordTime_string);
	DDX_Text(pDX, IDC_CLIENTCODE_AGREEMENTA_EDIT, m_agreementA_string);
	DDX_Text(pDX, IDC_CLIENTCODE_AGREEMENTB_EDIT, m_agreementB_string);
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
	ON_COMMAND(ID_LOGIN_OR_LOGOUT, &CClientCodeDlg::OnLoginOrLogout)
	ON_COMMAND(ID_START_OR_STOP, &CClientCodeDlg::OnStartOrStop)
	ON_COMMAND(ID_CAPTURE, &CClientCodeDlg::OnCapture)
	ON_COMMAND(ID_START_OR_STOP_RECORD, &CClientCodeDlg::OnStartOrStopRecord)
	ON_COMMAND(ID_VIDEO_CONFIG, &CClientCodeDlg::OnVideoConfig)
	ON_COMMAND(ID_CONFIG_EXIT, &CClientCodeDlg::OnConfigExit)
	ON_WM_SIZE()
	ON_MESSAGE(WM_MYUPDATEDATA,OnUpdate) 
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_NOCARD_BUTTON, &CClientCodeDlg::OnBnClickedNocardButton)
END_MESSAGE_MAP()


BOOL CClientCodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HICON m_hIcon= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	::GetPrivateProfileStringW(_T("Dialog Skin Config Info"),_T("Skin"),_T("skin/BlueStandard.ssk"),m_skin.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
	m_skin.ReleaseBuffer();
	m_logout_menu.LoadMenu(IDR_MENU_LOGIN);
	this->SetMenu(&m_logout_menu);
	
	USES_CONVERSION;
	skinppLoadSkin(T2A(m_skin));//项目下的皮肤文件
	CRect rect;    
	GetClientRect(&rect);     //取客户区大小  
	old.x=rect.right-rect.left;
	old.y=rect.bottom-rect.top;

	ShowWindow(SW_MAXIMIZE);
	m_hwnd = m_hWnd;
	m_nTextX_Down = 10;
	//hWnd = ::GetDlgItem(this->m_hWnd, IDC_VIDEO_ONE_CLIENTCODE_STATIC);  
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

	//隐藏界面上的信息控件
	for(int i = 1112; i <= 1154; i++)
	{
		GetDlgItem(i)->ShowWindow(SW_HIDE);
	}

	OnLogin();

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
		GetClientRect(&m_ClientRect);  
		LOGFONT logFont;   
		// 获取m_newFont字体的LOGFONT结构   
		m_newFont.GetLogFont(&logFont);
		CPaintDC   dc(this);   
		m_pOldFont = (CFont*)dc.SelectObject(&m_newFont); 
		dc.SetBkMode(TRANSPARENT); //设置背景为透明！    
        // 设置文本颜色为红色
        dc.SetTextColor(RGB(255,0,0));       
        // 在指定位置输出文本   
        dc.TextOut(m_nTextX_Down,m_ClientRect.Height()-abs(logFont.lfHeight)-10,_T("谢谢关注奥博特科技公司"));   

		dc.SelectObject(m_pOldFont);  
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClientCodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//int test = 0;
LRESULT CClientCodeDlg::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	//CRect rt;
	//for(int i = 1112; i <= 1125; i++)
	//{
	//	GetDlgItem(i)->GetWindowRect(&rt);// IDC_current 选择刷新文本的ID
	//	ScreenToClient(&rt); //转到客户端界面
	//	InvalidateRect(&rt);//最后刷新对话框背景 
	//}
	for(int i = 1112; i <= 1127; i++)
		GetDlgItem(i)->ShowWindow(SW_HIDE);
	tb_CarCheckInfo *_tb_CarCheckInfo = (tb_CarCheckInfo *)wParam;
	//m_style_string = _tb_CarCheckInfo->agreementNo;
	m_carCardID_string = _tb_CarCheckInfo->carCardID;
	m_emptyCarSize_string = _tb_CarCheckInfo->emptyCarSize;
	m_cargoSize_string = _tb_CarCheckInfo->cargoSize;
	m_cargoCarSize_string = _tb_CarCheckInfo->cargoCarSize;
	m_emptyCarWeight_string.Format(L"%f", _tb_CarCheckInfo->emptyCarWeight);
	m_carWeight_string.Format(L"%f", _tb_CarCheckInfo->CarWeight);
	m_cargoWeight_string.Format(L"%f", _tb_CarCheckInfo->cargoWeight);
	m_cargoDensity_string.Format(L"%f", _tb_CarCheckInfo->cargoDensity);
	m_deductWeight_string.Format(L"%f", _tb_CarCheckInfo->deductWeight);
	m_weightPlace_string = _tb_CarCheckInfo->weighPlace;
	switch(_tb_CarCheckInfo->abnormal)
	{
	case 0:
		m_abnormal_string = L"正常"; break;
	case 1:
		m_abnormal_string = L"可能违规"; break;
	case 2:
		m_abnormal_string = L"违规"; break;
	default:
		m_abnormal_string = L""; break;
	}
	if(_tb_CarCheckInfo->inOut)
		m_inOut_string = L"进";
	else
		m_inOut_string = L"出";

	m_recordTime_string = _tb_CarCheckInfo->recordTime.Format("%Y-%m-%d %H:%M:%S %W-%A");
	m_style_string = _tb_CarCheckInfo->style;
	m_agreementA_string = _tb_CarCheckInfo->agreementA;
	m_agreementB_string = _tb_CarCheckInfo->agreementB;

	for(int i = 1112; i <= 1127; i++)
		GetDlgItem(i)->ShowWindow(SW_SHOW);

	UpdateData(FALSE);

	return 0;
}

//登录
void CClientCodeDlg::OnLogin()
{
	//CLogin _login;
	//if(_login.DoModal() == IDOK)
	{
		m_logout_menu.DestroyMenu();
		CString _load_Config = L"FALSE";
		ifFind = finder.FindFile(_T("./Config.ini"));  
		if( ifFind )  
		{  
			::GetPrivateProfileStringW(_T("Load Config Info"),_T("Config"),_T("FALSE"),_load_Config.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
			_load_Config.ReleaseBuffer();  
			if(_load_Config == L"TRUE")
				m_login_menu.LoadMenu(IDR_MENU_CONFIGURE);
			else
				m_login_menu.LoadMenu(IDR_MENU_MAIN);
		}
		else
		{
			m_log.WriteErrorLog(L"读取配置文件错误!");
			m_login_menu.LoadMenu(IDR_MENU_MAIN);
		}

		this->SetMenu(&m_login_menu);

		USES_CONVERSION;
		skinppLoadSkin(T2A(m_skin));//项目下的皮肤文件//项目下的皮肤文件
		
		if(_load_Config == L"TRUE")
		{
			//初始情况下未连接串口 禁止读写操作
			m_login_menu.EnableMenuItem(ID_READCARCARD, MF_GRAYED); 
			m_login_menu.EnableMenuItem(ID_WRITECARCARD, MF_GRAYED);
			//禁止开始视频和截图功能和录像
			m_login_menu.EnableMenuItem(ID_START_OR_STOP, MF_GRAYED); 
			m_login_menu.EnableMenuItem(ID_CAPTURE, MF_GRAYED); 
			m_login_menu.EnableMenuItem(ID_START_OR_STOP_RECORD, MF_GRAYED);
		}
		else
		{	
			for(int i = 1128; i <= 1154; i++)
			{
				skinppSetNoSkinHwnd(GetDlgItem(i)->m_hWnd, true);
				GetDlgItem(i)->SetFont(&m_font_static);
				
				GetDlgItem(i)->ShowWindow(SW_SHOW);
			}
			for(int i = 1112; i <= 1127; i++)
			{
				skinppSetNoSkinHwnd(GetDlgItem(i)->m_hWnd, true);
				GetDlgItem(i)->SetFont(&m_font_edit);
				GetDlgItem(i)->ShowWindow(SW_SHOW);
			}
			//开启滑动字幕
			SetTimer(0,200,NULL); 
			if(_deviceOperation.Init())
			{
				::GetPrivateProfileStringW(_T("NetWork Config Info"),_T("Monitor_IP"),_T(""),m_Monitor_IP.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
				m_Monitor_IP.ReleaseBuffer();
				CString _temp = L"";
				::GetPrivateProfileStringW(_T("NetWork Config Info"),_T("Monitor_Port"),_T(""),_temp.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
				_temp.ReleaseBuffer();
				m_Monitor_Port = _ttoi(_temp);
				::GetPrivateProfileStringW(_T("Monitor Place Info"),_T("Place"),_T("1"),m_site.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
				m_site.ReleaseBuffer();
				for(int i = 0; i < 5; i++)
					_deviceOperation.LoginVideo(i);

				//_deviceOperation.ClientInfo[1].hPlayWnd = hWnd;
				//_deviceOperation.StartPlay(1);
				//m_devThreadParameter.m_carCardThreadParameter.m_ShowID = IDC_CARCARDINFO_CLIENTCODE_EDIT;
				m_devThreadParameter.m_carCardThreadParameter.m_carCardInfo = L"";
				m_devThreadParameter.m_carCardThreadParameter.m_isSuccess = false;
				m_devThreadParameter.m_carCardThreadParameter.m_dialog = this;
				m_devThreadParameter.m_deviceOperation = &_deviceOperation;
				//开启线程
				hThread = CreateThread(NULL,0,ListenCar,&m_devThreadParameter,0,&m_threadID);
			}
			
		}
	}
}

//开启监听
static DWORD WINAPI  ListenCar(void *pArg)
{
	DeviceThreadParameter *m_parameter = (struct DeviceThreadParameter*)pArg;
	SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);  
	SOCKADDR_IN addrSrv; 
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);  
	addrSrv.sin_family=AF_INET;  
	CString _temp = L"";
	::GetPrivateProfileStringW(_T("NetWork Config Info"),_T("Local_Port"),_T("6000"),_temp.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
	_temp.ReleaseBuffer();
	addrSrv.sin_port=htons(_ttoi(_temp)); 
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//绑定端口  
	listen(sockSrv, MAX_LISTEN_NUM); 
	SOCKADDR_IN addrClient;//连接上的客户端IP地址  
	int len=sizeof(SOCKADDR);  
	while(1)
	{ 
		SOCKET sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&len); //接受客户端连接,获取客户端的IP地址    
		m_parameter->m_deviceOperation->m_carCardThreadParameter.m_noCarCard = false;
		char recvBuf[RECV_BUF_SIZE], sendBuf[SEND_BUF_SIZE]; 
		memset(recvBuf, '\0', RECV_BUF_SIZE);
		recv(sockConn,recvBuf,RECV_BUF_SIZE,0);//接收地磅信息  
		
		tb_CarCheckInfo _tb_CarCheckInfo;
		_tb_CarCheckInfo.CarWeight = atoi(recvBuf);
		m_parameter->m_deviceOperation->ReadCarCardInfo(m_parameter->m_carCardThreadParameter.m_dialog, 
			m_parameter->m_carCardThreadParameter.m_ShowID);
		//直到读取车卡成功或者按下没有车卡按钮
		while(!(m_parameter->m_deviceOperation->m_carCardThreadParameter.m_isSuccess))
		{
			if(m_parameter->m_deviceOperation->m_carCardThreadParameter.m_noCarCard)
				break;
			Sleep(500);   //防止线程死锁
		}
		memset(sendBuf, '\0', SEND_BUF_SIZE);
		if(m_parameter->m_deviceOperation->m_carCardThreadParameter.m_noCarCard)
		{	
			//没有车卡通知地磅可以放行车辆
			send(sockConn, sendBuf, SEND_BUF_SIZE, 0);
			closesocket(sockConn);//断开与地磅的连接
			continue;    //没有车卡
		}

		//获取车卡信息
		_tb_CarCheckInfo.carCardID = m_parameter->m_deviceOperation->m_carCardThreadParameter.m_carCardInfo;
		_tb_CarCheckInfo.agreementNo = _tb_CarCheckInfo.carCardID.Left(AGREEMENT_SIZE);
		//拍照
		for(int i = 0; i < 5; i++)
		{
			CString _temp;
			_temp.Format(L"%d", i);
			m_parameter->m_deviceOperation->CDeviceCapture(i, _tb_CarCheckInfo.carCardID+L"-"+m_site+L"-0-"+_temp);
		}

		CString _picType;
		if(m_parameter->m_deviceOperation->m_pictureType == L"BMP")
			_picType = L"bmp";
		else
			_picType = L"jpg";

		CString _plateNumber = L"";
		//车牌识别
		{
			//表示第一张图片的包括车牌的照片
			CString _picturePath;
			::GetPrivateProfileStringW(_T("Video Config Info"),_T("PicturePath"),_T(""),_picturePath.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
			_picturePath.ReleaseBuffer();
			_picturePath.Replace(L"\\", L"/");
			CString _filePath = _picturePath + L"/" + _tb_CarCheckInfo.carCardID+L"-"+m_site+L"-0-0."+_picType;
			USES_CONVERSION;
			HRIVLpr_Result _lpHRIVLpr_Result;
			long _result = HRIVLprInit();
			if(_result == 1)
			{
				_result = HRIVLprProcExNew(T2A(_filePath), &_lpHRIVLpr_Result);
				if(_result  = 1)
				{
					//成功
					if(_lpHRIVLpr_Result.num == 0)
					{
						m_log.WriteErrorLog(L"获取 "+_filePath+L" 图片车牌号为0个!");
					}
					else
					{
						_plateNumber = A2T(_lpHRIVLpr_Result.cLprCharacter[0]);
					}
				}
				else if(_result == -1)
				{
					m_log.WriteErrorLog(L"获取 "+_filePath+L" 图片车牌号失败!");
				}
				else if(_result == 0)
				{
					m_log.WriteErrorLog(L"图片 "+_filePath+L" 不支持获取车牌号!");
				}
			}
			else
				m_log.WriteErrorLog(L"获取 "+_filePath+L" 图片车牌号时，初始化加密狗失败！");
		}
			
		//上传图片
		m_parameter->m_deviceOperation->SendPicture(_tb_CarCheckInfo.carCardID, _ttoi(m_site), FALSE);
				
		_tb_CarCheckInfo.carPicture1 = _tb_CarCheckInfo.carCardID+L"-"+m_site+L"-0-0."+_picType;
		_tb_CarCheckInfo.carPicture2 = _tb_CarCheckInfo.carCardID+L"-"+m_site+L"-0-1."+_picType;
		_tb_CarCheckInfo.carPicture3 = _tb_CarCheckInfo.carCardID+L"-"+m_site+L"-0-2."+_picType;
		_tb_CarCheckInfo.carPicture4 = _tb_CarCheckInfo.carCardID+L"-"+m_site+L"-0-3."+_picType;
		_tb_CarCheckInfo.carPicture5 = _tb_CarCheckInfo.carCardID+L"-"+m_site+L"-0-4."+_picType;
		_tb_CarCheckInfo.weighPlace = m_site;
		_tb_CarCheckInfo.plateNumber = _plateNumber;
		//接收雷达扫描的结果

		//添加数据库
		OperateDataBase _operateDataBase;
		_operateDataBase.Add(_tb_CarCheckInfo);

		SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);
		SOCKADDR_IN addrSrv; 
		USES_CONVERSION;
		addrSrv.sin_addr.S_un.S_addr=inet_addr(T2A(m_Monitor_IP));
		//本机地址,服务器在本机开启 
		addrSrv.sin_family=AF_INET;  
		addrSrv.sin_port=htons(m_Monitor_Port);
		//设置端口号
		connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
		//连接服务器
		char _carCardID[SEND_BUF_SIZE];
		memset(_carCardID, '\0', SEND_BUF_SIZE);
		strcpy(_carCardID, W2A(_tb_CarCheckInfo.carCardID));
		//发送车卡ID
		send(sockClient,_carCardID,SEND_BUF_SIZE,0);
		memset(_carCardID, '\0', SEND_BUF_SIZE);
		//数据对比
		if(!strcmp(recvBuf, "111"))
			strcpy(_carCardID, "error");
		else
			strcpy(_carCardID, "abnormal");
		//发送是否违规
		send(sockClient,_carCardID,SEND_BUF_SIZE,0);
		//接收人工监控站点返回是否违规操作的信息
		memset(recvBuf, '\0', RECV_BUF_SIZE);
		recv(sockClient, recvBuf, RECV_BUF_SIZE, 0);
		//发送数据
		closesocket(sockClient);

		//读取车卡成功通知地磅可以放行车辆
		if(!strcmp(recvBuf, "error"))
			strcpy(sendBuf, "error");
		else
			strcpy(sendBuf, "abnormal");

		send(sockConn, sendBuf, SEND_BUF_SIZE, 0);
		closesocket(sockConn);//断开与地磅的连接

		//更新界面信息
		::SendMessageW(m_hwnd, WM_MYUPDATEDATA, (WPARAM)&_tb_CarCheckInfo, 0);
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


void CClientCodeDlg::OnSize(UINT nType, int cx, int cy)
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
}

void CClientCodeDlg::ReSize()
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
		TLPoint.x = long(OldTLPoint.x*fsp[0]);  
		TLPoint.y = long(OldTLPoint.y*fsp[1]);  
		OldBRPoint = Rect.BottomRight();  
		BRPoint.x = long(OldBRPoint.x*fsp[0]);  
		BRPoint.y = long(OldBRPoint.y*fsp[1]);  
		Rect.SetRect(TLPoint,BRPoint);  
		GetDlgItem(woc)->MoveWindow(Rect,TRUE);
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);    
	}
	old=Newp;
}



void CClientCodeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == 0)
	{
		GetClientRect(&m_ClientRect); 
		m_nTextX_Down += 5;      
		if (m_nTextX_Down > m_ClientRect.Width() - 300)   
			m_nTextX_Down = 10;   

		//Invalidate();
		LOGFONT logFont;   
		// 获取m_newFont字体的LOGFONT结构   
		m_newFont.GetLogFont(&logFont);
		InvalidateRect(CRect(0, m_ClientRect.Height()-abs(logFont.lfHeight)-20, m_ClientRect.Width(), m_ClientRect.Height()), TRUE);
	}

	CDialogEx::OnTimer(nIDEvent);
}


HBRUSH CClientCodeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkMode(TRANSPARENT);
	if(pWnd->GetDlgCtrlID() >= 1112 && pWnd->GetDlgCtrlID() <= 1127)      
		pDC->SetTextColor(RGB(0,0,120));  
	if(pWnd->GetDlgCtrlID() >= 1128 && pWnd->GetDlgCtrlID() <= 1149)
		pDC->SetTextColor(RGB(120,0,120)); 
	if(pWnd->GetDlgCtrlID() >= 1150 && pWnd->GetDlgCtrlID() <= 1154)
	{
		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		CDC* dc = GetDC();
		pDC->BitBlt(0,0,rc.Width(),rc.Height(),dc,rc.left,rc.top,SRCCOPY);
		//把父窗口背景图片先画到按钮上
		ReleaseDC(dc);
	}
	return (HBRUSH)GetStockObject(HOLLOW_BRUSH);   
}


void CClientCodeDlg::OnBnClickedNocardButton()
{
	m_reason1 = (bool)((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON1_RADIO))->GetCheck();
	m_reason2 = (bool)((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON2_RADIO))->GetCheck();
	m_reason3 = (bool)((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON3_RADIO))->GetCheck();
	m_reason4 = (bool)((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON4_RADIO))->GetCheck();
	m_reason5 = (bool)((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON5_RADIO))->GetCheck();
	if(m_isConfirmNoCarCard)
	{
		//判断是否选择了没有车卡原因
		if(m_reason1 || m_reason2 || m_reason3 ||
			m_reason4 || m_reason5)
		{
			if(m_reason1)
				m_reasonNumber = 1;
			else if(m_reason2)
				m_reasonNumber = 2;
			else if(m_reason3)
				m_reasonNumber = 3;
			else if(m_reason4)
				m_reasonNumber = 4;
			else if(m_reason5)
				m_reasonNumber = 5;
		}
		else
		{
			MessageBox(L"请选择下列的原因！", L"提示");
			return ;
		}

		GetDlgItem(IDC_NOCARD_BUTTON)->SetWindowTextW(L"没有车卡");
		m_isConfirmNoCarCard = false;
		((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON1_RADIO))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON2_RADIO))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON3_RADIO))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON4_RADIO))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON5_RADIO))->EnableWindow(FALSE);
		//停止刷卡提示
		if(!m_devThreadParameter.m_deviceOperation->m_carCardThreadParameter.m_noCarCard)
			m_devThreadParameter.m_deviceOperation->m_carCardThreadParameter.m_noCarCard = true;
		//插入没有车卡原因
		tb_NoCarCardInfo _noCarCardInfo;
		_noCarCardInfo.reasonNumber = m_reasonNumber;
		_operateDataBase.Add(_noCarCardInfo);
	}
	else
	{
		m_isConfirmNoCarCard = true;
		((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON1_RADIO))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON2_RADIO))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON3_RADIO))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON4_RADIO))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON5_RADIO))->EnableWindow(TRUE);
		//默认没有选择原因
		switch (m_reasonNumber)
		{
		case 1:
			((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON1_RADIO))->SetCheck(false);
			break;
		case 2:
			((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON2_RADIO))->SetCheck(false);
			break;
		case 3:
			((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON3_RADIO))->SetCheck(false);
			break;
		case 4:
			((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON4_RADIO))->SetCheck(false);
			break;
		case 5:
			((CButton*)GetDlgItem(IDC_CLIENTCODE_REASON5_RADIO))->SetCheck(false);
			break;
		default:
			break;
		}
		m_reasonNumber = 0;
		GetDlgItem(IDC_NOCARD_BUTTON)->SetWindowTextW(L"确认");
	}
}

