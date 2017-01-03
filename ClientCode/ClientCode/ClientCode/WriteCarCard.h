#pragma once
#include "afxwin.h"


// CWriteCarCard 对话框

class CWriteCarCard : public CDialogEx
{
	DECLARE_DYNAMIC(CWriteCarCard)

public:
	CWriteCarCard(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWriteCarCard();

// 对话框数据
	enum { IDD = IDD_WRITECARCARD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReadWritecarcardButton();
	afx_msg void OnBnClickedWriteWritecarcardButton();
	afx_msg void OnBnClickedResetWritecarcardButton();
	BOOL CharsIsValid(CString& str);
	int CharsToHex(char _ch);
	CComboBox m_start;
	CComboBox m_length;
	afx_msg void OnCbnSelchangeStartWritecarcardCombo();
	CString m_rwdata;
};
