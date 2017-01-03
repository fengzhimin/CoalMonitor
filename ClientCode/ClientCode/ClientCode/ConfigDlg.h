#pragma once


// CConfigDlg 对话框

class CConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConfigDlg();
	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_CONFIG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
