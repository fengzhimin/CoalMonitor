// ShowDiffInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "MonitorCode.h"
#include "ShowDiffInfo.h"
#include "afxdialogex.h"
#include "Log.h"

extern Log m_log;

// ShowDiffInfo 对话框

IMPLEMENT_DYNAMIC(ShowDiffInfo, CDialogEx)

ShowDiffInfo::ShowDiffInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(ShowDiffInfo::IDD, pParent)
, m_style_string(_T(""))
	, m_carCard_string(_T(""))
	, m_agreementA_string(_T(""))
	, m_agreementB_string(_T(""))
	, m_cargoSize_string1(_T(""))
	, m_cargoSize_string2(_T(""))
	, m_cargoSize_string3(_T(""))
	, m_cargoWeight_string1(_T(""))
	, m_cargoWeight_string2(_T(""))
	, m_cargoWeight_string3(_T(""))
	, m_cargoDensity_string1(_T(""))
	, m_cargoDensity_string2(_T(""))
	, m_cargoDensity_string3(_T(""))
	, m_deductWeight_string1(_T(""))
	, m_deductWeight_string2(_T(""))
	, m_deductWeight_string3(_T(""))
	, m_emptyCarSize_string1(_T(""))
	, m_emptyCarSize_string2(_T(""))
	, m_emptyCarSize_string3(_T(""))
	, m_carCargoSize_string1(_T(""))
	, m_carCargoSize_string2(_T(""))
	, m_carCargoSize_string3(_T(""))
	, m_emptyCarWeight_string1(_T(""))
	, m_emptyCarWeight_string2(_T(""))
	, m_emptyCarWeight_string3(_T(""))
	, m_carWeight_string1(_T(""))
	, m_carWeight_string2(_T(""))
	, m_carWeight_string3(_T(""))
{
	m_style = m_carCard = m_agreementA = m_agreementB= L"";
	m_cargoSize1 = m_cargoSize2 = L"";
	m_cargoWeight1 = m_cargoWeight2 = L"";
	m_cargoDensity1 = m_cargoDensity2 = L"";
	m_deductWeight1 = m_deductWeight2 = L"";
	m_emptyCarSize1 = m_emptyCarSize2 = L"";
	m_carCargoSize1 = m_carCargoSize2 = L"";
	m_emptyCarWeight1 = m_emptyCarWeight2 = L"";
	m_carWeight1 = m_carWeight2 = L"";
	m_font_static.CreatePointFont(200, L"宋体");
	m_font_edit.CreatePointFont(180, L"宋体");
}

ShowDiffInfo::~ShowDiffInfo()
{
	m_font_static.DeleteObject();
	m_font_edit.DeleteObject();
}

void ShowDiffInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STYLE_EDIT, m_style_string);
	DDX_Text(pDX, IDC_CARCARD_EDIT, m_carCard_string);
	DDX_Text(pDX, IDC_AGREEMENTA_EDIT, m_agreementA_string);
	DDX_Text(pDX, IDC_AGREEMENTB_EDIT, m_agreementB_string);
	DDX_Text(pDX, IDC_CARGOSIZE_EDIT1, m_cargoSize_string1);
	DDX_Text(pDX, IDC_CARGOSIZE_EDIT2, m_cargoSize_string2);
	DDX_Text(pDX, IDC_CARGOSIZE_EDIT3, m_cargoSize_string3);
	DDX_Text(pDX, IDC_CARGOWEIGHT_EDIT1, m_cargoWeight_string1);
	DDX_Text(pDX, IDC_CARGOWEIGHT_EDIT2, m_cargoWeight_string2);
	DDX_Text(pDX, IDC_CARGOWEIGHT_EDIT3, m_cargoWeight_string3);
	DDX_Text(pDX, IDC_CARGODENSITY_EDIT1, m_cargoDensity_string1);
	DDX_Text(pDX, IDC_CARGODENSITY_EDIT2, m_cargoDensity_string2);
	DDX_Text(pDX, IDC_CARGODENSITY_EDIT3, m_cargoDensity_string3);
	DDX_Text(pDX, IDC_DEDUCTWEIGHT_EDIT1, m_deductWeight_string1);
	DDX_Text(pDX, IDC_DEDUCTWEIGHT_EDIT2, m_deductWeight_string2);
	DDX_Text(pDX, IDC_DEDUCTWEIGHT_EDIT3, m_deductWeight_string3);
	DDX_Text(pDX, IDC_EMPTYCARSIZE_EDIT1, m_emptyCarSize_string1);
	DDX_Text(pDX, IDC_EMPTYCARSIZE_EDIT2, m_emptyCarSize_string2);
	DDX_Text(pDX, IDC_EMPTYCARSIZE_EDIT3, m_emptyCarSize_string3);
	DDX_Text(pDX, IDC_CARGOCARSIZE_EDIT1, m_carCargoSize_string1);
	DDX_Text(pDX, IDC_CARGOCARSIZE_EDIT2, m_carCargoSize_string2);
	DDX_Text(pDX, IDC_CARGOCARSIZE_EDIT3, m_carCargoSize_string3);
	DDX_Text(pDX, IDC_EMPTYCARWEIGHT_EDIT1, m_emptyCarWeight_string1);
	DDX_Text(pDX, IDC_EMPTYCARWEIGHT_EDIT2, m_emptyCarWeight_string2);
	DDX_Text(pDX, IDC_EMPTYCARWEIGHT_EDIT3, m_emptyCarWeight_string3);
	DDX_Text(pDX, IDC_CARWIEIGHT_EDIT1, m_carWeight_string1);
	DDX_Text(pDX, IDC_CARWIEIGHT_EDIT2, m_carWeight_string2);
	DDX_Text(pDX, IDC_CARWIEIGHT_EDIT3, m_carWeight_string3);
}


BEGIN_MESSAGE_MAP(ShowDiffInfo, CDialogEx)
	ON_BN_CLICKED(IDC_CONFIRE_BUTTON, &ShowDiffInfo::OnBnClickedConfireButton)
	ON_BN_CLICKED(IDC_CANCEL_BUTTON, &ShowDiffInfo::OnBnClickedCancelButton)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void ShowDiffInfo::OnBnClickedConfireButton()
{	
	//设置为违规
	OperateDataBase _operateDataBase;
	_operateDataBase.setAbnormalToCarCheckInfoTable(m_carCard_string, 2);
	m_log.WriteOperateLog(L"车卡号：" + m_carCard + L" 进行了违规操作！");
	CDialog::OnOK();	
}


void ShowDiffInfo::OnBnClickedCancelButton()
{
	//设置没有违规
	OperateDataBase _operateDataBase;
	_operateDataBase.setAbnormalToCarCheckInfoTable(m_carCard_string, 0);
	m_log.WriteOperateLog(L"车卡号：" + m_carCard + L" 没有违规操作！");
	CDialog::OnCancel();
}


BOOL ShowDiffInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_style_string = m_style;
	m_carCard_string = m_carCard;
	m_agreementA_string = m_agreementA;
	m_agreementB_string = m_agreementB;
	m_cargoSize_string1 = m_cargoSize1;
	m_cargoSize_string2 = m_cargoSize2;
	m_cargoWeight_string1 = m_cargoWeight1;
	m_cargoWeight_string2 = m_cargoWeight2;
	m_cargoDensity_string1 = m_cargoDensity1;
	m_cargoDensity_string2 = m_cargoDensity2;
	m_deductWeight_string1 = m_deductWeight1;
	m_deductWeight_string2 = m_deductWeight2;
	m_emptyCarSize_string1 = m_emptyCarSize1;
	m_emptyCarSize_string2 = m_emptyCarSize2;
	m_carCargoSize_string1 = m_carCargoSize1;
	m_carCargoSize_string2 = m_carCargoSize2;
	m_emptyCarWeight_string1 = m_emptyCarWeight1;
	m_emptyCarWeight_string2 = m_emptyCarWeight2;
	m_carWeight_string1 = m_carWeight1;
	m_carWeight_string2 = m_carWeight2;

	for(int i = 1051; i <= 1066; i++)
	{
		skinppSetNoSkinHwnd(GetDlgItem(i)->m_hWnd, true);
		GetDlgItem(i)->SetFont(&m_font_static);
	}
	for(int i = 1023; i <= 1050; i++)
	{
		skinppSetNoSkinHwnd(GetDlgItem(i)->m_hWnd, true);
		GetDlgItem(i)->SetFont(&m_font_edit);
	}

	USES_CONVERSION;
	m_cargoSize_string3.Format(L"%f", atof(W2A(m_cargoSize_string2)) - atof(W2A(m_cargoSize_string1)));
	m_cargoWeight_string3.Format(L"%f", atof(W2A(m_cargoWeight_string2)) - atof(W2A(m_cargoWeight_string1)));
	m_cargoDensity_string3.Format(L"%f", atof(W2A(m_cargoDensity_string2)) - atof(W2A(m_cargoDensity_string1)));
	m_deductWeight_string3.Format(L"%f", atof(W2A(m_deductWeight_string2)) - atof(W2A(m_deductWeight_string1)));
	m_emptyCarSize_string3.Format(L"%f", atof(W2A(m_emptyCarSize_string2)) - atof(W2A(m_emptyCarSize_string1)));
	m_carCargoSize_string3.Format(L"%f", atof(W2A(m_carCargoSize_string2)) - atof(W2A(m_carCargoSize_string1)));
	m_emptyCarWeight_string3.Format(L"%f", atof(W2A(m_emptyCarWeight_string2)) - atof(W2A(m_emptyCarWeight_string1)));
	m_carWeight_string3.Format(L"%f", atof(W2A(m_carWeight_string2)) - atof(W2A(m_carWeight_string1)));

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}


HBRUSH ShowDiffInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(pWnd->GetDlgCtrlID() >= 1051 && pWnd->GetDlgCtrlID() <= 1066)    
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,120));  
	}
	else if(pWnd->GetDlgCtrlID() >= 1023 && pWnd->GetDlgCtrlID() <= 1050)
		pDC->SetTextColor(RGB(120,0,120)); 
	
	return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
}


// ShowDiffInfo 消息处理程序
