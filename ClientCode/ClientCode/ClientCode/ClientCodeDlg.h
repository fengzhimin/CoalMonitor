
#include "Serial.h"
#include "DeviceOperation.h"
#include "DeviceThreadParameter.h"
#include "OperateDataBase.h"

#pragma once

// CClientCodeDlg 对话框
class CClientCodeDlg : public CDialogEx
{
// 构造
public:
	CClientCodeDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CClientCodeDlg();
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
	struct DeviceThreadParameter m_devThreadParameter;
	CFont m_newFont;   // 新字体   
	CFont *m_pOldFont; // 选择新字体之前的字体  
	CRect m_ClientRect;   //客户端的大小
	CFont m_font_static, m_font_edit;
	int m_nTextX_Down;   //下面水平滚动文本的起点的x坐标  
	bool m_isConfirmNoCarCard;   //标示是否点击了没有车卡的原因 
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
	CString m_style_string;   //货物类型
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
