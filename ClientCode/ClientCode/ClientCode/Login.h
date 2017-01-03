#pragma once
#include "afxwin.h"


// CLogin �Ի���

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogin();
	virtual BOOL OnInitDialog();
// �Ի�������
	enum { IDD = IDD_LOGIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CFont m_font_static, m_font_edit, m_font_button;
public:
	afx_msg void OnBnClickedLoginLoginButton();
	afx_msg void OnBnClickedCancelLoginButton();
	CString m_loginName;
	CString m_passwd;
	BOOL m_remember_check;
	CEdit m_loginName_Control;
	CEdit m_passwd_Control;
};
