
// CallBackCarCardDlg.h : 头文件
#include "SerialPort.h"
#include "Log.h"

#pragma once


// CCallBackCarCardDlg 对话框
class CCallBackCarCardDlg : public CDialogEx
{
// 构造
public:
	CCallBackCarCardDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CCallBackCarCardDlg();
// 对话框数据
	enum { IDD = IDD_CALLBACKCARCARD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	int m_CallBackCardComm;   //收卡机的串口
	int m_CallBackCardBaudRate;  //收卡机的串口波特率
	int m_CallBackCardRFIDComm;  //收卡机的RFID设备串口
	int m_CallBackCardRFIDBaudRate;  //收卡机的RFID设备串口波特率
	CString m_skin;   //皮肤
	BOOL m_receiveCallBackCarCardCommand;   //开始接受收卡机查询状态
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
