#pragma once


// CConfigDlg �Ի���

class CConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConfigDlg();
	virtual BOOL OnInitDialog();
// �Ի�������
	enum { IDD = IDD_CONFIG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CFont m_font_static, m_font_edit, m_font_button;
public:
	afx_msg void OnBnClickedSaveButton();
	afx_msg void OnBnClickedCancelButton();
	CString m_ip;
	CString m_database;
	CString m_loginName;
	CString m_passwd;
};
