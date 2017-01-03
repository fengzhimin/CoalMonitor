
// SendCarCardDlg.h : ͷ�ļ�
//
#include "SerialPort.h"
#include "Log.h"
#pragma once


// CSendCarCardDlg �Ի���
class CSendCarCardDlg : public CDialogEx
{
// ����
public:
	CSendCarCardDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CSendCarCardDlg();

// �Ի�������
	enum { IDD = IDD_SENDCARCARD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnReceiveChar(UINT ch, LONG port);
	DECLARE_MESSAGE_MAP()

public:
	BOOL m_receiveSendCarCardCommand;   //��ʼ���ܷ�������ѯ״̬
	//BOOL m_receiveCallBackCarCardCommand;   //��ʼ�����տ�����ѯ״̬
	afx_msg void OnBnClickedTextButton();
	CString DevideHexChar(char HexChar);
	static char CombineHexChar(char CharH,char CharL);
	static void HexStringFilter(CString &str);
	static CString ChangeCharstr2Hexstr(CString Charstr);
	afx_msg void OnBnClickedStopButton();
	CString m_merchantID;
	CComboBox m_agreementID;
	CString m_strAgreementID;
	CFont m_font_static, m_font_edit, m_font_button;
	int m_nTextX_Up;   // ����ˮƽ�����ı�����ʼ���x����   
	int m_nTextX_Down;   //����ˮƽ�����ı�������x����  
	CFont m_newFont;   // ������   
	CFont *m_pOldFont; // ѡ��������֮ǰ������  
	CRect m_ClientRect;   //�ͻ��˵Ĵ�С
	int m_SendCardComm, m_CallBackCardComm;   //���������տ����Ĵ���
	int m_SendCardBaudRate, m_CallBackCardBaudRate;  //���������տ����Ĵ��ڲ�����
	int m_SendCardRFIDComm, m_CallBackCardRFIDComm;  //���������տ�����RFID�豸����
	int m_SendCardRFIDBaudRate, m_CallBackCardRFIDBaudRate;  //���������տ�����RFID�豸���ڲ�����
	CString m_skin;   //Ƥ��
	afx_msg void OnBnClickedStartButton();
	LRESULT OnUpdateMyData(WPARAM wParam, LPARAM lParam);  
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void CSendCarCardDlg::ReSize();
	POINT old;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
