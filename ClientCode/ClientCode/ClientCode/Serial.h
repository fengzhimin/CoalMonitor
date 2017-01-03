#pragma once
#include "afxwin.h"
#include "CarCardInfo.h"
#include "CarCardThreadParameter.h"

// CSerial 对话框

class CSerial : public CDialogEx
{
	DECLARE_DYNAMIC(CSerial)

public:
	CSerial(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSerial();
	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_SERIAL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	struct CarCardThreadParameter m_carCardThreadParameter;
	CComboBox m_serial_control;
	CString m_serial_string;
	CComboBox m_baudrate_control;
	CString m_baudrate_string;
	DWORD   m_readCarCardThreadID;
	HANDLE hThread;
	afx_msg void OnBnClickedConnectSerialButton();
	afx_msg void OnBnClickedResetSerialButton();
	void GetSerialPortList(CComboBox& cb);
	afx_msg void OnBnClickedExitSerialButton();
};
