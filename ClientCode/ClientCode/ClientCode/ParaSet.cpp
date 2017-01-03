// ParaSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientCode.h"
#include "ParaSet.h"
#include "afxdialogex.h"
#include "..//dll/disdll.h"
#pragma comment(lib, "..//dll/disdll.lib")

// CParaSet �Ի���

IMPLEMENT_DYNAMIC(CParaSet, CDialogEx)

CParaSet::CParaSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CParaSet::IDD, pParent)
	, m_TimingInterval(_T(""))
	, m_NeighJudge(_T(""))
	, m_PulseWidth(_T(""))
	, m_PulseCycle(_T(""))
	, m_usercode(0)
{

}

CParaSet::~CParaSet()
{
}

void CParaSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_WORKMODE, m_workmode);
	DDX_Text(pDX, IDC_EDIT_TIMING, m_TimingInterval);
	DDX_Control(pDX, IDC_COMBO_TRIG, m_zfmode);
	DDX_Control(pDX, IDC_CHECK_NEIJDG, m_neijdg);
	DDX_Text(pDX, IDC_EDIT_NEIJDG, m_NeighJudge);
	DDX_Control(pDX, IDC_COMBO_COMMTYPE, m_CommMode);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_BaudRate);
	DDX_Text(pDX, IDC_EDIT_PALWIDTH, m_PulseWidth);
	DDX_Text(pDX, IDC_EDIT_PALCYCLE, m_PulseCycle);
	DDX_Control(pDX, IDC_COMBO_PROTO, m_WiegandProto);
}


BEGIN_MESSAGE_MAP(CParaSet, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_WMREAD, &CParaSet::OnBnClickedBtnWmread)
	ON_BN_CLICKED(IDC_BTN_WMDEF, &CParaSet::OnBnClickedBtnWmdef)
	ON_CBN_SELCHANGE(IDC_COMBO_WORKMODE, &CParaSet::OnCbnSelchangeComboWorkmode)
	ON_BN_CLICKED(IDC_BTN_WMSET, &CParaSet::OnBnClickedBtnWmset)
	ON_BN_CLICKED(IDC_BTN_COMMREAD, &CParaSet::OnBnClickedBtnCommread)
	ON_BN_CLICKED(IDC_BTN_COMMDEF, &CParaSet::OnBnClickedBtnCommdef)
	ON_BN_CLICKED(IDC_BTN_COMMSET, &CParaSet::OnBnClickedBtnCommset)
	ON_CBN_SELCHANGE(IDC_COMBO_COMMTYPE, &CParaSet::OnCbnSelchangeComboCommtype)
END_MESSAGE_MAP()

BOOL CParaSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_workmode.AddString(_T("����ģʽ"));
	m_workmode.AddString(_T("��ʱģʽ"));
	m_workmode.AddString(_T("����ģʽ"));

	m_zfmode.AddString(_T("������"));	
	m_zfmode.AddString(_T("�ߵ�ƽ����"));
	
	m_workmode.SetCurSel(0);
	GetDlgItem(IDC_STA_LBTIMING)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_TIMING)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STA_LBTIMING_MS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMBO_TRIG)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STA_ZFMODE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_NEIJDG)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STA_NEIJDG_S)->ShowWindow(SW_HIDE);

	m_CommMode.AddString(_T("RS485"));
	m_CommMode.AddString(_T("Wiegand"));
	m_CommMode.AddString(_T("RS232"));


	m_BaudRate.AddString(_T("9600bps"));
	m_BaudRate.AddString(_T("19200bps"));
	m_BaudRate.AddString(_T("38400bps"));
	m_BaudRate.AddString(_T("57600bps"));
	m_BaudRate.AddString(_T("115200bps"));
	m_BaudRate.AddString(_T("2400bps"));
	
	m_WiegandProto.AddString(_T("Wiegand26"));
	m_WiegandProto.AddString(_T("Wiegand34"));
	m_WiegandProto.AddString(_T("Wiegand32"));

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

// CParaSet ��Ϣ�������


void CParaSet::OnBnClickedBtnWmread()
{
	BYTE WorkMode = 0;
	if (GetSingleParameter(m_usercode, ADD_WORKMODE,  &WorkMode))
	{
		m_workmode.SetCurSel(WorkMode - 1);
		OnCbnSelchangeComboWorkmode();
		switch(WorkMode)
		{
		case WM_MASTER_SLAVE: // ����ģʽ
			{
				break;
			}
		case WM_TIMING:// ��ʱģʽ
			{
				BYTE TimingInterval = 0;
				if (GetSingleParameter(m_usercode, ADD_TIME_INTERVAL, &TimingInterval))
				{
					CString temp;
					temp.Format(L"%d", TimingInterval);
					m_TimingInterval = temp;
				}
				else
				{
					MessageBox(_T("��ȡ��ʱ����ʧ��"));
					return;
				}
				break;
			}
		case WM_TRIG:// ����ģʽ
			{
				BYTE DelayTime = 0;
				if (GetSingleParameter(m_usercode, ADD_TRIG_DELAYTIME, &DelayTime))  // ��ȡ�ӳ�ʱ��
				{
					CString temp;
					temp.Format(L"%d", DelayTime);
					m_TimingInterval = temp;
				}
				else
				{
					MessageBox(_T("��ȡ�ӳ�ʱ��ʧ��"));
				}
				BYTE Trig = 0; 
				if (GetSingleParameter(m_usercode, ADD_TRIG_SWITCH,  &Trig))  // ��ȡ��������
				{
					m_zfmode.SetCurSel(0x1 & Trig); // ֻ�������λ
				}
				else
				{
					MessageBox(_T("��ȡ��������ʧ��"));
				}
				break;
			}
		}		
	}
	else
	{
		MessageBox(_T("��ȡ����ģʽ����ʧ��"));
		return;
	}
	BYTE NeiJdg = 0;
	if (GetSingleParameter(m_usercode,  ADD_NEIGHJUDGE_SET,  &NeiJdg))
	{
		switch(NeiJdg)
		{
		case 1: // ���������б�
			{
				m_neijdg.SetCheck(TRUE);
				BYTE NeiJdgTime = 0;
				if (GetSingleParameter(m_usercode,  ADD_NEIGHJUDGE_TIME,  &NeiJdgTime))
				{
					CString temp;
					temp.Format(L"%d", NeiJdgTime);
					m_NeighJudge = temp;
					GetDlgItem(IDC_EDIT_NEIJDG)->ShowWindow(SW_NORMAL);
					GetDlgItem(IDC_STA_NEIJDG_S)->ShowWindow(SW_NORMAL);
				}
				else
				{
					MessageBox(_T("��ȡ�����б�ʱ��ʧ��"));
				}		
				break;
			}
		case 2: // δ���������б�
			{
				m_neijdg.SetCheck(FALSE);
				GetDlgItem(IDC_EDIT_NEIJDG)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STA_NEIJDG_S)->ShowWindow(SW_HIDE);
				break;
			}
		default: // ����ֵ
			break;
		}
	}
	UpdateData(FALSE);	
}


void CParaSet::OnBnClickedBtnWmdef()
{
	m_workmode.SetCurSel(1); // ��ʱģʽ
	OnCbnSelchangeComboWorkmode();
	m_neijdg.SetCheck(FALSE); // ���������б�
	GetDlgItem(IDC_EDIT_NEIJDG)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STA_NEIJDG_S)->ShowWindow(SW_HIDE);
}


void CParaSet::OnCbnSelchangeComboWorkmode()
{
	UpdateData();
	switch(m_workmode.GetCurSel() + 1)
	{
	case WM_MASTER_SLAVE:// ����ģʽ
		{
			GetDlgItem(IDC_STA_LBTIMING)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_TIMING)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STA_LBTIMING_MS)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_TRIG)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STA_ZFMODE)->ShowWindow(SW_HIDE);
		}
		break;
	case WM_TIMING: // ��ʱģʽ
		{
			m_TimingInterval = _T("20"); // Ĭ��ֵ20*10ms
			GetDlgItem(IDC_STA_LBTIMING)->SetWindowText(_T("��ʱ���(2-100)"));
			GetDlgItem(IDC_STA_LBTIMING)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_EDIT_TIMING)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STA_LBTIMING_MS)->SetWindowText(_T("*10����"));
			GetDlgItem(IDC_STA_LBTIMING_MS)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_COMBO_TRIG)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STA_ZFMODE)->ShowWindow(SW_HIDE);
		}
		break;
	case WM_TRIG: // ����ģʽ
		{
			m_TimingInterval = _T("10"); // Ĭ�ϳ��ӹر�ʱ����10s
			m_zfmode.SetCurSel(1);
			GetDlgItem(IDC_STA_LBTIMING)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STA_LBTIMING)->SetWindowText(_T("�ӳٹر�(0-240)"));
			GetDlgItem(IDC_EDIT_TIMING)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STA_LBTIMING_MS)->SetWindowText(_T("��"));
			GetDlgItem(IDC_STA_LBTIMING_MS)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_COMBO_TRIG)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STA_ZFMODE)->ShowWindow(SW_NORMAL);
			//m_zfmode.SetCurSel(1);
		}
	}
	UpdateData(FALSE);
}


void CParaSet::OnBnClickedBtnWmset()
{
	UpdateData();
	if (-1 == m_workmode.GetCurSel())
	{
		MessageBox(_T("��ѡ��һ�ֹ���ģʽ"));
		return;
	}
	BYTE WorkMode = m_workmode.GetCurSel() + 1; // ��ȡ����ģʽ����(��ЧֵΪ1��2��3)
	BYTE bSucceed = SetSingleParameter(m_usercode,  ADD_WORKMODE, WorkMode); // д�빤��ģʽ
	switch(WorkMode)
	{
	case WM_MASTER_SLAVE:// ����ģʽ
		{
			break;
		}
	case WM_TIMING: // ��ʱģʽ,
		{
			UINT TimeInterval = _ttoi(m_TimingInterval.GetBuffer());
			if (TimeInterval < 2 ||  TimeInterval > 100)
			{
				MessageBox(_T("��ʱ�������Чֵ��2-100"));
				return;
			}
			bSucceed *= SetSingleParameter(m_usercode,  ADD_TIME_INTERVAL,  TimeInterval);
			break;
		}
	case WM_TRIG: // ����ģʽ
		{
			UINT DelayTime = _ttoi(m_TimingInterval.GetBuffer());
			if (DelayTime > 240)
			{
				MessageBox(_T("�ӳ�ʱ������ֵ��240"));
				return;
			}
			bSucceed *= SetSingleParameter(m_usercode, ADD_TRIG_DELAYTIME,  DelayTime);// д���ӳ�ʱ��
			bSucceed *= SetSingleParameter(m_usercode, ADD_TRIG_SWITCH,  m_zfmode.GetCurSel()); // ���ùܽ�0
			bSucceed *= SetSingleParameter(m_usercode, ADD_TRIG_MODE, 0x1);// �ܽ�0���óɸߵ�ƽģʽ��0Ϊ�͵�ƽ
			break;
		}
	}
	// �����б�
	if (m_neijdg.GetCheck())
	{
		bSucceed *= SetSingleParameter(m_usercode, ADD_NEIGHJUDGE_SET, 1); // ���������б�
		UINT TimeNeiJdg = _ttoi(m_NeighJudge.GetBuffer());
		if (TimeNeiJdg < 1 || TimeNeiJdg > 200 )
		{
			MessageBox(_T("�����б�ʱ������ֵ��200"));
			return;
		}
		bSucceed *= SetSingleParameter(m_usercode, ADD_NEIGHJUDGE_TIME,  TimeNeiJdg);
	}
	else
	{
		bSucceed *= SetSingleParameter(m_usercode, ADD_NEIGHJUDGE_SET, 2); // �����������б�
	}
	if (bSucceed)
	{
		MessageBox(_T("����ģʽ�������óɹ�"),  _T("��Ϣ"), 0);
	}
	else
	{
		MessageBox(_T("����ģʽ��������ʧ��"),  _T("��Ϣ"), 0);
	}	
}


void CParaSet::OnBnClickedBtnCommread()
{
	BYTE CommMode = 0;
	if (GetSingleParameter(m_usercode,  ADD_COMM_MODE,  &CommMode))
	{
		m_CommMode.SetCurSel(CommMode - 1);  // ����ͨѶ��ʽ����Чֵ������ֵ��1
		OnCbnSelchangeComboCommtype(); // ����ģʽ��ʾ��Ӧ��ѡ��
		switch(CommMode)
		{
		case MODE_RS232: // RS232��RS485��������
		case  MODE_RS485:
			{
				BYTE BaudRate = 0;
				if (GetSingleParameter(m_usercode,  ADD_BAUD_RATE, &BaudRate ))
				{
					m_BaudRate.SetCurSel(BaudRate);
				}
				break;
			}
		case MODE_Wiegend:
			{
				BYTE WiegandProto = 0;
				if (GetSingleParameter(m_usercode, ADD_WIEGAND_PROTO,  &WiegandProto))
				{
					m_WiegandProto.SetCurSel(WiegandProto - 1);
				}
				BYTE WiegandPulseWidth = 0;
				if (GetSingleParameter(m_usercode,  ADD_WIEGAND_PULSEWIDTH,  &WiegandPulseWidth))
				{
					CString temp;
					temp.Format(L"%d",  WiegandPulseWidth);
					m_PulseWidth = temp;
				}
				BYTE WiegandPulseCycle = 0;
				if (GetSingleParameter(m_usercode, ADD_WIEGAND_PULSECYCLE, &WiegandPulseCycle))
				{
					CString temp;
					temp.Format(L"%d",  WiegandPulseCycle);
					m_PulseCycle = temp;
				}
				UpdateData(FALSE);
				break;
			}
		}
	}
	else
	{
		MessageBox(_T("��ȡͨѶ����ʧ��"));
		return;
	}
}


void CParaSet::OnBnClickedBtnCommdef()
{
	m_CommMode.SetCurSel(2); // RS232
	OnCbnSelchangeComboCommtype();
}


void CParaSet::OnBnClickedBtnCommset()
{
	UpdateData();
	if (-1 == m_CommMode.GetCurSel())
	{
		MessageBox(_T("��ѡ��ͨѶ��ʽ"));
		return;
	}
	BYTE CommMode = m_CommMode.GetCurSel() +1;
	BOOL bSucceed = SetSingleParameter(m_usercode,  ADD_COMM_MODE, CommMode);
	switch(CommMode)
	{
	case MODE_RS232:
	case MODE_RS485:
		{
			int BaudRate = m_BaudRate.GetCurSel();
			if (-1 == BaudRate)
			{
				MessageBox(_T("��ѡ������"));
				return;
			}
			bSucceed *= SetSingleParameter(m_usercode,  ADD_BAUD_RATE,  BaudRate);
			break;
		}
	case MODE_Wiegend:
		{
			int WiegandProto = m_WiegandProto.GetCurSel() + 1;
			if (0 == WiegandProto)
			{
					MessageBox(_T("��ѡ��Τ��Э��"));
					return;
			}
			bSucceed *= SetSingleParameter(m_usercode, ADD_WIEGAND_PROTO, WiegandProto );
			UINT WiegandPulseWidth = _ttoi(m_PulseWidth.GetBuffer());
			UINT WiegandPulseCycle = _ttoi(m_PulseCycle.GetBuffer());
			if (WiegandPulseCycle > 255 || WiegandPulseWidth > 255 ||
				WiegandPulseCycle < 1	    || WiegandPulseWidth < 1)
			{
				MessageBox(_T("�����Ⱥ�����������Чֵ��1��255"));
				return;
			}
			bSucceed *= SetSingleParameter(m_usercode, ADD_WIEGAND_PULSEWIDTH,  WiegandPulseWidth);
			bSucceed *= SetSingleParameter(m_usercode, ADD_WIEGAND_PULSECYCLE,  WiegandPulseCycle);
			break;
		}
	}
	if (bSucceed)
	{
		MessageBox(_T("ͨѶ�������óɹ�"),  _T("��Ϣ"), 0);
	}
	else
	{
		MessageBox(_T("ͨѶ��������ʧ��"),  _T("��Ϣ"), 0);
	}	
}


void CParaSet::OnCbnSelchangeComboCommtype()
{
	UpdateData();
	switch(m_CommMode.GetCurSel() + 1)
	{
	case MODE_RS232:
	case MODE_RS485:
		{
			m_BaudRate.SetCurSel(0); // Ĭ��ֵ9600bps
			GetDlgItem(IDC_STA_LBBAUD)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_COMBO_BAUD)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STA_LBPULWID)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STA_LBPULCYC)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STA_LBWIEPRO)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_PALWIDTH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_PALCYCLE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STA_PULWID_US)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STA_PULCYC_US)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_PROTO)->ShowWindow(SW_HIDE);
			break;
		}
	case MODE_Wiegend:
		{
			m_PulseCycle = _T("16");
			m_PulseWidth = _T("10");
			UpdateData(FALSE);
			m_WiegandProto.SetCurSel(0);
			GetDlgItem(IDC_STA_LBBAUD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO_BAUD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STA_LBPULWID)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STA_LBPULCYC)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STA_LBWIEPRO)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_EDIT_PALWIDTH)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_EDIT_PALCYCLE)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STA_PULWID_US)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STA_PULCYC_US)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_COMBO_PROTO)->ShowWindow(SW_NORMAL);
		}
	default:
		break;
	}
}
