
// MonitorCodeDlg.h : ͷ�ļ�
//
#include "GeneralDef.h"
#include "Log.h"
#pragma once


// CMonitorCodeDlg �Ի���
class CMonitorCodeDlg : public CDialogEx
{
// ����
public:
	CMonitorCodeDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CMonitorCodeDlg();
// �Ի�������
	enum { IDD = IDD_MONITORCODE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	void ReSize();  
	POINT old;
	DWORD  m_threadID;
	HANDLE hThread;
	CString m_videoIP[5], m_videoUser[5], m_videoPwd[5];
	int m_videoPort[5];
	NET_DVR_CLIENTINFO ClientInfo[5];
	LOCAL_DEVICE_INFO m_videoDeviceInfo[5];   //����ͷ�豸
	int m_iCurChanIndex[5];               //��ǰͨ��������������
	LONG m_lPlayHandle[5];
	HWND hWnd[5];       //ÿ����̬�ؼ��ľ��
	CRect m_rcWnd[5];   //ÿ����̬�ؼ��Ĵ�С
	CRect m_rcScreen;   //���
	BOOL m_bFullScreen[5];  
	int m_nTextX_Down;   //����ˮƽ�����ı�������x����  
	CFont m_newFont;   // ������   
	CFont *m_pOldFont; // ѡ��������֮ǰ������  
	CRect m_ClientRect;   //�ͻ��˵Ĵ�С
	CFont m_font_static, m_font_edit;
// ʵ��
protected:
	HICON m_hIcon;
	
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLoadconfig();
	//��¼����ͷ
	bool LoginVideo(int _camera);
	//�˳�����ͷ
	bool LogoutVideo(int _camera);
	//��ʼһ·����
	bool StartPlay(int _camera);
	//ֹͣ����
	bool StopPlay(int _camera);
	//���
	void ShowFullScreen(int _videoID, int _carmera);
	//�ָ�����
	void ShowNormal(int _videoID, int _carmera);
	afx_msg void OnStnDblclickVideo4Static();
	afx_msg void OnStnDblclickVideo1Static();
	afx_msg void OnStnDblclickVideo2Static();
	afx_msg void OnStnDblclickVideo3Static();
	afx_msg void OnStnDblclickVideo5Static();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CComboBox m_select_control;
	CString m_cargoSize_string;
	CString m_carCargoSize_string;
	CString m_cargoWeight_string;
	CString m_weightPlace_string;
	CString m_inOut_string;
	afx_msg void OnCbnSelchangeSelectCombo();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	LRESULT OnUpdateMyData(WPARAM wParam, LPARAM lParam);
	LRESULT OnEnableButton(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedDetailButton();
};
