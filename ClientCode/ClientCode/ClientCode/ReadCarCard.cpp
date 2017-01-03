// ReadCarCard.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientCode.h"
#include "ReadCarCard.h"
#include "..//dll/disdll.h"
#pragma comment(lib, "..//dll/disdll.lib")

// CReadCarCard 对话框
CReadCarCard* pReadCarCard = NULL;
static int tagCount = 0;
static int dataCount = 0;

IMPLEMENT_DYNAMIC(CReadCarCard, CDialogEx)

CReadCarCard::CReadCarCard(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReadCarCard::IDD, pParent)
	, m_cardInfo(_T(""))
{

}

CReadCarCard::~CReadCarCard()
{
}

void CReadCarCard::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC__CONTENT_READCARCARD_EDIT, m_cardInfo);
}

BOOL CReadCarCard::OnInitDialog()
{
	CDialog::OnInitDialog();
	pReadCarCard = this;

	GetDlgItem(IDC_STOP_READCARCARD_BUTTON)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BEGIN_MESSAGE_MAP(CReadCarCard, CDialogEx)
	ON_BN_CLICKED(IDC_READCARCARD_READCARCARD_BUTTON, &CReadCarCard::OnBnClickedReadcarcardReadcarcardButton)
	ON_BN_CLICKED(IDC_CONTINUE_READCARCARD_BUTTON, &CReadCarCard::OnBnClickedContinueReadcarcardButton)
	ON_BN_CLICKED(IDC_STOP_READCARCARD_BUTTON, &CReadCarCard::OnBnClickedStopReadcarcardButton)
	ON_BN_CLICKED(IDC_RESET_READCARCARD_BUTTON, &CReadCarCard::OnBnClickedResetReadcarcardButton)
END_MESSAGE_MAP()

void CALLBACK HandleData(unsigned char* pData, int length)
{
	CString strCount, strTagCount;
	if (length >  1 && length <=	 15)// 去除无效包
	{
		dataCount++; // 读取次数加1，并显示
		if (dataCount == (0xFFFFFFFF - 1))
		{
			dataCount = 0;
		}

		CString data = CString("");
		CString temp;
		for(int i = 1; i <= length - 3; ++i)
		{
			temp.Format(L"%02X ", pData[i]);
			data += CString(temp);
		}

		pReadCarCard->SetDlgItemText(IDC_CONNECTINFO_READCARCARD_STATIC, data);
		pReadCarCard->SetDlgItemText(IDC__CONTENT_READCARCARD_EDIT, data);
	}

}

// CReadCarCard 消息处理程序


void CReadCarCard::OnBnClickedReadcarcardReadcarcardButton()
{
	BYTE TagData[16] = {0}; 
	BYTE antNo = 0;
	BYTE usercode = 0;

	int count = 3;
	while (!ReadSingleTag(usercode,  &TagData[1],  &antNo) && count >= 0)
	{
		count--;
	}
	if (count >= 0)
	{
			TagData[0] = usercode; 
			TagData[13] = antNo;
			HandleData(&TagData[0],  15);
	}
	else
	{
		GetDlgItem(IDC_CONNECTINFO_READCARCARD_STATIC)->SetWindowText(_T("读取标签失败"));
	}	
}


void CReadCarCard::OnBnClickedContinueReadcarcardButton()
{
	int loop_count = 0;
	while(1)
	{
		BYTE TagData[16] = {0}; 
		BYTE antNo = 0;
		BYTE usercode = 0;

		int count = 3;
		while (!ReadSingleTag(usercode,  &TagData[1],  &antNo) && count >= 0)
		{
			count--;
		}
		if (count >= 0)
		{
				TagData[0] = usercode; 
				TagData[13] = antNo;
				HandleData(&TagData[0],  15);
				if(loop_count == 10)
					break;
				else
					loop_count++;
		}
	}

	GetDlgItem(IDC_STOP_READCARCARD_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_READCARCARD_READCARCARD_BUTTON)->EnableWindow(FALSE);
}


void CReadCarCard::OnBnClickedStopReadcarcardButton()
{
	GetDlgItem(IDC_STOP_READCARCARD_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_READCARCARD_READCARCARD_BUTTON)->EnableWindow(TRUE);
}


void CReadCarCard::OnBnClickedResetReadcarcardButton()
{
	GetDlgItem(IDC_CONNECTINFO_READCARCARD_STATIC)->SetWindowText(_T(""));
	m_cardInfo = L"";
	UpdateData(false);
}
