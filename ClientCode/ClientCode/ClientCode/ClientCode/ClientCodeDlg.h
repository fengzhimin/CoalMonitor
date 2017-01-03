
#include "Serial.h"
#include "DeviceOperation.h"

#pragma once

// CClientCodeDlg �Ի���
class CClientCodeDlg : public CDialogEx
{
// ����
public:
	CClientCodeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLIENTCODE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
//	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CMenu m_login_menu, m_logout_menu;
	CSerial *m_serial;
	CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����  
	BOOL ifFind;
	BOOL m_bIsLogin;    //�Ƿ��¼����ͷ
	BOOL m_bIsPlaying;  //�Ƿ�ʼ��������ͷ
	BOOL m_bIsRecording;//�Ƿ�ʼ¼��
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
