#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CVideoConfig 对话框

class CVideoConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoConfig)

public:
	CVideoConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoConfig();
	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_VIDEOCONFIG_DIALOG };
private:
	CFont m_font_static, m_font_edit, m_font_button;
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
	BOOL ifFind;  
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_camera_control;
	CComboBox m_pictureTyoe_control;
	CIPAddressCtrl m_ip_control;
	CString m_port_string;
	CString m_user_string;
	CString m_pwd_string;
	CString m_picturePath_string;
	CString m_videoPath_string;
	afx_msg void OnCbnSelchangeCameraVideocondifCombo();
	afx_msg void OnBnClickedBrowsepicturepathVideocondifButton();
	afx_msg void OnBnClickedBrowsevideopathVideocondifButton();
	afx_msg void OnBnClickedConfirmVideocondifButton();
	afx_msg void OnBnClickedResetVideocondifButton();
	afx_msg void OnBnClickedCancelVideocondifButton();
	CString GetSelectFolderPath();   //获取选择的文件夹路径
	void ReadVideoConfig(int _camera);   //读取某一个摄像头的配置文件  _camera为摄像头编号1、2、3、4
	//0：成功 1：IP为空 2：port为空  3：User为空 4：Pwd为空
	int WriteVideoConfig(int _camera);   
	afx_msg void OnCbnSelchangePicturetypeVideocondifCombo();
	CString m_coJpgQuality_string;
	CString m_coJpgSize_string;
};
