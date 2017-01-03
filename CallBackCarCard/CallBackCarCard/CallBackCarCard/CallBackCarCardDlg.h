
// CallBackCarCardDlg.h : ͷ�ļ�
#include "SerialPort.h"
#include "Log.h"

#pragma once


// CCallBackCarCardDlg �Ի���
class CCallBackCarCardDlg : public CDialogEx
{
// ����
public:
	CCallBackCarCardDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CCallBackCarCardDlg();
// �Ի�������
	enum { IDD = IDD_CALLBACKCARCARD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	int m_CallBackCardComm;   //�տ����Ĵ���
	int m_CallBackCardBaudRate;  //�տ����Ĵ��ڲ�����
	int m_CallBackCardRFIDComm;  //�տ�����RFID�豸����
	int m_CallBackCardRFIDBaudRate;  //�տ�����RFID�豸���ڲ�����
	CString m_skin;   //Ƥ��
	BOOL m_receiveCallBackCarCardCommand;   //��ʼ�����տ�����ѯ״̬
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
	afx_msg void OnBnClickedStartButton();
	afx_msg void OnBnClickedStopButton();
	static char CombineHexChar(char CharH,char CharL);
	static void HexStringFilter(CString &str);
	CString DevideHexChar(char HexChar);
	static CString ChangeCharstr2Hexstr(CString Charstr);
	afx_msg LRESULT OnReceiveChar(UINT ch, LONG port);
};
