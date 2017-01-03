#pragma once
#include "afxwin.h"


// CReadCarCard 对话框

class CReadCarCard : public CDialogEx
{
	DECLARE_DYNAMIC(CReadCarCard)

public:
	CReadCarCard(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReadCarCard();

// 对话框数据
	enum { IDD = IDD_READCARCARD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReadcarcardReadcarcardButton();
	afx_msg void OnBnClickedContinueReadcarcardButton();
	CString m_cardInfo;
	afx_msg void OnBnClickedStopReadcarcardButton();
	afx_msg void OnBnClickedResetReadcarcardButton();
};
