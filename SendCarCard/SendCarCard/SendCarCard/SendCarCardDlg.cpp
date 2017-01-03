
// SendCarCardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SendCarCard.h"
#include "SendCarCardDlg.h"
#include "afxdialogex.h"
#include "Voice.h"
#include "RFIDDevice.h"
#include "OperateDataBase.h"
#include "afxwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_MYUPDATEDATA WM_USER+100  


#define SHOW_COMPANY_INFO_IDEVENT                 0      //显示公司信息的事件ID

#define SENDCARCARD_TO_WRITEAREA  "02 44 48 03 0D 05"   //发卡到写卡区域
#define SENDCARCARD_FROM_WRITEAREA_TO_OUT "02 43 46 03 04 05"  //将卡从写卡区域发送到收卡区域
#define SENDCARCARD_TO_RECOVER    "02 43 50 03 12 05"   //回收卡
#define INQUIREWORKSTATS   "02 52 46 03 15 05"   //查询工作状态

#define SENDCARCARD_TO_WRITEAREA_IDEVENT           1    //发卡到写卡区域事件ID
#define INQUIREWORKSTATS_IDEVENT                   2    //查询工作状态事件ID
#define SENDCARCARD_FROM_WRITEAREA_TO_OUT_IDEVENT  3    //将卡从写卡区域发送到收卡区域事件ID
#define SENDCARCARD_TO_RECOVER_IDEVENT             4    //回收卡事件ID

#define SEND_TO_WRITEAREA_RATE                     2000   //发送卡到写卡区域的频率(ms)
#define INQUIREWORKSTATS_RATE                      100   //查询工作状态频率(ms)
#define SEND_FROM_WRITEAREA_TO_OUT_RATE            200   //完全出卡的频率(ms)
#define SEND_TO_RECOVER_RATE                       100   //回收车卡的频率(ms)

#define CALLBACKCARCARD_CALLBACK_IDEVENT           5    //收卡机回收状态事件ID
#define CALLBACKCARCARD_REFUSE_IDEVENT             6    //收卡机退卡状态事件ID
#define CALLBACKCARCARD_INQUIRE_IDEVENT            7    //收卡机查询工作状态事件ID

#define CALLBACKCARCARD_INQUIREWORKSTATS         "7f 40 01 07 0d 12"   //收卡机查询命令
#define CALLBACKCARCARD_CALLBACK                 "7f 40 01 20 0d c0"   //收卡机收卡命令
#define CALLBACKCARCARD_REFUSE                   "7f 40 01 21 8d c5"   //收卡机退卡命令 

#define CALLBACKCARCARD_INQUIRE_RATE                100  //收卡机查询工作状态频率(ms)

CSerialPort m_SendCardSerialPort;   //发卡机的串口
//CSerialPort m_CallBackSerialPort;   //收卡机的串口
CString m_SendCarCardstrReceive;    //发卡机接收的字符
//CString m_CallCarCardstrReceive;    //收卡机接收的字符
CString m_merchantID_sending;       //正在发送车卡的商户ID
CString m_agreementID_sending;      //正在发送车卡的合同ID
CRFIDDevice m_writeInfo;//, m_readInfo;
OperateDataBase _operate;
int m_reReadRate;       //重复读卡频率
int m_reWriteRate;      //重复写卡频率
DWORD  m_threadID;
HANDLE hThread;
Log m_log;
LPCWSTR m_speakInfo;
static DWORD WINAPI SpeakInfo(void *pArg);

//定时器器触函数：将车卡从写卡区域完全发出
void CALLBACK Send_from_writearea_to_out(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime);
//定时器器触函数：将车卡发送到写卡区域
void CALLBACK Send_to_writearea(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime);
//定时器器触函数：将车卡从写卡区域发送到回收区域
void CALLBACK Send_from_writearea_to_recover(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime);
//定时器器触函数：发送查询指令
void CALLBACK InquireWorkStat(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime);

//定时器触发函数：收卡机查询命令
//void CALLBACK CallBackCarCard_Inquire(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime);
//定时器触发函数：收卡机回收车卡
//void CALLBACK CallBackCarCard_CallBack(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime);
//定时器触发函数：收卡机退出车卡
//void CALLBACK CallBackCarCard_Refuse(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime);

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


// CSendCarCardDlg 对话框



CSendCarCardDlg::CSendCarCardDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSendCarCardDlg::IDD, pParent)
	, m_merchantID(_T(""))
{
	m_receiveSendCarCardCommand = false;
	//m_receiveCallBackCarCardCommand = false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_font_static.CreatePointFont(300, "宋体");
	m_font_edit.CreatePointFont(200, "宋体");
	m_font_button.CreatePointFont(400, "宋体");
	m_log.WriteOperateLog("登录系统！");
}

CSendCarCardDlg::~CSendCarCardDlg()
{
	m_font_static.DeleteObject();
	m_font_edit.DeleteObject();
	m_font_button.DeleteObject();
	m_log.WriteOperateLog("注销系统！");
}

void CSendCarCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MERCHANT_EDIT, m_merchantID);
	DDX_Control(pDX, IDC_AGREEMENT_COMBO, m_agreementID);
}

BEGIN_MESSAGE_MAP(CSendCarCardDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND_BUTTON, &CSendCarCardDlg::OnBnClickedTextButton)
	ON_MESSAGE(WM_COMM_RXCHAR,OnReceiveChar)
	ON_BN_CLICKED(IDC_STOP_BUTTON, &CSendCarCardDlg::OnBnClickedStopButton)
	ON_BN_CLICKED(IDC_START_BUTTON, &CSendCarCardDlg::OnBnClickedStartButton)
	ON_MESSAGE(WM_MYUPDATEDATA,OnUpdateMyData) 
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSendCarCardDlg 消息处理程序

BOOL CSendCarCardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	HICON m_hIcon= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
   
	GetClientRect(&m_ClientRect);     //取客户区大小  
	old.x=m_ClientRect.right - m_ClientRect.left;
	old.y=m_ClientRect.bottom - m_ClientRect.top;
	ShowWindow(SW_MAXIMIZE);

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

    // 设置定时器，定时时间为100ms   
	SetTimer(SHOW_COMPANY_INFO_IDEVENT,200,NULL);   
	m_nTextX_Up = m_ClientRect.Width() - 300;   
	m_nTextX_Down = 10;   
    m_pOldFont = NULL; 

	GetDlgItem(IDC_START_BUTTON)->SetFont(&m_font_button);
	GetDlgItem(IDC_MERCHANT_STATIC)->SetFont(&m_font_static);
	GetDlgItem(IDC_AGREEMENT_STATIC)->SetFont(&m_font_static);
	GetDlgItem(IDC_SEND_BUTTON)->SetFont(&m_font_static);
	GetDlgItem(IDC_STOP_BUTTON)->SetFont(&m_font_static);
	GetDlgItem(IDC_MERCHANT_EDIT)->SetFont(&m_font_static);
	GetDlgItem(IDC_AGREEMENT_COMBO)->SetFont(&m_font_edit);

	GetDlgItem(IDC_MERCHANT_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_MERCHANT_EDIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_AGREEMENT_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_AGREEMENT_COMBO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STOP_BUTTON)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SEND_BUTTON)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_INSTRUCTION_EDIT)->SetFont(&m_font_edit);
	GetDlgItem(IDC_INSTRUCTION_EDIT)->SetWindowTextA("操作指南：\r\n第一：首先将商话卡插入读卡区域!\r\n第二：点击打印车卡按钮!\r\n第三：等待读取商户卡信息！\r\n第四：选择要打印的合同信息！\r\n第五：点击打印，开始打印车卡！\r\n第六：等待打印完毕车卡，点击退出！ \r\n");

	CFileFind finder;  
	BOOL ifFind = finder.FindFile(_T("./Config.ini"));  
	if( ifFind )  
	{  
		CString _temp;
		//发卡机的串口
		::GetPrivateProfileStringA("SendCardSerial Config Info","Com","",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");  
		_temp.ReleaseBuffer();
		_temp.Trim();
		m_SendCardComm = _temp.GetAt(3) - '0';
		::GetPrivateProfileStringA("SendCardSerial Config Info","BaudRate","",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");  
		_temp.ReleaseBuffer();
		_temp.Trim();
		m_SendCardBaudRate = _ttoi(_temp);
		//发卡机的RFID设备串口
		::GetPrivateProfileStringA("SendCarCardRFID Config Info","Com","",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");  
		_temp.ReleaseBuffer();
		_temp.Trim();
		m_SendCardRFIDComm = _temp.GetAt(3) - '0';
		::GetPrivateProfileStringA("SendCarCardRFID Config Info","BaudRate","",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");  
		_temp.ReleaseBuffer();
		_temp.Trim();
		m_SendCardRFIDBaudRate = _ttoi(_temp);
		////收卡机的串口
		//::GetPrivateProfileStringA("CallBackCardSerial Config Info","Com","",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");  
		//_temp.ReleaseBuffer();
		//_temp.Trim();
		//m_CallBackCardComm = _temp.GetAt(3) - '0';
		//::GetPrivateProfileStringA("CallBackCardSerial Config Info","BaudRate", "",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");     
		//_temp.ReleaseBuffer();
		//_temp.Trim();
		//m_CallBackCardBaudRate = _ttoi(_temp);
		////收卡机的RFID设备串口
		//::GetPrivateProfileStringA("CallBackCarCardRFID Config Info","Com","",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");  
		//_temp.ReleaseBuffer();
		//_temp.Trim();
		//m_CallBackCardRFIDComm = _temp.GetAt(3) - '0';
		//::GetPrivateProfileStringA("CallBackCarCardRFID Config Info","BaudRate", "",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");     
		//_temp.ReleaseBuffer();
		//_temp.Trim();
		//m_CallBackCardRFIDBaudRate = _ttoi(_temp);
		//发卡机重复写卡的次数
		::GetPrivateProfileStringA("WriteAgainRate Config Info","Rate", "5",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");     
		_temp.ReleaseBuffer();
		_temp.Trim();
		m_reWriteRate = _ttoi(_temp);
		::GetPrivateProfileStringA("Dialog Skin Config Info", "Skin", "skin/adamant.she", m_skin.GetBuffer(MAX_PATH), MAX_PATH, "./Config.ini");  
		m_skin.ReleaseBuffer();
	}
	else
	{
		m_log.WriteErrorLog("读取配置文件错误!");
		m_SendCardComm = m_CallBackCardComm = 0;
		m_SendCardBaudRate = m_CallBackCardBaudRate = 9600;
		m_skin = "skin/adamant.she";
	}
	
	SkinH_AttachEx(m_skin, NULL); //这句核心

	if(!m_writeInfo.Initial(m_SendCardRFIDComm, m_SendCardRFIDBaudRate))
	{
		m_log.WriteErrorLog("RFID写卡设备初始化失败！");
		MessageBox("RFID写卡设备初始化失败");
	}
	
	/*if(!m_readInfo.Initial(m_CallBackCardRFIDComm, m_CallBackCardRFIDBaudRate))
	{
		m_log.WriteErrorLog("RFID读卡设备初始化失败！");
		MessageBox("RFID读卡设备初始化失败");
	}*/
	//监听发卡机的串口
	if(m_SendCardSerialPort.InitPort(this,m_SendCardComm,m_SendCardBaudRate,'N',8,1,EV_RXCHAR|EV_CTS,1024))		
		m_SendCardSerialPort.StartMonitoring();
	else
	{
		m_log.WriteErrorLog("发卡机串口打开失败！");
		AfxMessageBox("发卡机串口打开失败！");
	}

	//监听收卡机的串口
	/*if(m_CallBackSerialPort.InitPort(this,m_CallBackCardComm,m_CallBackCardBaudRate,'N',8,1,EV_RXCHAR|EV_CTS,1024))		
		m_CallBackSerialPort.StartMonitoring();
	else 
	{
		m_log.WriteErrorLog("收卡机串口打开失败！");
		AfxMessageBox("收卡机串口打开失败！");
	}*/

	//开启收卡机查询状态
	//SetTimer(CALLBACKCARCARD_INQUIRE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Inquire);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CSendCarCardDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSendCarCardDlg::OnPaint()
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
		/*CRect   rect;    
		CPaintDC   dc(this);    
		GetClientRect(rect);    
		dc.FillSolidRect(rect,RGB(120,120,120));*/     //设置为绿色背景  
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
        dc.TextOut(m_nTextX_Up,10,_T("欢迎来到自动取卡机"));   
        // 设置文本颜色为绿色   
        dc.SetTextColor(RGB(0,255,0));       
        // 在指定位置输出文本   
        dc.TextOut(m_nTextX_Down,m_ClientRect.Height()-abs(logFont.lfHeight)-10,_T("谢谢关注奥博特科技公司"));   

		dc.SelectObject(m_pOldFont);  

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSendCarCardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSendCarCardDlg::HexStringFilter(CString &str)
{
	BOOL bOK;
	for(int i=0;i<str.GetLength();)
	{
		bOK=((str.GetAt(i)>='0')&&(str.GetAt(i)<='9'))||
			((str.GetAt(i)>='A')&&(str.GetAt(i)<='F'))||
			((str.GetAt(i)>='a')&&(str.GetAt(i)<='f'));
		if(!bOK)
			str.Delete(i);
		else i++;	
	}
}

//更新合同下拉列表
LRESULT CSendCarCardDlg::OnUpdateMyData(WPARAM wParam, LPARAM lParam)  
{  
	m_agreementID.EnableWindow(TRUE);
	m_agreementID.DeleteString(0);
	m_agreementID.SetCurSel(0);
    UpdateData(wParam);  
    return 1;  
}  

char CSendCarCardDlg::CombineHexChar(char CharH,char CharL)
{
	char result;
	CString temp;
	if(CharH>='0'&&CharH<='9')			result=(CharH-'0');
	else if(CharH>='a'&&CharH<='f')		result=(CharH-'a'+10);
	else if(CharH>='A'&&CharH<='F')		result=(CharH-'A'+10);
	else								result=0;
	result<<=4;	
	if(CharL>='0'&&CharL<='9')			result+=(CharL-'0');
	else if(CharL>='a'&&CharL<='f')		result+=(CharL-'a'+10);
	else if(CharL>='A'&&CharL<='F')		result+=(CharL-'A'+10);
	else								result+=0;
	return result;
}

CString CSendCarCardDlg::ChangeCharstr2Hexstr(CString Charstr)
{
	CString Hexstr=_T("");
	Charstr.MakeUpper();
	HexStringFilter(Charstr);
	int Length=Charstr.GetLength();
	if(Length%2)
		Charstr.Delete(Length-1);
	Length=Charstr.GetLength();
	for(int i=0;i<Length/2;i++)
	{
		Hexstr+=CombineHexChar(Charstr.GetAt(i*2),Charstr.GetAt(i*2+1));
	}
	return Hexstr;
}

CString CSendCarCardDlg::DevideHexChar(char HexChar)
{
	CString result=_T("");
	int temp=(HexChar&0xF0)>>4;
	if(temp<10)
		result+=char(temp+'0');
	else 
		result+=char(temp+'A'-10);
	temp=HexChar&0x0F;
	if(temp<10)
		result+=char(temp+'0');
	else 
		result+=char(temp+'A'-10);
	return result;
}

//BOOL m_callBackReceiveOne = false, m_callBackReceiveTwo = false;
LRESULT CSendCarCardDlg::OnReceiveChar(UINT ch, LONG port)
{
	if(port == m_SendCardComm)
	{
		CString _receiveStr;
		_receiveStr = DevideHexChar(ch);
		if(_receiveStr == "03")
		{
			m_receiveSendCarCardCommand = false;  //停止接受
			m_SendCarCardstrReceive = m_SendCarCardstrReceive.Right(6);  //截取Flag Code
		}
		if(m_receiveSendCarCardCommand)
			m_SendCarCardstrReceive += _receiveStr;

		if(_receiveStr == "02")
		{
			m_SendCarCardstrReceive = "";
			m_receiveSendCarCardCommand = true;  //开始接受
		}
	}
	/*else if(port == m_CallBackCardComm)
	{
		CString _receiveStr;
		_receiveStr = DevideHexChar(ch);

		if(m_receiveCallBackCarCardCommand)
		{
			m_CallCarCardstrReceive = _receiveStr;
			m_receiveCallBackCarCardCommand = false;
		}

		if(m_callBackReceiveOne && m_callBackReceiveTwo && _receiveStr == "01")
		{
			m_callBackReceiveOne = false;
			m_callBackReceiveTwo = false;
			m_CallCarCardstrReceive = "";
			m_receiveCallBackCarCardCommand = true;
		}else if(m_callBackReceiveOne)
		{
			if(_receiveStr == "40")
				m_callBackReceiveTwo = true;
			else
			{
				m_callBackReceiveOne = false;
				m_callBackReceiveTwo = false;
				if(_receiveStr == "7f" || _receiveStr == "7F")
					m_callBackReceiveOne = true;
			}
		}else if(_receiveStr == "7f" || _receiveStr == "7F")
			m_callBackReceiveOne = true;
		else
		{
			m_callBackReceiveOne = false;
			m_callBackReceiveTwo = false;
		}
	}*/
	
	return 0;
}

void CSendCarCardDlg::OnBnClickedTextButton()
{
	UpdateData();
	m_agreementID.GetLBText(m_agreementID.GetCurSel(), m_strAgreementID);
	m_merchantID_sending = m_merchantID;
	m_agreementID_sending = m_strAgreementID;
	if(!_operate.MerchantIsExist(m_merchantID))
	{
		MessageBox("商户卡ID不存在！");
		m_log.WriteErrorLog("商户卡ID不存在！");
		return ;
	}
	else if(!_operate.AgreementIsExist(m_strAgreementID))
	{
		MessageBox("合同卡ID不存在！");
		m_log.WriteErrorLog("合同卡ID不存在！");
		return ;
	}
	else
		_operate.getCarCardInfo();
	//开始打印车卡的时候禁止合同ID选择
	m_agreementID.EnableWindow(FALSE); 
	SetTimer(SENDCARCARD_TO_WRITEAREA_IDEVENT, SEND_TO_WRITEAREA_RATE, Send_to_writearea);
}

void CALLBACK Send_from_writearea_to_out(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)
{
	CString _strSendCommand = SENDCARCARD_FROM_WRITEAREA_TO_OUT;
	_strSendCommand=CSendCarCardDlg::ChangeCharstr2Hexstr(_strSendCommand);
	m_SendCardSerialPort.WriteToPort(_strSendCommand.GetBuffer(_strSendCommand.GetLength()), _strSendCommand.GetLength());
	KillTimer(hwnd, SENDCARCARD_FROM_WRITEAREA_TO_OUT_IDEVENT);
	_operate.AddUseCarCard();   //成功发送车卡
	if(_operate.m_carCardCount == _operate.m_useCarCardCount)
	{
		m_speakInfo = LPCWSTR(L"发送车卡完毕！");
		m_log.WriteOperateLog("商户："+m_merchantID_sending+" 合同："+m_agreementID_sending+" 发卡完毕");
		hThread = CreateThread(NULL,0,SpeakInfo,NULL,0,&m_threadID);
		CloseHandle(hThread);
		AfxGetMainWnd()->SendMessage(WM_MYUPDATEDATA, FALSE);
	}
	else
		SetTimer(hwnd, SENDCARCARD_TO_WRITEAREA_IDEVENT, SEND_TO_WRITEAREA_RATE, Send_to_writearea);
}

void CALLBACK Send_to_writearea(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)
{
	if(_operate.m_carCardCount == _operate.m_useCarCardCount)
	{
		KillTimer(hwnd, SENDCARCARD_TO_WRITEAREA_IDEVENT);
		return ;
	}
	CString _strSendCommand = SENDCARCARD_TO_WRITEAREA;
	_strSendCommand=CSendCarCardDlg::ChangeCharstr2Hexstr(_strSendCommand);
	m_SendCardSerialPort.WriteToPort(_strSendCommand.GetBuffer(_strSendCommand.GetLength()), _strSendCommand.GetLength());
	KillTimer(hwnd, SENDCARCARD_TO_WRITEAREA_IDEVENT);
	SetTimer(hwnd, INQUIREWORKSTATS_IDEVENT, INQUIREWORKSTATS_RATE, InquireWorkStat);
}

void CALLBACK Send_from_writearea_to_recover(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)
{
	CString _strSendCommand = SENDCARCARD_TO_RECOVER;
	_strSendCommand=CSendCarCardDlg::ChangeCharstr2Hexstr(_strSendCommand);
	m_SendCardSerialPort.WriteToPort(_strSendCommand.GetBuffer(_strSendCommand.GetLength()), _strSendCommand.GetLength());
	KillTimer(hwnd, SENDCARCARD_TO_RECOVER_IDEVENT);
	SetTimer(hwnd, SENDCARCARD_TO_WRITEAREA_IDEVENT, SEND_TO_WRITEAREA_RATE, Send_to_writearea);
}

void CALLBACK InquireWorkStat(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)
{
	//查询工作状态
	CString _strSendCommand = INQUIREWORKSTATS;
	_strSendCommand=CSendCarCardDlg::ChangeCharstr2Hexstr(_strSendCommand);
	m_SendCardSerialPort.WriteToPort(_strSendCommand.GetBuffer(_strSendCommand.GetLength()), _strSendCommand.GetLength());
	KillTimer(hwnd, INQUIREWORKSTATS_IDEVENT);
	if(m_SendCarCardstrReceive == "")   //没有接受返回值
		SetTimer(hwnd, INQUIREWORKSTATS_IDEVENT, INQUIREWORKSTATS_RATE, InquireWorkStat);
	else
	{
		int _receiveD1 = _ttoi(m_SendCarCardstrReceive.Left(2).Right(1));
		int _receiveD2 = _ttoi(m_SendCarCardstrReceive.Left(4).Right(2).Right(1));
		int _receiveD3 = _ttoi(m_SendCarCardstrReceive.Right(2).Right(1));
		CVoice _voice;
		_voice.Initial();
		if(_receiveD1 & 0x02)
		{
			_voice.SpeakInfo(L"出卡机不能正常出卡!");
			m_log.WriteErrorLog("商户："+m_merchantID_sending+" 合同："+m_agreementID_sending+" 发卡过程中出卡机不能正常出卡!");
		}
		else if(_receiveD2 & 0x02)
		{
			_voice.SpeakInfo(L"出卡机送卡通道堵卡!");
			m_log.WriteErrorLog("商户："+m_merchantID_sending+" 合同："+m_agreementID_sending+" 发卡过程中出卡机送卡通道堵卡!");
		}
		else if(_receiveD1 & 0x01)
		{
			_voice.SpeakInfo(L"出卡机的回收仓卡片已装满!");
			m_log.WriteErrorLog("商户："+m_merchantID_sending+" 合同："+m_agreementID_sending+" 发卡过程中出卡机的回收仓卡片已装满!");
		}
		else if(_receiveD3 & 0x08)
		{
			_voice.SpeakInfo(L"出卡机无卡!");
			m_log.WriteErrorLog("商户："+m_merchantID_sending+" 合同："+m_agreementID_sending+" 发卡过程中出卡机无卡!");
		}
		else if((_receiveD3 & 0x01) & ((_receiveD3 & 0x02)>>1))
		{
			//卡到写卡区域
			int _writeRate = 0;
			if(_receiveD2 & 0x01)
				_voice.SpeakInfo(L"出卡机卡不足!");
			CString _carCardID = _operate.getNextCarCard(9, 9);
			WRITE:
			if(!m_writeInfo.Write(_carCardID))
			{
				_writeRate++;
				if(_writeRate == m_reWriteRate)
				{
					SetTimer(hwnd, SENDCARCARD_TO_RECOVER_IDEVENT,SEND_TO_RECOVER_RATE,Send_from_writearea_to_recover);
					_writeRate = 0;
				}
				else
					goto WRITE;
			}
			else
			{
				SetTimer(hwnd, SENDCARCARD_FROM_WRITEAREA_TO_OUT_IDEVENT,SEND_FROM_WRITEAREA_TO_OUT_RATE,Send_from_writearea_to_out);
			}
		}
		else
			SetTimer(hwnd, INQUIREWORKSTATS_IDEVENT, INQUIREWORKSTATS_RATE, InquireWorkStat);
	}
}

//void CALLBACK CallBackCarCard_Inquire(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)
//{
//	//查询工作状态
//	CString _strInquireCommand = CALLBACKCARCARD_INQUIREWORKSTATS;
//	_strInquireCommand=CSendCarCardDlg::ChangeCharstr2Hexstr(_strInquireCommand);
//	m_CallBackSerialPort.WriteToPort(_strInquireCommand.GetBuffer(_strInquireCommand.GetLength()), _strInquireCommand.GetLength());
//	KillTimer(hwnd, CALLBACKCARCARD_INQUIRE_IDEVENT);
//	CVoice _voice;
//	_voice.Initial();
//	if(m_CallCarCardstrReceive == "")   //没有接受返回值
//		SetTimer(hwnd, CALLBACKCARCARD_INQUIRE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Inquire);
//	else if(m_CallCarCardstrReceive == "c7" || m_CallCarCardstrReceive == "C7")
//	{
//		_voice.SpeakInfo(L"卡在收卡时超时或阻塞!");
//		m_log.WriteErrorLog("卡在收卡时超时或阻塞!");
//	}
//	else if(m_CallCarCardstrReceive == "c8" || m_CallCarCardstrReceive == "C8")
//	{
//		_voice.SpeakInfo(L"卡在退卡时超时或阻塞!");
//		m_log.WriteErrorLog("卡在退卡时超时或阻塞!");
//	}
//	else if(m_CallCarCardstrReceive == "f0" || m_CallCarCardstrReceive == "F0")
//		SetTimer(hwnd, CALLBACKCARCARD_INQUIRE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Inquire);
//	else if(m_CallCarCardstrReceive == "c1" || m_CallCarCardstrReceive == "C1")
//	{
//		CString _temp = m_readInfo.Read();
//		_temp.Replace(" ", "");
//		if(_temp.IsEmpty())
//		{
//			//退卡
//			CVoice _voice;
//			_voice.Initial();
//			_voice.SpeakInfo(L"读取车卡信息失败!");
//			m_log.WriteErrorLog("读取车卡信息失败!");
//			SetTimer(hwnd, CALLBACKCARCARD_REFUSE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Refuse);
//		}
//		else if(_operate.MerchantIsExist(_temp.Left(9)) && _operate.AgreementIsExist(_temp.Left(18)))
//		{
//			//收卡
//			m_log.WriteOperateLog("回收 "+_temp+" 车卡成功！");
//			_operate.AddRetureCarCard();
//			SetTimer(hwnd, CALLBACKCARCARD_CALLBACK_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_CallBack);
//		}
//		else
//		{
//			//退卡
//			CVoice _voice;
//			_voice.Initial();
//			_voice.SpeakInfo(L"该车卡信息不正确!");
//			m_log.WriteErrorLog(_temp+": 该卡不属于本公司!");
//			SetTimer(hwnd, CALLBACKCARCARD_REFUSE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Refuse);
//		}
//	}
//	else
//		SetTimer(hwnd, CALLBACKCARCARD_INQUIRE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Inquire);
//}
//
//void CALLBACK CallBackCarCard_CallBack(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)
//{
//	CString _strCallBackCommand = CALLBACKCARCARD_CALLBACK;
//	_strCallBackCommand=CSendCarCardDlg::ChangeCharstr2Hexstr(_strCallBackCommand);
//	m_CallBackSerialPort.WriteToPort(_strCallBackCommand.GetBuffer(_strCallBackCommand.GetLength()), _strCallBackCommand.GetLength());
//	KillTimer(hwnd, CALLBACKCARCARD_CALLBACK_IDEVENT);
//	SetTimer(hwnd, CALLBACKCARCARD_INQUIRE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Inquire);
//}
//void CALLBACK CallBackCarCard_Refuse(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)
//{
//	CString _strCallBackCommand = CALLBACKCARCARD_REFUSE;
//	_strCallBackCommand=CSendCarCardDlg::ChangeCharstr2Hexstr(_strCallBackCommand);
//	m_CallBackSerialPort.WriteToPort(_strCallBackCommand.GetBuffer(_strCallBackCommand.GetLength()), _strCallBackCommand.GetLength());
//	KillTimer(hwnd, CALLBACKCARCARD_REFUSE_IDEVENT);
//	m_speakInfo = LPCWSTR(L"该卡不属于本公司!");
//	hThread = CreateThread(NULL,0,SpeakInfo,NULL,0,&m_threadID);
//	CloseHandle(hThread);
//	SetTimer(hwnd, CALLBACKCARCARD_INQUIRE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Inquire);
//}

void CSendCarCardDlg::OnBnClickedStopButton()
{
	KillTimer(SENDCARCARD_FROM_WRITEAREA_TO_OUT_IDEVENT);
	KillTimer(INQUIREWORKSTATS_IDEVENT);
	KillTimer(SENDCARCARD_TO_WRITEAREA_IDEVENT);
	KillTimer(SENDCARCARD_TO_RECOVER_IDEVENT);
	GetDlgItem(IDC_MERCHANT_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_MERCHANT_EDIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_AGREEMENT_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_AGREEMENT_COMBO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STOP_BUTTON)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SEND_BUTTON)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_AGREEMENT_COMBO)->EnableWindow(TRUE);

	GetDlgItem(IDC_START_BUTTON)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_INSTRUCTION_EDIT)->ShowWindow(SW_SHOW);

	m_agreementID.ResetContent();
	m_merchantID = L"";

	UpdateData(FALSE);
}


void CSendCarCardDlg::OnBnClickedStartButton()
{
	CString _temp = m_writeInfo.Read();
	_temp.Replace(" ", "");
	_temp = _temp.Left(9);   //读取商户信息
	if(_temp.IsEmpty())
	{
		//退卡
		m_speakInfo = LPCWSTR(L"读取商户卡信息失败!");
		m_log.WriteErrorLog("读取商户卡信息失败!");
		hThread = CreateThread(NULL,0,SpeakInfo,NULL,0,&m_threadID);
		CloseHandle(hThread);
	}
	else
	{
		m_merchantID = _temp;
		_operate.getUnFinishAgreementByMerchantID(m_merchantID);
		int _size = _operate.m_agreementIDArray.GetSize();
		if(_size == 0)
		{
			MessageBox("该商户没有未领取车卡的合同！");
			m_log.WriteOperateLog("该商户没有未领取车卡的合同！");
		}
		else
		{
			for(int i = 0; i < _size; i++)
				m_agreementID.AddString(_operate.m_agreementIDArray.GetAt(i));
			m_agreementID.SetCurSel(0);
		}

		GetDlgItem(IDC_MERCHANT_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MERCHANT_EDIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_AGREEMENT_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_AGREEMENT_COMBO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STOP_BUTTON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SEND_BUTTON)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_START_BUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_INSTRUCTION_EDIT)->ShowWindow(SW_HIDE);
	}

	UpdateData(FALSE);
}

void CSendCarCardDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == 0)
	{
		GetClientRect(&m_ClientRect);  
		//CClientDC dc(this); // device context for painting   
  //      // 设置m_newFont对象的字体为当前字体，并将之前的字体指针保存到m_pOldFont   
		//GetClientRect(&m_ClientRect);    
		//m_pOldFont = (CFont*)dc.SelectObject(&m_newFont);   
		//LOGFONT logFont;   
		//// 获取m_newFont字体的LOGFONT结构   
		//m_newFont.GetLogFont(&logFont);
  //      // 擦除上次的字体   
		//CBrush Brush(RGB(120,120,120));
		//dc.FillRect(CRect(m_nTextX_Up, 10, m_nTextX_Up+9*42, 10+60), &Brush);
		//int _height = m_ClientRect.Height()-abs(logFont.lfHeight)-10;
  //      dc.FillRect(CRect(m_nTextX_Down, _height, m_nTextX_Down+10*42, _height+60), &Brush);
 
		m_nTextX_Up -= 5;      
		if (m_nTextX_Up < 10)   
			m_nTextX_Up = m_ClientRect.Width() - 300;   
 
		m_nTextX_Down += 5;      
		if (m_nTextX_Down > m_ClientRect.Width() - 300)   
			m_nTextX_Down = 10;   

		//Invalidate();
		LOGFONT logFont;   
		// 获取m_newFont字体的LOGFONT结构   
		m_newFont.GetLogFont(&logFont);
		InvalidateRect(CRect(0, 0, m_ClientRect.Width(), abs(logFont.lfHeight)+20), TRUE);
		InvalidateRect(CRect(0, m_ClientRect.Height()-abs(logFont.lfHeight)-20, m_ClientRect.Width(), m_ClientRect.Height()), TRUE);
	}

	CDialogEx::OnTimer(nIDEvent);
}

static DWORD WINAPI SpeakInfo(void *pArg)
{
	CVoice _voice;
	_voice.Initial();
	_voice.SpeakInfo(m_speakInfo);

	ExitThread(0);
}

void CSendCarCardDlg::OnSize(UINT nType, int cx, int cy)
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

void CSendCarCardDlg::ReSize()
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
		if(woc == IDC_AGREEMENT_COMBO)
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

HBRUSH CSendCarCardDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	/*switch(pWnd->GetDlgCtrlID())   
	{   
	case IDC_MERCHANT_STATIC: 
	case IDC_AGREEMENT_STATIC:  
		pDC->SetBkMode(TRANSPARENT);   
		pDC->SetTextColor(RGB(0,0,120));  
		break;
	case IDC_INSTRUCTION_EDIT:
		pDC->SetBkMode(TRANSPARENT);   
		pDC->SetTextColor(RGB(120,0,120));  
	default:   
		break;   
	}  */
	return (HBRUSH)GetStockObject(HOLLOW_BRUSH);   
}
