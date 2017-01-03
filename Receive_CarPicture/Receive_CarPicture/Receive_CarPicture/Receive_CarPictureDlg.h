
// Receive_CarPictureDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CReceive_CarPictureDlg 对话框
class CReceive_CarPictureDlg : public CDialogEx
{
// 构造
public:
	CReceive_CarPictureDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CReceive_CarPictureDlg();
// 对话框数据
	enum { IDD = IDD_RECEIVE_CARPICTURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartButton();
	afx_msg void OnBnClickedStopButton();
	afx_msg void OnBnClickedSaveconfigButton();
	CIPAddressCtrl m_ip_control;
	CString m_port_string;
	CString m_PicturePath_string;
	CString GetSelectFolderPath();
	CString m_skin;  //皮肤
	int m_nSend;
	afx_msg void OnBnClickedResetButton();
	afx_msg void OnBnClickedBrowseButton();
	afx_msg void OnBnClickedSendButton();
	LRESULT OnUpdateSocketInfo(WPARAM wParam, LPARAM lParam);  
};
