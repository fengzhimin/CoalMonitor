
// SendCarCardDlg.cpp : ʵ���ļ�
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


#define SHOW_COMPANY_INFO_IDEVENT                 0      //��ʾ��˾��Ϣ���¼�ID

#define SENDCARCARD_TO_WRITEAREA  "02 44 48 03 0D 05"   //������д������
#define SENDCARCARD_FROM_WRITEAREA_TO_OUT "02 43 46 03 04 05"  //������д�������͵��տ�����
#define SENDCARCARD_TO_RECOVER    "02 43 50 03 12 05"   //���տ�
#define INQUIREWORKSTATS   "02 52 46 03 15 05"   //��ѯ����״̬

#define SENDCARCARD_TO_WRITEAREA_IDEVENT           1    //������д�������¼�ID
#define INQUIREWORKSTATS_IDEVENT                   2    //��ѯ����״̬�¼�ID
#define SENDCARCARD_FROM_WRITEAREA_TO_OUT_IDEVENT  3    //������д�������͵��տ������¼�ID
#define SENDCARCARD_TO_RECOVER_IDEVENT             4    //���տ��¼�ID

#define SEND_TO_WRITEAREA_RATE                     2000   //���Ϳ���д�������Ƶ��(ms)
#define INQUIREWORKSTATS_RATE                      100   //��ѯ����״̬Ƶ��(ms)
#define SEND_FROM_WRITEAREA_TO_OUT_RATE            200   //��ȫ������Ƶ��(ms)
#define SEND_TO_RECOVER_RATE                       100   //���ճ�����Ƶ��(ms)

#define CALLBACKCARCARD_CALLBACK_IDEVENT           5    //�տ�������״̬�¼�ID
#define CALLBACKCARCARD_REFUSE_IDEVENT             6    //�տ����˿�״̬�¼�ID
#define CALLBACKCARCARD_INQUIRE_IDEVENT            7    //�տ�����ѯ����״̬�¼�ID

#define CALLBACKCARCARD_INQUIREWORKSTATS         "7f 40 01 07 0d 12"   //�տ�����ѯ����
#define CALLBACKCARCARD_CALLBACK                 "7f 40 01 20 0d c0"   //�տ����տ�����
#define CALLBACKCARCARD_REFUSE                   "7f 40 01 21 8d c5"   //�տ����˿����� 

#define CALLBACKCARCARD_INQUIRE_RATE                100  //�տ�����ѯ����״̬Ƶ��(ms)

CSerialPort m_SendCardSerialPort;   //�������Ĵ���
//CSerialPort m_CallBackSerialPort;   //�տ����Ĵ���
CString m_SendCarCardstrReceive;    //���������յ��ַ�
//CString m_CallCarCardstrReceive;    //�տ������յ��ַ�
CString m_merchantID_sending;       //���ڷ��ͳ������̻�ID
CString m_agreementID_sending;      //���ڷ��ͳ����ĺ�ͬID
CRFIDDevice m_writeInfo;//, m_readInfo;
OperateDataBase _operate;
int m_reReadRate;       //�ظ�����Ƶ��
int m_reWriteRate;      //�ظ�д��Ƶ��
DWORD  m_threadID;
HANDLE hThread;
Log m_log;
LPCWSTR m_speakInfo;
static DWORD WINAPI SpeakInfo(void *pArg);

//��ʱ��������������������д��������ȫ����
void CALLBACK Send_from_writearea_to_out(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime);
//��ʱ���������������������͵�д������
void CALLBACK Send_to_writearea(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime);
//��ʱ��������������������д�������͵���������
void CALLBACK Send_from_writearea_to_recover(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime);
//��ʱ���������������Ͳ�ѯָ��
void CALLBACK InquireWorkStat(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime);

//��ʱ�������������տ�����ѯ����
//void CALLBACK CallBackCarCard_Inquire(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime);
//��ʱ�������������տ������ճ���
//void CALLBACK CallBackCarCard_CallBack(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime);
//��ʱ�������������տ����˳�����
//void CALLBACK CallBackCarCard_Refuse(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime);

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


// CSendCarCardDlg �Ի���



CSendCarCardDlg::CSendCarCardDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSendCarCardDlg::IDD, pParent)
	, m_merchantID(_T(""))
{
	m_receiveSendCarCardCommand = false;
	//m_receiveCallBackCarCardCommand = false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_font_static.CreatePointFont(300, "����");
	m_font_edit.CreatePointFont(200, "����");
	m_font_button.CreatePointFont(400, "����");
	m_log.WriteOperateLog("��¼ϵͳ��");
}

CSendCarCardDlg::~CSendCarCardDlg()
{
	m_font_static.DeleteObject();
	m_font_edit.DeleteObject();
	m_font_button.DeleteObject();
	m_log.WriteOperateLog("ע��ϵͳ��");
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


// CSendCarCardDlg ��Ϣ�������

BOOL CSendCarCardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	HICON m_hIcon= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
   
	GetClientRect(&m_ClientRect);     //ȡ�ͻ�����С  
	old.x=m_ClientRect.right - m_ClientRect.left;
	old.y=m_ClientRect.bottom - m_ClientRect.top;
	ShowWindow(SW_MAXIMIZE);

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

    // ���ö�ʱ������ʱʱ��Ϊ100ms   
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
	GetDlgItem(IDC_INSTRUCTION_EDIT)->SetWindowTextA("����ָ�ϣ�\r\n��һ�����Ƚ��̻��������������!\r\n�ڶ��������ӡ������ť!\r\n�������ȴ���ȡ�̻�����Ϣ��\r\n���ģ�ѡ��Ҫ��ӡ�ĺ�ͬ��Ϣ��\r\n���壺�����ӡ����ʼ��ӡ������\r\n�������ȴ���ӡ��ϳ���������˳��� \r\n");

	CFileFind finder;  
	BOOL ifFind = finder.FindFile(_T("./Config.ini"));  
	if( ifFind )  
	{  
		CString _temp;
		//�������Ĵ���
		::GetPrivateProfileStringA("SendCardSerial Config Info","Com","",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");  
		_temp.ReleaseBuffer();
		_temp.Trim();
		m_SendCardComm = _temp.GetAt(3) - '0';
		::GetPrivateProfileStringA("SendCardSerial Config Info","BaudRate","",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");  
		_temp.ReleaseBuffer();
		_temp.Trim();
		m_SendCardBaudRate = _ttoi(_temp);
		//��������RFID�豸����
		::GetPrivateProfileStringA("SendCarCardRFID Config Info","Com","",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");  
		_temp.ReleaseBuffer();
		_temp.Trim();
		m_SendCardRFIDComm = _temp.GetAt(3) - '0';
		::GetPrivateProfileStringA("SendCarCardRFID Config Info","BaudRate","",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");  
		_temp.ReleaseBuffer();
		_temp.Trim();
		m_SendCardRFIDBaudRate = _ttoi(_temp);
		////�տ����Ĵ���
		//::GetPrivateProfileStringA("CallBackCardSerial Config Info","Com","",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");  
		//_temp.ReleaseBuffer();
		//_temp.Trim();
		//m_CallBackCardComm = _temp.GetAt(3) - '0';
		//::GetPrivateProfileStringA("CallBackCardSerial Config Info","BaudRate", "",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");     
		//_temp.ReleaseBuffer();
		//_temp.Trim();
		//m_CallBackCardBaudRate = _ttoi(_temp);
		////�տ�����RFID�豸����
		//::GetPrivateProfileStringA("CallBackCarCardRFID Config Info","Com","",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");  
		//_temp.ReleaseBuffer();
		//_temp.Trim();
		//m_CallBackCardRFIDComm = _temp.GetAt(3) - '0';
		//::GetPrivateProfileStringA("CallBackCarCardRFID Config Info","BaudRate", "",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");     
		//_temp.ReleaseBuffer();
		//_temp.Trim();
		//m_CallBackCardRFIDBaudRate = _ttoi(_temp);
		//�������ظ�д���Ĵ���
		::GetPrivateProfileStringA("WriteAgainRate Config Info","Rate", "5",_temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");     
		_temp.ReleaseBuffer();
		_temp.Trim();
		m_reWriteRate = _ttoi(_temp);
		::GetPrivateProfileStringA("Dialog Skin Config Info", "Skin", "skin/adamant.she", m_skin.GetBuffer(MAX_PATH), MAX_PATH, "./Config.ini");  
		m_skin.ReleaseBuffer();
	}
	else
	{
		m_log.WriteErrorLog("��ȡ�����ļ�����!");
		m_SendCardComm = m_CallBackCardComm = 0;
		m_SendCardBaudRate = m_CallBackCardBaudRate = 9600;
		m_skin = "skin/adamant.she";
	}
	
	SkinH_AttachEx(m_skin, NULL); //������

	if(!m_writeInfo.Initial(m_SendCardRFIDComm, m_SendCardRFIDBaudRate))
	{
		m_log.WriteErrorLog("RFIDд���豸��ʼ��ʧ�ܣ�");
		MessageBox("RFIDд���豸��ʼ��ʧ��");
	}
	
	/*if(!m_readInfo.Initial(m_CallBackCardRFIDComm, m_CallBackCardRFIDBaudRate))
	{
		m_log.WriteErrorLog("RFID�����豸��ʼ��ʧ�ܣ�");
		MessageBox("RFID�����豸��ʼ��ʧ��");
	}*/
	//�����������Ĵ���
	if(m_SendCardSerialPort.InitPort(this,m_SendCardComm,m_SendCardBaudRate,'N',8,1,EV_RXCHAR|EV_CTS,1024))		
		m_SendCardSerialPort.StartMonitoring();
	else
	{
		m_log.WriteErrorLog("���������ڴ�ʧ�ܣ�");
		AfxMessageBox("���������ڴ�ʧ�ܣ�");
	}

	//�����տ����Ĵ���
	/*if(m_CallBackSerialPort.InitPort(this,m_CallBackCardComm,m_CallBackCardBaudRate,'N',8,1,EV_RXCHAR|EV_CTS,1024))		
		m_CallBackSerialPort.StartMonitoring();
	else 
	{
		m_log.WriteErrorLog("�տ������ڴ�ʧ�ܣ�");
		AfxMessageBox("�տ������ڴ�ʧ�ܣ�");
	}*/

	//�����տ�����ѯ״̬
	//SetTimer(CALLBACKCARCARD_INQUIRE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Inquire);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSendCarCardDlg::OnPaint()
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
		/*CRect   rect;    
		CPaintDC   dc(this);    
		GetClientRect(rect);    
		dc.FillSolidRect(rect,RGB(120,120,120));*/     //����Ϊ��ɫ����  
		GetClientRect(&m_ClientRect);  
		LOGFONT logFont;   
		// ��ȡm_newFont�����LOGFONT�ṹ   
		m_newFont.GetLogFont(&logFont);
		CPaintDC   dc(this);   
		m_pOldFont = (CFont*)dc.SelectObject(&m_newFont); 
		dc.SetBkMode(TRANSPARENT); //���ñ���Ϊ͸����  
		 // �����ı���ɫΪ��ɫ 
        dc.SetTextColor(RGB(255,0,0));   
        // ��ָ��λ������ı�   
        dc.TextOut(m_nTextX_Up,10,_T("��ӭ�����Զ�ȡ����"));   
        // �����ı���ɫΪ��ɫ   
        dc.SetTextColor(RGB(0,255,0));       
        // ��ָ��λ������ı�   
        dc.TextOut(m_nTextX_Down,m_ClientRect.Height()-abs(logFont.lfHeight)-10,_T("лл��ע�²��ؿƼ���˾"));   

		dc.SelectObject(m_pOldFont);  

		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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

//���º�ͬ�����б�
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
			m_receiveSendCarCardCommand = false;  //ֹͣ����
			m_SendCarCardstrReceive = m_SendCarCardstrReceive.Right(6);  //��ȡFlag Code
		}
		if(m_receiveSendCarCardCommand)
			m_SendCarCardstrReceive += _receiveStr;

		if(_receiveStr == "02")
		{
			m_SendCarCardstrReceive = "";
			m_receiveSendCarCardCommand = true;  //��ʼ����
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
		MessageBox("�̻���ID�����ڣ�");
		m_log.WriteErrorLog("�̻���ID�����ڣ�");
		return ;
	}
	else if(!_operate.AgreementIsExist(m_strAgreementID))
	{
		MessageBox("��ͬ��ID�����ڣ�");
		m_log.WriteErrorLog("��ͬ��ID�����ڣ�");
		return ;
	}
	else
		_operate.getCarCardInfo();
	//��ʼ��ӡ������ʱ���ֹ��ͬIDѡ��
	m_agreementID.EnableWindow(FALSE); 
	SetTimer(SENDCARCARD_TO_WRITEAREA_IDEVENT, SEND_TO_WRITEAREA_RATE, Send_to_writearea);
}

void CALLBACK Send_from_writearea_to_out(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)
{
	CString _strSendCommand = SENDCARCARD_FROM_WRITEAREA_TO_OUT;
	_strSendCommand=CSendCarCardDlg::ChangeCharstr2Hexstr(_strSendCommand);
	m_SendCardSerialPort.WriteToPort(_strSendCommand.GetBuffer(_strSendCommand.GetLength()), _strSendCommand.GetLength());
	KillTimer(hwnd, SENDCARCARD_FROM_WRITEAREA_TO_OUT_IDEVENT);
	_operate.AddUseCarCard();   //�ɹ����ͳ���
	if(_operate.m_carCardCount == _operate.m_useCarCardCount)
	{
		m_speakInfo = LPCWSTR(L"���ͳ�����ϣ�");
		m_log.WriteOperateLog("�̻���"+m_merchantID_sending+" ��ͬ��"+m_agreementID_sending+" �������");
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
	//��ѯ����״̬
	CString _strSendCommand = INQUIREWORKSTATS;
	_strSendCommand=CSendCarCardDlg::ChangeCharstr2Hexstr(_strSendCommand);
	m_SendCardSerialPort.WriteToPort(_strSendCommand.GetBuffer(_strSendCommand.GetLength()), _strSendCommand.GetLength());
	KillTimer(hwnd, INQUIREWORKSTATS_IDEVENT);
	if(m_SendCarCardstrReceive == "")   //û�н��ܷ���ֵ
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
			_voice.SpeakInfo(L"������������������!");
			m_log.WriteErrorLog("�̻���"+m_merchantID_sending+" ��ͬ��"+m_agreementID_sending+" ���������г�����������������!");
		}
		else if(_receiveD2 & 0x02)
		{
			_voice.SpeakInfo(L"�������Ϳ�ͨ���¿�!");
			m_log.WriteErrorLog("�̻���"+m_merchantID_sending+" ��ͬ��"+m_agreementID_sending+" ���������г������Ϳ�ͨ���¿�!");
		}
		else if(_receiveD1 & 0x01)
		{
			_voice.SpeakInfo(L"�������Ļ��ղֿ�Ƭ��װ��!");
			m_log.WriteErrorLog("�̻���"+m_merchantID_sending+" ��ͬ��"+m_agreementID_sending+" ���������г������Ļ��ղֿ�Ƭ��װ��!");
		}
		else if(_receiveD3 & 0x08)
		{
			_voice.SpeakInfo(L"�������޿�!");
			m_log.WriteErrorLog("�̻���"+m_merchantID_sending+" ��ͬ��"+m_agreementID_sending+" ���������г������޿�!");
		}
		else if((_receiveD3 & 0x01) & ((_receiveD3 & 0x02)>>1))
		{
			//����д������
			int _writeRate = 0;
			if(_receiveD2 & 0x01)
				_voice.SpeakInfo(L"������������!");
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
//	//��ѯ����״̬
//	CString _strInquireCommand = CALLBACKCARCARD_INQUIREWORKSTATS;
//	_strInquireCommand=CSendCarCardDlg::ChangeCharstr2Hexstr(_strInquireCommand);
//	m_CallBackSerialPort.WriteToPort(_strInquireCommand.GetBuffer(_strInquireCommand.GetLength()), _strInquireCommand.GetLength());
//	KillTimer(hwnd, CALLBACKCARCARD_INQUIRE_IDEVENT);
//	CVoice _voice;
//	_voice.Initial();
//	if(m_CallCarCardstrReceive == "")   //û�н��ܷ���ֵ
//		SetTimer(hwnd, CALLBACKCARCARD_INQUIRE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Inquire);
//	else if(m_CallCarCardstrReceive == "c7" || m_CallCarCardstrReceive == "C7")
//	{
//		_voice.SpeakInfo(L"�����տ�ʱ��ʱ������!");
//		m_log.WriteErrorLog("�����տ�ʱ��ʱ������!");
//	}
//	else if(m_CallCarCardstrReceive == "c8" || m_CallCarCardstrReceive == "C8")
//	{
//		_voice.SpeakInfo(L"�����˿�ʱ��ʱ������!");
//		m_log.WriteErrorLog("�����˿�ʱ��ʱ������!");
//	}
//	else if(m_CallCarCardstrReceive == "f0" || m_CallCarCardstrReceive == "F0")
//		SetTimer(hwnd, CALLBACKCARCARD_INQUIRE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Inquire);
//	else if(m_CallCarCardstrReceive == "c1" || m_CallCarCardstrReceive == "C1")
//	{
//		CString _temp = m_readInfo.Read();
//		_temp.Replace(" ", "");
//		if(_temp.IsEmpty())
//		{
//			//�˿�
//			CVoice _voice;
//			_voice.Initial();
//			_voice.SpeakInfo(L"��ȡ������Ϣʧ��!");
//			m_log.WriteErrorLog("��ȡ������Ϣʧ��!");
//			SetTimer(hwnd, CALLBACKCARCARD_REFUSE_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_Refuse);
//		}
//		else if(_operate.MerchantIsExist(_temp.Left(9)) && _operate.AgreementIsExist(_temp.Left(18)))
//		{
//			//�տ�
//			m_log.WriteOperateLog("���� "+_temp+" �����ɹ���");
//			_operate.AddRetureCarCard();
//			SetTimer(hwnd, CALLBACKCARCARD_CALLBACK_IDEVENT, CALLBACKCARCARD_INQUIRE_RATE, CallBackCarCard_CallBack);
//		}
//		else
//		{
//			//�˿�
//			CVoice _voice;
//			_voice.Initial();
//			_voice.SpeakInfo(L"�ó�����Ϣ����ȷ!");
//			m_log.WriteErrorLog(_temp+": �ÿ������ڱ���˾!");
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
//	m_speakInfo = LPCWSTR(L"�ÿ������ڱ���˾!");
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
	_temp = _temp.Left(9);   //��ȡ�̻���Ϣ
	if(_temp.IsEmpty())
	{
		//�˿�
		m_speakInfo = LPCWSTR(L"��ȡ�̻�����Ϣʧ��!");
		m_log.WriteErrorLog("��ȡ�̻�����Ϣʧ��!");
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
			MessageBox("���̻�û��δ��ȡ�����ĺ�ͬ��");
			m_log.WriteOperateLog("���̻�û��δ��ȡ�����ĺ�ͬ��");
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent == 0)
	{
		GetClientRect(&m_ClientRect);  
		//CClientDC dc(this); // device context for painting   
  //      // ����m_newFont���������Ϊ��ǰ���壬����֮ǰ������ָ�뱣�浽m_pOldFont   
		//GetClientRect(&m_ClientRect);    
		//m_pOldFont = (CFont*)dc.SelectObject(&m_newFont);   
		//LOGFONT logFont;   
		//// ��ȡm_newFont�����LOGFONT�ṹ   
		//m_newFont.GetLogFont(&logFont);
  //      // �����ϴε�����   
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
		// ��ȡm_newFont�����LOGFONT�ṹ   
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
		GetClientRect(&rect);     //ȡ�ͻ�����С  
		old.x=rect.right-rect.left;
		old.y=rect.bottom-rect.top;
    }  
}

void CSendCarCardDlg::ReSize()
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
