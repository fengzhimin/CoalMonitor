#pragma once
#include "afxwin.h"


// CParaSet 对话框

class CParaSet : public CDialogEx
{
	DECLARE_DYNAMIC(CParaSet)

public:
	CParaSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CParaSet();

// 对话框数据
	enum { IDD = IDD_PARASET_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	int m_usercode;	
	CComboBox m_workmode;
	CString m_TimingInterval;
	CComboBox m_zfmode;
	CButton m_neijdg;
	CString m_NeighJudge;
	CComboBox m_CommMode;
	CComboBox m_BaudRate;
	CString m_PulseWidth;
	CString m_PulseCycle;
	CComboBox m_WiegandProto;
	afx_msg void OnBnClickedBtnWmread();
	afx_msg void OnBnClickedBtnWmdef();
	afx_msg void OnCbnSelchangeComboWorkmode();
	afx_msg void OnBnClickedBtnWmset();
	afx_msg void OnBnClickedBtnCommread();
	afx_msg void OnBnClickedBtnCommdef();
	afx_msg void OnBnClickedBtnCommset();
	afx_msg void OnCbnSelchangeComboCommtype();
};
