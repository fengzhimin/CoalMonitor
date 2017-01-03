
#include "Serial.h"
#include "DeviceOperation.h"

#pragma once

// CClientCodeDlg 对话框
class CClientCodeDlg : public CDialogEx
{
// 构造
public:
	CClientCodeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CLIENTCODE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
//	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CMenu m_login_menu, m_logout_menu;
	CSerial *m_serial;
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
	BOOL ifFind;
	BOOL m_bIsLogin;    //是否登录摄像头
	BOOL m_bIsPlaying;  //是否开始播放摄像头
	BOOL m_bIsRecording;//是否开始录像
	DWORD  m_threadID;
	HANDLE hThread;
	CDeviceOperation _deviceOperation;
	HWND hWnd;
public:
	afx_msg void OnLogin();
	afx_msg void OnAbout();
	afx_msg void OnConfig();
	afx_msg void OnClose();
	afx_msg void OnLogout();
	afx_msg void OnReadcarcard();
	afx_msg void OnWritecarcard();
	afx_msg void OnSerial();
	CString m_carCardInfo_string;
	afx_msg void OnParaset();
	afx_msg void OnBnClickedTextClientcodeButton();
	afx_msg void OnLoginOrLogout();
	afx_msg void OnStartOrStop();
	virtual BOOL OnInitDialog();
	afx_msg void OnCapture();
	afx_msg void OnStartOrStopRecord();
	afx_msg void OnVideoConfig();
	afx_msg void OnConfigExit();
};
