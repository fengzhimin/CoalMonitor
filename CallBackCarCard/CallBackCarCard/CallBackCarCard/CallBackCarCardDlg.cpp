
// CallBackCarCardDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CallBackCarCard.h"
#include "CallBackCarCardDlg.h"
#include "afxdialogex.h"
#include "Voice.h"
#include "RFIDDevice.h"
#include "OperateDataBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CALLBACKCARCARD_CALLBACK_IDEVENT           5    //�տ�������״̬�¼�ID
#define CALLBACKCARCARD_REFUSE_IDEVENT             6    //�տ����˿�״̬�¼�ID
#define CALLBACKCARCARD_INQUIRE_IDEVENT            7    //�տ�����ѯ����״̬�¼�ID

#define CALLBACKCARCARD_INQUIREWORKSTATS         "7f 40 01 07 0d 12"   //�տ�����ѯ����
#define CALLBACKCARCARD_CALLBACK                 "7f 40 01 20 0d c0"   //�տ����տ�����
#define CALLBACKCARCARD_REFUSE                   "7f 40 01 21 8d c5"   //�տ����˿����� 

#define CALLBACKCARCARD_INQUIRE_RATE                100  //�տ�����ѯ����״̬Ƶ��(ms)


CSerialPort m_CallBackSerialPort;   //�տ����Ĵ���
CString m_CallCarCardstrReceive;    //�տ������յ��ַ�
CRFIDDevice m_readInfo;
OperateDataBase _operate;
int m_reReadRate;       //�ظ�����Ƶ��
DWORD  m_threadID;
HANDLE hThread;
Log m_log;
LPCWSTR m_speakInfo;
static DWORD WINAPI SpeakInfo(void *pArg);

//��ʱ�������������տ�����ѯ����
void CALLBACK CallBackCarCard_Inquire(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime);
//��ʱ�������������տ������ճ���
void CALLBACK CallBackCarCard_CallBack(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime);
//��ʱ�������������տ����˳�����
void CALLBACK CallBackCarCard_Refuse(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime);

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


// CCallBackCarCardDlg �Ի���



CCallBackCarCardDlg::CCallBackCarCardDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCallBackCarCardDlg::IDD, pParent)
{
	m_receiveCallBackCarCardCommand = false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_log.WriteOperateLog("��¼ϵͳ��");
}

CCallBackCarCardDlg::~CCallBackCarCardDlg()
{
	m_log.WriteOperateLog("ע��ϵͳ��");
}

void CCallBackCarCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCallBackCarCardDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_COMM_RXCHAR,OnReceiveChar)
	ON_BN_CLICKED(IDC_START_BUTTON, &CCallBackCarCardDlg::OnBnClickedStartButton)
	ON_BN_CLICKED(IDC_STOP_BUTTON, &CCallBackCarCardDlg::OnBnClickedStopButton)
END_MESSAGE_MAP()


// CCallBackCarCardDlg ��Ϣ�������

BOOL CCallBackCarCardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(FALSE);
	CFileFind finder;  
	BOOL ifFind = finder.FindFile(_T("./Config.ini"));  
	if( ifFind )  
	{  
		CString _temp;
		//�տ����Ĵ���
		::GetPrivateProfileStringA("CallBackCardSerial Config Info","Com","",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");  
		_temp.ReleaseBuffer();
		_temp.Trim();
		m_CallBackCardComm = _temp.GetAt(3) - '0';
		::GetPrivateProfileStringA("CallBackCardSerial Config Info","BaudRate", "",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");     
		_temp.ReleaseBuffer();
		_temp.Trim();
		m_CallBackCardBaudRate = _ttoi(_temp);
		//�տ�����RFID�豸����
		::GetPrivateProfileStringA("CallBackCarCardRFID Config Info","Com","",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");  
		_temp.ReleaseBuffer();
		_temp.Trim();
		m_CallBackCardRFIDComm = _temp.GetAt(3) - '0';
		::GetPrivateProfileStringA("CallBackCarCardRFID Config Info","BaudRate", "",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");     
		_temp.ReleaseBuffer();
		_temp.Trim();
		m_CallBackCardRFIDBaudRate = _ttoi(_temp);
		::GetPrivateProfileStringA("Dialog Skin Config Info", "Skin", "skin/adamant.she", m_skin.GetBuffer(MAX_PATH), MAX_PATH, "./Config.ini");  
		m_skin.ReleaseBuffer();
	}
	else
	{
		m_log.WriteErrorLog("��ȡ�����ļ�ʧ�ܣ�");
		m_skin = "skin/adamant.she";
	}
	
	SkinH_AttachEx(m_skin, NULL); //������

	if(!m_readInfo.Initial(m_CallBackCardRFIDComm, m_CallBackCardRFIDBaudRate))
	{
		m_log.WriteErrorLog("RFID�����豸��ʼ��ʧ�ܣ�");
		MessageBox("RFID�����豸��ʼ��ʧ��");
	}

	//�����տ����Ĵ���
	if(m_CallBackSerialPort.InitPort(this,m_CallBackCardComm,m_CallBackCardBaudRate,'N',8,1,EV_RXCHAR|EV_CTS,1024))		
		m_CallBackSerialPort.StartMonitoring();
	else 
	{
		m_log.WriteErrorLog("�տ������ڴ�ʧ�ܣ�");
		AfxMessageBox("�տ������ڴ�ʧ�ܣ�");
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCallBackCarCardDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCallBackCarCardDlg::OnPaint()
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
HCURSOR CCallBackCarCardDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CCallBackCarCardDlg::DevideHexChar(char HexChar)
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

BOOL m_callBackReceiveOne = false, m_callBackReceiveTwo = false;
LRESULT CCallBackCarCardDlg::OnReceiveChar(UINT ch, LONG port)
{
	if(port == m_CallBackCardComm)
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
	}
	
	return 0;
}

static DWORD WINAPI SpeakInfo(void *pArg)
{
	CVoice _voice;
	_voice.Initial();
	_voice.SpeakInfo(m_speakInfo);

	ExitThread(0);
}

char CCallBackCarCardDlg::CombineHexChar(char CharH,char CharL)
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

void CCallBackCarCardDlg::HexStringFilter(CString &str)
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

CString CCallBackCarCardDlg::ChangeCharstr2Hexstr(CString Charstr)
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

void CALLBACK CallBackCarCard_Inquire(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)
{
	//��ѯ����״̬
	CString _strInquireCommand = CALLBACKCARCARD_INQUIREWORKSTATS;
	_strInquireCommand=CCallBackCarCardDlg::ChangeCharstr2Hexstr(_strInquireCommand);
	m_CallBackSerialPort.WriteToPort(_strInquireCommand.GetBuffer(_strInquireCommand.GetLength()), _strInquireCommand.GetLength());
	KillTimer(hwnd, CALLBACKCARCARD_INQUIRE_IDEVENT);
	CVoice _voice;
	_voice.Initial();
	if(m_CallCarCardstrReceive == "")   //û�н��ܷ���ֵ
		SetTimer(hwnd, CALLBACKCARCARD_INQUIRE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Inquire);
	else if(m_CallCarCardstrReceive == "c7" || m_CallCarCardstrReceive == "C7")
	{
		_voice.SpeakInfo(L"�����տ�ʱ��ʱ������!");
		m_log.WriteErrorLog("�����տ�ʱ��ʱ������!");
	}
	else if(m_CallCarCardstrReceive == "c8" || m_CallCarCardstrReceive == "C8")
	{
		_voice.SpeakInfo(L"�����˿�ʱ��ʱ������!");
		m_log.WriteErrorLog("�����˿�ʱ��ʱ������!");
	}
	else if(m_CallCarCardstrReceive == "f0" || m_CallCarCardstrReceive == "F0")
		SetTimer(hwnd, CALLBACKCARCARD_INQUIRE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Inquire);
	else if(m_CallCarCardstrReceive == "c1" || m_CallCarCardstrReceive == "C1")
	{
		CString _temp = m_readInfo.Read();
		_temp.Replace(" ", "");
		if(_temp.IsEmpty())
		{
			//�˿�
			CVoice _voice;
			_voice.Initial();
			_voice.SpeakInfo(L"��ȡ������Ϣʧ��!");
			m_log.WriteErrorLog("��ȡ������Ϣʧ��!");
			SetTimer(hwnd, CALLBACKCARCARD_REFUSE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Refuse);
		}
		else if(_operate.MerchantIsExist(_temp.Left(9)) && _operate.AgreementIsExist(_temp.Left(18)))
		{
			//�տ�
			m_log.WriteOperateLog("���� "+_temp+" �����ɹ���");
			_operate.AddRetureCarCard();
			SetTimer(hwnd, CALLBACKCARCARD_CALLBACK_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_CallBack);
		}
		else
		{
			//�˿�
			CVoice _voice;
			_voice.Initial();
			_voice.SpeakInfo(L"�ó�����Ϣ����ȷ!");
			m_log.WriteErrorLog(_temp+": �ÿ������ڱ���˾!");
			SetTimer(hwnd, CALLBACKCARCARD_REFUSE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Refuse);
		}
	}
	else
		SetTimer(hwnd, CALLBACKCARCARD_INQUIRE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Inquire);
}

void CALLBACK CallBackCarCard_CallBack(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)
{
	CString _strCallBackCommand = CALLBACKCARCARD_CALLBACK;
	_strCallBackCommand=CCallBackCarCardDlg::ChangeCharstr2Hexstr(_strCallBackCommand);
	m_CallBackSerialPort.WriteToPort(_strCallBackCommand.GetBuffer(_strCallBackCommand.GetLength()), _strCallBackCommand.GetLength());
	KillTimer(hwnd, CALLBACKCARCARD_CALLBACK_IDEVENT);
	SetTimer(hwnd, CALLBACKCARCARD_INQUIRE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Inquire);
}
void CALLBACK CallBackCarCard_Refuse(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)
{
	CString _strCallBackCommand = CALLBACKCARCARD_REFUSE;
	_strCallBackCommand=CCallBackCarCardDlg::ChangeCharstr2Hexstr(_strCallBackCommand);
	m_CallBackSerialPort.WriteToPort(_strCallBackCommand.GetBuffer(_strCallBackCommand.GetLength()), _strCallBackCommand.GetLength());
	KillTimer(hwnd, CALLBACKCARCARD_REFUSE_IDEVENT);
	m_speakInfo = LPCWSTR(L"�ÿ������ڱ���˾!");
	hThread = CreateThread(NULL,0,SpeakInfo,NULL,0,&m_threadID);
	CloseHandle(hThread);
	SetTimer(hwnd, CALLBACKCARCARD_INQUIRE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Inquire);
}


void CCallBackCarCardDlg::OnBnClickedStartButton()
{
	//�����տ�����ѯ״̬
	GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_START_BUTTON)->EnableWindow(FALSE);
	SetTimer(CALLBACKCARCARD_INQUIRE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Inquire);
}

void CCallBackCarCardDlg::OnBnClickedStopButton()
{
	GetDlgItem(IDC_START_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(FALSE);
	KillTimer(CALLBACKCARCARD_INQUIRE_IDEVENT);
	KillTimer(CALLBACKCARCARD_REFUSE_IDEVENT);
	KillTimer(CALLBACKCARCARD_CALLBACK_IDEVENT);
}
