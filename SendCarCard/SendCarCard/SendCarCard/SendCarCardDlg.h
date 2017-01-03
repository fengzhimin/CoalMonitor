
// SendCarCardDlg.h : 头文件
//
#include "SerialPort.h"
#include "Log.h"
#pragma once


// CSendCarCardDlg 对话框
class CSendCarCardDlg : public CDialogEx
{
// 构造
public:
	CSendCarCardDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CSendCarCardDlg();

// 对话框数据
	enum { IDD = IDD_SENDCARCARD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnReceiveChar(UINT ch, LONG port);
	DECLARE_MESSAGE_MAP()

public:
	BOOL m_receiveSendCarCardCommand;   //开始接受发卡机查询状态
	//BOOL m_receiveCallBackCarCardCommand;   //开始接受收卡机查询状态
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
	int m_nTextX_Up;   // 上面水平滚动文本的起始点的x坐标   
	int m_nTextX_Down;   //下面水平滚动文本的起点的x坐标  
	CFont m_newFont;   // 新字体   
	CFont *m_pOldFont; // 选择新字体之前的字体  
	CRect m_ClientRect;   //客户端的大小
	int m_SendCardComm, m_CallBackCardComm;   //发卡机和收卡机的串口
	int m_SendCardBaudRate, m_CallBackCardBaudRate;  //发卡机和收卡机的串口波特率
	int m_SendCardRFIDComm, m_CallBackCardRFIDComm;  //发卡机和收卡机的RFID设备串口
	int m_SendCardRFIDBaudRate, m_CallBackCardRFIDBaudRate;  //发卡机和收卡机的RFID设备串口波特率
	CString m_skin;   //皮肤
	afx_msg void OnBnClickedStartButton();
	LRESULT OnUpdateMyData(WPARAM wParam, LPARAM lParam);  
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void CSendCarCardDlg::ReSize();
	POINT old;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
