
#include "Serial.h"
#include "DeviceOperation.h"
#include "DeviceThreadParameter.h"
#include "OperateDataBase.h"

#pragma once

// CClientCodeDlg �Ի���
class CClientCodeDlg : public CDialogEx
{
// ����
public:
	CClientCodeDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CClientCodeDlg();
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
	struct DeviceThreadParameter m_devThreadParameter;
	CFont m_newFont;   // ������   
	CFont *m_pOldFont; // ѡ��������֮ǰ������  
	CRect m_ClientRect;   //�ͻ��˵Ĵ�С
	CFont m_font_static, m_font_edit;
	int m_nTextX_Down;   //����ˮƽ�����ı�������x����  
	bool m_isConfirmNoCarCard;   //��ʾ�Ƿ�����û�г�����ԭ�� 
	int m_reasonNumber;
public:
	void ReSize();  
	POINT old;
	afx_msg void OnLogin();
	afx_msg void OnAbout();
	afx_msg void OnConfig();
	afx_msg void OnClose();
	afx_msg void OnLogout();
	afx_msg void OnReadcarcard();
	afx_msg void OnWritecarcard();
	afx_msg void OnSerial();
	afx_msg void OnParaset();
	afx_msg void OnLoginOrLogout();
	afx_msg void OnStartOrStop();
	virtual BOOL OnInitDialog();
	afx_msg void OnCapture();
	afx_msg void OnStartOrStopRecord();
	afx_msg void OnVideoConfig();
	afx_msg void OnConfigExit();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT OnUpdate(WPARAM wParam, LPARAM lParam);
	CString m_style_string;   //��������
	CString m_carCardID_string;
	CString m_emptyCarSize_string;
	CString m_cargoSize_string;
	CString m_cargoCarSize_string;
	CString m_emptyCarWeight_string;
	CString m_carWeight_string;
	CString m_cargoWeight_string;
	CString m_cargoDensity_string;
	CString m_deductWeight_string;
	CString m_weightPlace_string;
	CString m_abnormal_string;
	CString m_inOut_string;
	CString m_recordTime_string;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedNocardButton();
	CString m_agreementA_string;
	CString m_agreementB_string;
	bool m_reason1;
	bool m_reason2;
	bool m_reason3;
	bool m_reason4;
	bool m_reason5;
};
