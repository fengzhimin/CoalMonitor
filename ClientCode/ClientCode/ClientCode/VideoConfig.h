#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CVideoConfig �Ի���

class CVideoConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoConfig)

public:
	CVideoConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVideoConfig();
	virtual BOOL OnInitDialog();
// �Ի�������
	enum { IDD = IDD_VIDEOCONFIG_DIALOG };
private:
	CFont m_font_static, m_font_edit, m_font_button;
	CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����  
	BOOL ifFind;  
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	CString GetSelectFolderPath();   //��ȡѡ����ļ���·��
	void ReadVideoConfig(int _camera);   //��ȡĳһ������ͷ�������ļ�  _cameraΪ����ͷ���1��2��3��4
	//0���ɹ� 1��IPΪ�� 2��portΪ��  3��UserΪ�� 4��PwdΪ��
	int WriteVideoConfig(int _camera);   
	afx_msg void OnCbnSelchangePicturetypeVideocondifCombo();
	CString m_coJpgQuality_string;
	CString m_coJpgSize_string;
};
