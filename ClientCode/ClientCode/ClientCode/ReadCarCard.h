#pragma once
#include "afxwin.h"


// CReadCarCard �Ի���

class CReadCarCard : public CDialogEx
{
	DECLARE_DYNAMIC(CReadCarCard)

public:
	CReadCarCard(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReadCarCard();

// �Ի�������
	enum { IDD = IDD_READCARCARD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReadcarcardReadcarcardButton();
	afx_msg void OnBnClickedContinueReadcarcardButton();
	CString m_cardInfo;
	afx_msg void OnBnClickedStopReadcarcardButton();
	afx_msg void OnBnClickedResetReadcarcardButton();
};
