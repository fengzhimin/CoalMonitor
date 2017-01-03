
// MonitorCodeDlg.cpp : ʵ���ļ�
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
CString m_config;   //�洢�����ļ�·��
int m_port;    
CString m_carCardID;   //���ڽ���վ�㷢����Υ�泵��ID
HWND hwnd;  
SOCKET sockConn;
static DWORD WINAPI  ListenClient(void *pArg);

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


// CMonitorCodeDlg �Ի���

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
	m_font_static.CreatePointFont(200, L"����");
	m_font_edit.CreatePointFont(180, L"����");
	m_log.WriteOperateLog(L"��¼ϵͳ��");
}

CMonitorCodeDlg::~CMonitorCodeDlg()
{
	m_font_static.DeleteObject();
	m_font_edit.DeleteObject();
	m_log.WriteOperateLog(L"ע��ϵͳ��");
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


// CMonitorCodeDlg ��Ϣ�������

BOOL CMonitorCodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	HICON m_hIcon= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	skinppLoadSkin(SKIN);//��Ŀ�µ�Ƥ���ļ�
	ShowWindow(SW_MAXIMIZE);

	CRect rect;    
	GetClientRect(&rect);     //ȡ�ͻ�����С  
	old.x=rect.right-rect.left;
	old.y=rect.bottom-rect.top;
	
	hwnd = m_hWnd;
	//GetClientRect(&m_ClientRect);     //ȡ�ͻ�����С  
	m_newFont.CreateFont (   
            60, // nHeight ����߶�  
            20, // nWidth ������  
            0, // nEscapement ��ʾ�Ƕ�  
            0, // nOrientation ����Ƕ�  
            FW_HEAVY, // nWeight �����ֵ  
            FALSE, // bItalic �Ƿ���б  
            FALSE, // bUnderline �Ƿ�б��  
            0, // cStrikeOut �Ƿ��ɾ����  
            ANSI_CHARSET, // nCharSet ָ���ַ���  
            OUT_DEFAULT_PRECIS, // nOutPrecision ָ���������  
            CLIP_DEFAULT_PRECIS, // nClipPrecision ָ�����о���  
            DEFAULT_QUALITY, // nQuality ָ���������  
            DEFAULT_PITCH | FF_SWISS, /*nPitchAndFamily �ַ���*/_T("����")); 
	m_nTextX_Down = 10;   
    m_pOldFont = NULL; 
	SetTimer(0,200,NULL);   

	hWnd[0] = ::GetDlgItem(this->m_hWnd, IDC_VIDEO1_STATIC); 
	hWnd[1] = ::GetDlgItem(this->m_hWnd, IDC_VIDEO2_STATIC); 
	hWnd[2] = ::GetDlgItem(this->m_hWnd, IDC_VIDEO3_STATIC); 
	hWnd[3] = ::GetDlgItem(this->m_hWnd, IDC_VIDEO4_STATIC); 
	hWnd[4] = ::GetDlgItem(this->m_hWnd, IDC_VIDEO5_STATIC); 

	//Ĭ�����пؼ�����ʾ
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //�г����пؼ� 
	int woc;
	while(hwndChild)    
	{    
		woc=::GetDlgCtrlID(hwndChild);//ȡ��ID
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMonitorCodeDlg::OnPaint()
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
		CPaintDC   dc(this);        
		LOGFONT logFont;   
		// ��ȡm_newFont�����LOGFONT�ṹ   
		m_newFont.GetLogFont(&logFont);
		m_pOldFont = (CFont*)dc.SelectObject(&m_newFont); 
		dc.SetBkMode(TRANSPARENT); //���ñ���Ϊ͸����  
		 // �����ı���ɫΪ��ɫ 
        dc.SetTextColor(RGB(255,0,0));         
        // ��ָ��λ������ı�   
        dc.TextOut(m_nTextX_Down,m_ClientRect.Height()-abs(logFont.lfHeight)-10,_T("�²��ؿƼ���˾���վ��"));   

		dc.SelectObject(m_pOldFont);  
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMonitorCodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMonitorCodeDlg::ReSize()
{
	float fsp[2];
	POINT Newp; //��ȡ���ڶԻ���Ĵ�С
	CRect recta;    
	GetClientRect(&recta);     //ȡ�ͻ�����С  
	Newp.x=recta.right-recta.left;
	Newp.y=recta.bottom-recta.top;
	fsp[0]=(float)Newp.x/old.x;
	fsp[1]=(float)Newp.y/old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint,TLPoint; //���Ͻ�
	CPoint OldBRPoint,BRPoint; //���½�
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //�г����пؼ�  
	while(hwndChild)    
	{    
		woc=::GetDlgCtrlID(hwndChild);//ȡ��ID
		GetDlgItem(woc)->GetWindowRect(Rect);  
		ScreenToClient(Rect);  
		OldTLPoint = Rect.TopLeft(); 
		OldBRPoint = Rect.BottomRight(); 
		//���Ŵ�ؼ���ʱ�򣬽�������ĸ߶ȱ��ֲ���
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
		GetClientRect(&rect);     //ȡ�ͻ�����С  
		old.x=rect.right-rect.left;
		old.y=rect.bottom-rect.top;
    } 
	// TODO: �ڴ˴������Ϣ����������
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
		MessageBox(L"����ͷSDK��ʼ��ʧ�ܣ�", L"����",MB_OK); 
	LONG lLoginID = NET_DVR_Login_V30(_videoIP, m_videoPort[_camera], _videoUser, _videoPwd, &DeviceInfoTmp);
	if(lLoginID == -1)
	{
		DWORD err=NET_DVR_GetLastError();
		CString m_csErr;
        m_csErr.Format(L"��¼����ͷ�����������%d",err);
		MessageBox(m_csErr, L"����", MB_OK);
		return FALSE;
	}
    m_videoDeviceInfo[_camera].lLoginID = lLoginID;
	m_videoDeviceInfo[_camera].iDeviceChanNum = DeviceInfoTmp.byChanNum;
    m_videoDeviceInfo[_camera].iIPChanNum = DeviceInfoTmp.byIPChanNum;
    m_videoDeviceInfo[_camera].iStartChan  = DeviceInfoTmp.byStartChan;
    m_videoDeviceInfo[_camera].iIPStartChan  = DeviceInfoTmp.byStartDChan;
	m_iCurChanIndex[_camera] = DeviceInfoTmp.byStartChan;
	
	//����ʱ��
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
        m_csErr.Format(L"����ʱ������������%d",err);
		MessageBox(m_csErr, L"����", MB_OK);
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
        m_csErr.Format(L"���ų����������%d",err);
		MessageBox(m_csErr, L"����", MB_OK);
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
	LPCTSTR exTable = _T("�ı��ļ� (*.ini)|*.ini|�����ļ� (*.*)|*.*||");
    LPCTSTR defExtName = _T("txt");
    CFileDialog dlgOpenFiles(TRUE, defExtName, NULL, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, exTable);
    if(dlgOpenFiles.DoModal() == IDOK)
	{
		DWORD dw = WaitForSingleObject(hThread, 0);
		if(dw == WAIT_TIMEOUT)    //�߳���������
		{
			TerminateThread(hThread, 0);    //ǿ�ƽ���

			if(hThread != NULL)
			{
				CloseHandle(hThread);    //�ر��߳̾���������߳��ѽ������������û�ͷ�
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

		//���пؼ���ʾ
		HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //�г����пؼ� 
		int woc;
		while(hwndChild)    
		{    
			woc=::GetDlgCtrlID(hwndChild);//ȡ��ID
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
		
		//�����̼߳����ͻ��˳���
		hThread = CreateThread(NULL,0,ListenClient,NULL,0,&m_threadID);
	}
}

//��������
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
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//�󶨶˿�  
	listen(sockSrv, MAX_LISTEN_NUM); 
	SOCKADDR_IN addrClient;//�����ϵĿͻ���IP��ַ  
	int len=sizeof(SOCKADDR);  

	while(1)  
	{ 
		sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&len); //���ܿͻ�������,��ȡ�ͻ��˵�IP��ַ    
		char recvBuf[RECV_BUF_SIZE]; 
		memset(recvBuf, '\0', RECV_BUF_SIZE);
		recv(sockConn,recvBuf,RECV_BUF_SIZE,0);//���ճ���ID   
		USES_CONVERSION;
		m_carCardID = A2T(recvBuf);
		::SendMessageW(hwnd, WM_MYUPDATEDATA, FALSE, 0);
		memset(recvBuf, '\0', RECV_BUF_SIZE);
		recv(sockConn,recvBuf,RECV_BUF_SIZE,0);//�����Ƿ�Υ����Ϣ
		//�ж��Ƿ�Υ��
		if(!strcmp(recvBuf, "error"))
		{
			CString _error;
			::GetPrivateProfileString(_T("Monitor Place Info"),_T("Place"),_T("1"),_error.GetBuffer(MAX_PATH),MAX_PATH,m_config);
			_error.ReleaseBuffer();
			_error = L"���վ�� " + _error + L" �г���Υ�棡\r\nΥ�泵��ID�� " + m_carCardID;
			::SendMessageW(hwnd, WM_ENABLEBUTTON, TRUE, 0);
			AfxMessageBox(_error);
		}
		else
		{
			//û�н���Υ�����
			closesocket(sockConn);//�Ͽ�����  
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
	m_weightPlace_string = L"���վ�㣺" + _carCheckInfo.weighPlace;
	if(_carCheckInfo.inOut)
		m_inOut_string = L"��վ��";
	else
		m_inOut_string = L"��վ��";
	m_select_control.AddString(L"���μ�¼");
	m_select_control.AddString(L"�ϴμ�¼");
	m_select_control.SetCurSel(m_select_control.FindString(-1, L"���μ�¼"));
	
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
	//������Ϣ�ؼ�
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
	//��ʾ��Ϣ�ؼ�
	for(int i = 1005; i <= 1018; i++)
		GetDlgItem(i)->ShowWindow(SW_SHOW);
}

void CMonitorCodeDlg::OnStnDblclickVideo4Static()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(nIDEvent == 0)
	{
		CClientDC dc(this); // device context for painting   
        // ����m_newFont���������Ϊ��ǰ���壬����֮ǰ������ָ�뱣�浽m_pOldFont   
		
		GetClientRect(&m_ClientRect);     
 
		m_nTextX_Down += 5;      
		if (m_nTextX_Down > m_ClientRect.Width() - 300)   
			m_nTextX_Down = 10;   

		LOGFONT logFont;   
		// ��ȡm_newFont�����LOGFONT�ṹ   
		m_newFont.GetLogFont(&logFont);
		//���Ŵ����ֹͣˢ����������
		if(!(m_bFullScreen[0] || m_bFullScreen[1] || m_bFullScreen[2] || m_bFullScreen[3] || m_bFullScreen[4]))
			InvalidateRect(CRect(0, m_ClientRect.Height()-abs(logFont.lfHeight)-20, m_ClientRect.Width(), m_ClientRect.Height()), TRUE);
	}
	CDialogEx::OnTimer(nIDEvent);
}

//ѡ����һ�γ�֤��Ϣ
void CMonitorCodeDlg::OnCbnSelchangeSelectCombo()
{
	for(int i = 1005; i <= 1017; i++)
		GetDlgItem(i)->ShowWindow(SW_HIDE);
	OperateDataBase _operateDataBase;
	tb_CarCheckInfo _carCheckInfo;
	if(m_select_control.GetCurSel() == m_select_control.FindString(-1, L"���μ�¼"))
	{
		_carCheckInfo = _operateDataBase.getLastRecordByCarCardID(m_carCardID);
		m_cargoSize_string = _carCheckInfo.cargoSize;
		m_carCargoSize_string = _carCheckInfo.cargoCarSize;
		m_cargoWeight_string.Format(L"%f", _carCheckInfo.cargoWeight);
		m_weightPlace_string = L"���վ�㣺" + _carCheckInfo.weighPlace;
		if(_carCheckInfo.inOut)
			m_inOut_string = L"��վ��";
		else
			m_inOut_string = L"��վ��";
	}
	else
	{
		_carCheckInfo = _operateDataBase.getPrevRecordByCarCardID(m_carCardID);
		m_cargoSize_string = _carCheckInfo.cargoSize;
		m_carCargoSize_string = _carCheckInfo.cargoCarSize;
		m_cargoWeight_string.Format(L"%f", _carCheckInfo.cargoWeight);
		m_weightPlace_string = L"���վ�㣺" + _carCheckInfo.weighPlace;
		if(_carCheckInfo.inOut)
			m_inOut_string = L"��վ��";
		else
			m_inOut_string = L"��վ��";
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
	//�ϴμ�¼����
	_showDiffInfo.m_cargoSize1 = _carCheckInfo2.cargoCarSize;
	_showDiffInfo.m_cargoWeight1.Format(L"%f", _carCheckInfo2.cargoWeight);
	_showDiffInfo.m_cargoDensity1.Format(L"%f", _carCheckInfo2.cargoDensity);
	_showDiffInfo.m_deductWeight1.Format(L"%f", _carCheckInfo2.deductWeight);
	_showDiffInfo.m_emptyCarSize1 = _carCheckInfo2.emptyCarSize;
	_showDiffInfo.m_carCargoSize1 = _carCheckInfo2.cargoCarSize;
	_showDiffInfo.m_emptyCarWeight1.Format(L"%f", _carCheckInfo2.emptyCarWeight);
	_showDiffInfo.m_carWeight1.Format(L"%f", _carCheckInfo2.CarWeight);
	//���μ�¼����
	_showDiffInfo.m_cargoSize2 = _carCheckInfo1.cargoCarSize;
	_showDiffInfo.m_cargoWeight2.Format(L"%f", _carCheckInfo1.cargoWeight);
	_showDiffInfo.m_cargoDensity2.Format(L"%f", _carCheckInfo1.cargoDensity);
	_showDiffInfo.m_deductWeight2.Format(L"%f", _carCheckInfo1.deductWeight);
	_showDiffInfo.m_emptyCarSize2 = _carCheckInfo1.emptyCarSize;
	_showDiffInfo.m_carCargoSize2 = _carCheckInfo1.cargoCarSize;
	_showDiffInfo.m_emptyCarWeight2.Format(L"%f", _carCheckInfo1.emptyCarWeight);
	_showDiffInfo.m_carWeight2.Format(L"%f", _carCheckInfo1.CarWeight);

	//���г���Υ�����ʱ�����˹��жϺ�����վ�㷵��һ����Ϣ
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
