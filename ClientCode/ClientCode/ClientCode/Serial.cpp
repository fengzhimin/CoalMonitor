// Serial.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientCode.h"
#include "Serial.h"
#include "afxdialogex.h"
#include "..//dll/disdll.h"
#pragma comment(lib, "..//dll/disdll.lib")

// CSerial �Ի���

IMPLEMENT_DYNAMIC(CSerial, CDialogEx)

CSerial* pReadCarCard = NULL;
static int tagCount = 0;
static int dataCount = 0;
static DWORD WINAPI  ReadCarCardInfo(void *pArg);

CSerial::CSerial(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSerial::IDD, pParent)
	, m_serial_string(_T(""))
	, m_baudrate_string(_T(""))
{
}

CSerial::~CSerial()
{
}

void CSerial::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERIAL_READCARCARD_COMBO, m_serial_control);
	DDX_CBString(pDX, IDC_SERIAL_READCARCARD_COMBO, m_serial_string);
	DDX_Control(pDX, IDC_BAUDRATE_READCARCARD_COMBO, m_baudrate_control);
	DDX_CBString(pDX, IDC_BAUDRATE_READCARCARD_COMBO, m_baudrate_string);
}

BEGIN_MESSAGE_MAP(CSerial, CDialogEx)
	ON_BN_CLICKED(IDC_CONNECT_SERIAL_BUTTON, &CSerial::OnBnClickedConnectSerialButton)
	ON_BN_CLICKED(IDC_RESET_SERIAL_BUTTON, &CSerial::OnBnClickedResetSerialButton)
	ON_BN_CLICKED(IDC_EXIT_SERIAL_BUTTON, &CSerial::OnBnClickedExitSerialButton)
END_MESSAGE_MAP()

BOOL CSerial::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetSerialPortList(m_serial_control);
	m_serial_control.SetCurSel(0);
	m_baudrate_control.AddString(_T("115200"));
	m_baudrate_control.AddString(_T("57600"));
	m_baudrate_control.AddString(_T("38400"));
	m_baudrate_control.AddString(_T("19200"));
	m_baudrate_control.AddString(_T("9600"));
	m_baudrate_control.SetCurSel(4);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
// CSerial ��Ϣ�������


void CSerial::OnBnClickedConnectSerialButton()
{
	UpdateData();
	
	if(m_serial_string == L"")
	{
		GetDlgItem(IDC_CONNECTINFO_SERIAL_STATIC)->SetWindowText(_T("��ѡ�񴮿ڣ�"));
		return ;
	}
	if(m_baudrate_string == L"")
	{
		GetDlgItem(IDC_CONNECTINFO_SERIAL_STATIC)->SetWindowText(_T("��ѡ�����ʣ�"));
		return ;
	}

	int serial = m_serial_string.GetAt(3) - '0';
	int baudrate = _ttoi(m_baudrate_string);
	if (!DeviceInit("", serial, baudrate))
	{
		GetDlgItem(IDC_CONNECTINFO_SERIAL_STATIC)->SetWindowText(_T("�豸��ʼ��ʧ�ܡ�"));
		return;
	}

	if (!DeviceConnect())
	{
		GetDlgItem(IDC_CONNECTINFO_SERIAL_STATIC)->SetWindowText(_T("�豸����ʧ�ܡ�"));
		return;
	}
	else
	{
		//�����д����
		AfxGetMainWnd()->GetMenu()->EnableMenuItem(ID_READCARCARD, MF_ENABLED); 
		AfxGetMainWnd()->GetMenu()->EnableMenuItem(ID_WRITECARCARD, MF_ENABLED); 
		GetDlgItem(IDC_CONNECTINFO_SERIAL_STATIC)->SetWindowText(_T("�豸���ӳɹ���"));
		
		pReadCarCard = this;
		m_carCardThreadParameter.m_dialog = this;
		m_carCardThreadParameter.m_carCardInfo = L"";
		m_carCardThreadParameter.m_isSuccess = false;

		//�����߳�			
		hThread = CreateThread(NULL,0,ReadCarCardInfo,&m_carCardThreadParameter,0,&m_readCarCardThreadID);  
	}
}


void CSerial::OnBnClickedResetSerialButton()
{
	GetDlgItem(IDC_CONNECTINFO_SERIAL_STATIC)->SetWindowText(_T(""));
	m_baudrate_control.SetCurSel(4);
	m_serial_control.SetCurSel(0);
	UpdateData(false);
}

void CSerial::GetSerialPortList(CComboBox& cb)
{
	cb.ResetContent();
	HKEY hKey ;  //�򿪴���ע����Ӧ�ļ�ֵ
	int res = 0;
	if ((res = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		_T("Hardware\\DeviceMap\\SerialComm"), 
		NULL,  KEY_READ, &hKey))
		== ERROR_SUCCESS)
	{
		int i=0;  
		TCHAR portName[256],commName[256];    
		DWORD dwLong,dwSize;    
		while(1)
		{    
			dwLong   =   dwSize   =   sizeof(portName);    
			if( ::RegEnumValue(hKey,    i,    portName,    &dwLong,    
				NULL,    NULL,     (PUCHAR)commName,     &dwSize   )   ==   ERROR_NO_MORE_ITEMS )//   ö�ٴ���    
				break; 
			cb.AddString(  commName   );   // commName���Ǵ�������    
			i++;  
		}  
		RegCloseKey(hKey);    
	}  
}

void CSerial::OnBnClickedExitSerialButton()
{
	TerminateThread(hThread, 0);
	CDialog::OnOK();
}

//ȫ�ֺ���
static DWORD WINAPI  ReadCarCardInfo(void *pArg)
{
	CarCardThreadParameter *m_parameter = (struct CarCardThreadParameter*)pArg;

	unsigned char begin = 2;
	unsigned char length = 6; // ѡ����������0��ʼ
	unsigned char buf[128] = {0};// ���ջ�����	
	int readAgainRate;
	CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����  
	BOOL ifFind = finder.FindFile(_T("./Config.ini"));  
	if( ifFind )  
	{  
		CString _temp = L"";
		::GetPrivateProfileStringW(_T("ReadAgainRate Config Info"),_T("Rate"),_T("10"),_temp.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
		_temp.ReleaseBuffer();  
		readAgainRate = _ttoi(_temp);
	}
	else
		readAgainRate = 10;

	CVoice _voice;
	_voice.Initial();
	int _warn_count = readAgainRate;
	while(1)
	{
		if ( ReadTagData(0,  1, begin, length, &buf[0]))
		{
			CString temp;
			m_parameter->m_carCardInfo = CString("�����ɹ���");
			for(int i = 3; i < length *2 + 3; ++i) // ǰ3�ֽ�Ϊ�����bank��begin��length
			{
				temp.Format(L"%02X ", buf[i]);
				m_parameter->m_carCardInfo += temp;
			}

			m_parameter->m_dialog->SetDlgItemText(IDC_CONNECTINFO_SERIAL_STATIC, m_parameter->m_carCardInfo);
			_voice.SpeakInfo(m_parameter->m_carCardInfo);
			m_parameter->m_isSuccess = true;
			ExitThread(0);
		}
		else
		{
			if(_warn_count == readAgainRate)
			{
				_voice.SpeakInfo(L"������ˢ����");
				m_parameter->m_isSuccess = false;
				m_parameter->m_dialog->SetDlgItemText(IDC_CONNECTINFO_SERIAL_STATIC, L"����ʧ�ܣ�");
				_warn_count = 0;
			}
			else
				_warn_count++;
		}
		
	}	
}