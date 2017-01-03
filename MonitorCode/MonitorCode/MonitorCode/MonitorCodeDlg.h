
// MonitorCodeDlg.h : 头文件
//
#include "GeneralDef.h"
#include "Log.h"
#pragma once


// CMonitorCodeDlg 对话框
class CMonitorCodeDlg : public CDialogEx
{
// 构造
public:
	CMonitorCodeDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CMonitorCodeDlg();
// 对话框数据
	enum { IDD = IDD_MONITORCODE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	void ReSize();  
	POINT old;
	DWORD  m_threadID;
	HANDLE hThread;
	CString m_videoIP[5], m_videoUser[5], m_videoPwd[5];
	int m_videoPort[5];
	NET_DVR_CLIENTINFO ClientInfo[5];
	LOCAL_DEVICE_INFO m_videoDeviceInfo[5];   //摄像头设备
	int m_iCurChanIndex[5];               //当前通道在数组中索引
	LONG m_lPlayHandle[5];
	HWND hWnd[5];       //每个静态控件的句柄
	CRect m_rcWnd[5];   //每个静态控件的大小
	CRect m_rcScreen;   //最大化
	BOOL m_bFullScreen[5];  
	int m_nTextX_Down;   //下面水平滚动文本的起点的x坐标  
	CFont m_newFont;   // 新字体   
	CFont *m_pOldFont; // 选择新字体之前的字体  
	CRect m_ClientRect;   //客户端的大小
	CFont m_font_static, m_font_edit;
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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLoadconfig();
	//登录摄像头
	bool LoginVideo(int _camera);
	//退出摄像头
	bool LogoutVideo(int _camera);
	//开始一路播放
	bool StartPlay(int _camera);
	//停止播放
	bool StopPlay(int _camera);
	//最大化
	void ShowFullScreen(int _videoID, int _carmera);
	//恢复正常
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
