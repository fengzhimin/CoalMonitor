// WriteCarCard.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientCode.h"
#include "WriteCarCard.h"
#include "afxdialogex.h"
#include "..//dll/disdll.h"
#pragma comment(lib, "..//dll/disdll.lib")

// CWriteCarCard 对话框

IMPLEMENT_DYNAMIC(CWriteCarCard, CDialogEx)

CWriteCarCard::CWriteCarCard(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWriteCarCard::IDD, pParent)
	, m_rwdata(_T(""))
{

}

CWriteCarCard::~CWriteCarCard()
{
}

void CWriteCarCard::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START_WRITECARCARD_COMBO, m_start);
	DDX_Control(pDX, IDC_LENGTH_WRITECARCARD_COMBO, m_length);
	DDX_Text(pDX, IDC_RW_WRITECARCARD_EDIT, m_rwdata);
}


BEGIN_MESSAGE_MAP(CWriteCarCard, CDialogEx)
	ON_BN_CLICKED(IDC_READ_WRITECARCARD_BUTTON, &CWriteCarCard::OnBnClickedReadWritecarcardButton)
	ON_BN_CLICKED(IDC_WRITE_WRITECARCARD_BUTTON, &CWriteCarCard::OnBnClickedWriteWritecarcardButton)
	ON_BN_CLICKED(IDC_RESET_WRITECARCARD_BUTTON, &CWriteCarCard::OnBnClickedResetWritecarcardButton)
	ON_CBN_SELCHANGE(IDC_START_WRITECARCARD_COMBO, &CWriteCarCard::OnCbnSelchangeStartWritecarcardCombo)
END_MESSAGE_MAP()

BOOL CWriteCarCard::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString _str;
	for (int i = 2  ; i <= 7; ++i)
	{
		_str.Format(L"%d",  i );
		m_start.AddString(_str);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// CWriteCarCard 消息处理程序


void CWriteCarCard::OnBnClickedReadWritecarcardButton()
{
	UpdateData();
	
	int index = m_start.GetCurSel();
	if (-1 == index)	// 没有选择有效项目
	{
		GetDlgItem(IDC_SHOWINFO_WRITECARCARD_STATIC)->SetWindowText(_T("请选择起始地址！"));
		return;
	}
	unsigned char begin = 2 + index;

	index = m_length.GetCurSel();
	if (-1 == index)	// 没有选择有效项目
	{
		GetDlgItem(IDC_SHOWINFO_WRITECARCARD_STATIC)->SetWindowText(_T("请选择长度！"));
		return;
	}
	unsigned char length = m_length.GetCurSel() + 1; // 选择项索引从0开始

	unsigned char buf[128] = {0};// 接收缓冲区	
	if ( ReadTagData(0,  1, begin, length, &buf[0]))
	{
		CString temp;
		m_rwdata = CString("");
		for(int i = 3; i < length *2 + 3; ++i) // 前3字节为传入的bank、begin、length
		{
			temp.Format(L"%02X ", buf[i]);
			m_rwdata += temp;
		}
		UpdateData(FALSE);
		GetDlgItem(IDC_SHOWINFO_WRITECARCARD_STATIC)->SetWindowText(_T("读取成功"));
	}
	else
	{
		GetDlgItem(IDC_SHOWINFO_WRITECARCARD_STATIC)->SetWindowText(_T("读取失败"));
	}	
}


void CWriteCarCard::OnBnClickedWriteWritecarcardButton()
{
	UpdateData();
	
	if (m_rwdata.GetLength() == 0)
	{
		MessageBox(_T("没有数据."), _T("错误"), 0);
		return;
	}
	int index = m_start.GetCurSel();
	if (-1 == index)	// 没有选择有效项目
	{
		MessageBox(_T("请选择起始地址！"), _T("错误"), 0);
		return;
	}
	// bank为EPC(1)时，起始地址为2
	unsigned char begin = 2 + index;

	m_rwdata.Remove(' '); // 去除加入的空格
	m_rwdata.Remove(','); // 去除加入的逗号

	if (CharsIsValid(m_rwdata))
	{
		int MaxLength = 0;
		MaxLength = 8 - begin;
		BYTE length = m_rwdata.GetLength() / 4; // 换算为字数
		 if (length > MaxLength)
		 {
			 	MessageBox(_T("数据长度超过最大值."),  _T("信息"), 0);
				return;
		 }
		//	转换写入数据
		unsigned char buf[128] = {0};
	
		for (int index = 0; index < length*2; ++index)
		{
			int i = CharsToHex(m_rwdata[2 * index])<<4;
			int j = CharsToHex(m_rwdata[2 * index + 1]);
			buf[index] = CharsToHex(m_rwdata[2 * index])<<4 | CharsToHex(m_rwdata[2 * index + 1]);
		}

		if ( WriteTagData(0, 1, begin, length, &buf[0]))
		{
			GetDlgItem(IDC_SHOWINFO_WRITECARCARD_STATIC)->SetWindowText(_T("写入成功"));
		}
		else
		{
			GetDlgItem(IDC_SHOWINFO_WRITECARCARD_STATIC)->SetWindowText(_T("写入失败"));
		}	
	}
}


void CWriteCarCard::OnBnClickedResetWritecarcardButton()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CWriteCarCard::OnCbnSelchangeStartWritecarcardCombo()
{
	UpdateData();
	m_length.ResetContent();
	int begin = 1;
	int end = 6 -  m_start.GetCurSel();; // 最大数据长度

	CString temp;
	for (int index = begin; index <=  end; ++index)
	{
		temp.Format(L"%d", index);
		m_length.AddString(temp);
	}
}

BOOL CWriteCarCard::CharsIsValid(CString& str)
{
	int length = str.GetLength();
	if (0 == length)
	{
		MessageBox(_T("没有数据"), _T("错误"), 0);
		return FALSE;
	}
	for (int i = 0; i < length;  ++i) // 检查字符是否为无效字符
	{
		char ch = str.GetAt(i);
		if ( (ch <  '0')   ||
			(ch > '9' && ch < 'A') ||
			(ch > 'F'  && ch < 'a') ||
			(ch  > 'f'))
		{
			MessageBox(_T("含有无效字符"), _T("错误"), 0);
			return FALSE;
		}
	}
	if (length % 4 != 0)
	{
		MessageBox(_T("数据位数必须是4的整倍数"), _T("错误"), 0);
		return FALSE;
	}
	return TRUE;
}

int CWriteCarCard::CharsToHex(char _ch)
{
	if(_ch >= '0' && _ch <= '9')
		return _ch - '0';
	else
	{
		if(_ch >= 'a' && _ch <= 'f')
			return _ch - 87;
		else if(_ch >= 'A' && _ch <= 'F')
			return _ch - 55;
		else
			return 0;
	}
}