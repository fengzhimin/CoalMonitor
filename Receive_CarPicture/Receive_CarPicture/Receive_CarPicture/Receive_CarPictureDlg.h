
// Receive_CarPictureDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CReceive_CarPictureDlg �Ի���
class CReceive_CarPictureDlg : public CDialogEx
{
// ����
public:
	CReceive_CarPictureDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CReceive_CarPictureDlg();
// �Ի�������
	enum { IDD = IDD_RECEIVE_CARPICTURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	CString m_skin;  //Ƥ��
	int m_nSend;
	afx_msg void OnBnClickedResetButton();
	afx_msg void OnBnClickedBrowseButton();
	afx_msg void OnBnClickedSendButton();
	LRESULT OnUpdateSocketInfo(WPARAM wParam, LPARAM lParam);  
};
