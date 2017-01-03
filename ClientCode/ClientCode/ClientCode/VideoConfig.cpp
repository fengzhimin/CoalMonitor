// VideoConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientCode.h"
#include "VideoConfig.h"
#include "afxdialogex.h"
#include "Log.h"

extern Log m_log;
// CVideoConfig 对话框

IMPLEMENT_DYNAMIC(CVideoConfig, CDialogEx)

CVideoConfig::CVideoConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoConfig::IDD, pParent)
	, m_port_string(_T(""))
	, m_user_string(_T(""))
	, m_pwd_string(_T(""))
	, m_picturePath_string(_T(""))
	, m_videoPath_string(_T(""))
	, m_coJpgQuality_string(_T(""))
	, m_coJpgSize_string(_T(""))
{
	m_font_static.CreatePointFont(160, L"宋体");
	m_font_edit.CreatePointFont(150, L"宋体");
	m_font_button.CreatePointFont(190, L"宋体");
}

CVideoConfig::~CVideoConfig()
{
	m_font_static.DeleteObject();
	m_font_edit.DeleteObject();
	m_font_button.DeleteObject();
}

void CVideoConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAMERA_VIDEOCONDIF_COMBO, m_camera_control);
	DDX_Control(pDX, IDC_PICTURETYPE_VIDEOCONDIF_COMBO, m_pictureTyoe_control);
	DDX_Control(pDX, IDC_IP_VIDEOCONDIF_IPADDRESS, m_ip_control);
	DDX_Text(pDX, IDC_PORT_VIDEOCONDIF_EDIT, m_port_string);
	DDX_Text(pDX, IDC_USER_VIDEOCONDIF_EDIT, m_user_string);
	DDX_Text(pDX, IDC_PWD_VIDEOCONDIF_EDIT, m_pwd_string);
	DDX_Text(pDX, IDC_BROWSEPICTUREPATH_VIDEOCONDIF_EDIT, m_picturePath_string);
	DDX_Text(pDX, IDC_BROWSEVIDEOPATH_VIDEOCONDIF_EDIT, m_videoPath_string);
	DDX_CBString(pDX, IDC_JPG_QUALITY_COMBO, m_coJpgQuality_string);
	DDX_CBString(pDX, IDC_JPG_SIZE_COMBO, m_coJpgSize_string);
}


BEGIN_MESSAGE_MAP(CVideoConfig, CDialogEx)
	ON_CBN_SELCHANGE(IDC_CAMERA_VIDEOCONDIF_COMBO, &CVideoConfig::OnCbnSelchangeCameraVideocondifCombo)
	ON_BN_CLICKED(IDC_BROWSEPICTUREPATH_VIDEOCONDIF_BUTTON, &CVideoConfig::OnBnClickedBrowsepicturepathVideocondifButton)
	ON_BN_CLICKED(IDC_BROWSEVIDEOPATH_VIDEOCONDIF_BUTTON, &CVideoConfig::OnBnClickedBrowsevideopathVideocondifButton)
	ON_BN_CLICKED(IDC_CONFIRM_VIDEOCONDIF_BUTTON, &CVideoConfig::OnBnClickedConfirmVideocondifButton)
	ON_BN_CLICKED(IDC_RESET_VIDEOCONDIF_BUTTON, &CVideoConfig::OnBnClickedResetVideocondifButton)
	ON_BN_CLICKED(IDC_CANCEL_VIDEOCONDIF_BUTTON, &CVideoConfig::OnBnClickedCancelVideocondifButton)
	ON_CBN_SELCHANGE(IDC_PICTURETYPE_VIDEOCONDIF_COMBO, &CVideoConfig::OnCbnSelchangePicturetypeVideocondifCombo)
END_MESSAGE_MAP()


BOOL CVideoConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//设置字体
	GetDlgItem(IDC_SELCAMERA_VIDEOCONDIF_STATIC)->SetFont(&m_font_static);
	GetDlgItem(IDC_CAMERA_VIDEOCONDIF_STATIC)->SetFont(&m_font_edit);
	GetDlgItem(IDC_PICTURECONFIG_VIDEOCONDIF_STATIC)->SetFont(&m_font_static);
	GetDlgItem(IDC_PICTURETYPE_VIDEOCONDIF_STATIC)->SetFont(&m_font_edit);
	GetDlgItem(IDC_CAMERAINFO_VIDEOCONDIF_STATIC)->SetFont(&m_font_static);
	GetDlgItem(IDC_JPGE_STATIC)->SetFont(&m_font_static);
	GetDlgItem(IDC_IP_VIDEOCONDIF_STATIC)->SetFont(&m_font_edit);
	GetDlgItem(IDC_USER_VIDEOCONDIF_STATIC)->SetFont(&m_font_edit);
	GetDlgItem(IDC_BROWSEPICTUREPATH_VIDEOCONDIF_STATIC)->SetFont(&m_font_edit);
	GetDlgItem(IDC_BROWSEVIDEOPATH_VIDEOCONDIF_STATIC)->SetFont(&m_font_edit);
	GetDlgItem(IDC_PORT_VIDEOCONDIF_STATIC)->SetFont(&m_font_edit);
	GetDlgItem(IDC_PWD_VIDEOCONDIF_STATIC)->SetFont(&m_font_edit);
	GetDlgItem(IDC_CAMERA_VIDEOCONDIF_COMBO)->SetFont(&m_font_edit);
	GetDlgItem(IDC_PICTURETYPE_VIDEOCONDIF_COMBO)->SetFont(&m_font_edit);
	GetDlgItem(IDC_JPG_SIZE_COMBO)->SetFont(&m_font_edit);
	GetDlgItem(IDC_JPG_QUALITY_COMBO)->SetFont(&m_font_edit);
	GetDlgItem(IDC_IP_VIDEOCONDIF_IPADDRESS)->SetFont(&m_font_edit);
	GetDlgItem(IDC_PORT_VIDEOCONDIF_EDIT)->SetFont(&m_font_edit);
	GetDlgItem(IDC_USER_VIDEOCONDIF_EDIT)->SetFont(&m_font_edit);
	GetDlgItem(IDC_PWD_VIDEOCONDIF_EDIT)->SetFont(&m_font_edit);
	GetDlgItem(IDC_BROWSEPICTUREPATH_VIDEOCONDIF_EDIT)->SetFont(&m_font_edit);
	GetDlgItem(IDC_BROWSEVIDEOPATH_VIDEOCONDIF_EDIT)->SetFont(&m_font_edit);
	GetDlgItem(IDC_BROWSEPICTUREPATH_VIDEOCONDIF_BUTTON)->SetFont(&m_font_static);
	GetDlgItem(IDC_BROWSEVIDEOPATH_VIDEOCONDIF_BUTTON)->SetFont(&m_font_static);
	GetDlgItem(IDC_CONFIRM_VIDEOCONDIF_BUTTON)->SetFont(&m_font_button);
	GetDlgItem(IDC_RESET_VIDEOCONDIF_BUTTON)->SetFont(&m_font_button);
	GetDlgItem(IDC_CANCEL_VIDEOCONDIF_BUTTON)->SetFont(&m_font_button);

	m_camera_control.SetCurSel(m_camera_control.FindString(0, L"摄像头-上"));

	ifFind = finder.FindFile(_T("./Config.ini"));
	if( ifFind )  
	{  
		CString _temp = L"";
		::GetPrivateProfileStringW(_T("Video Config Info"),_T("PicturePath"),_T(""),m_picturePath_string.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
		m_picturePath_string.ReleaseBuffer();
		::GetPrivateProfileStringW(_T("Video Config Info"),_T("VideoPath"),_T(""),m_videoPath_string.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		m_videoPath_string.ReleaseBuffer();
		::GetPrivateProfileStringW(_T("Video Config Info"),_T("JPG_QUALITY"),_T(""),m_coJpgQuality_string.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));   
		m_coJpgQuality_string.ReleaseBuffer();
		::GetPrivateProfileStringW(_T("Video Config Info"),_T("JPG_SIZE"),_T(""),m_coJpgSize_string.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));   
		m_coJpgSize_string.ReleaseBuffer();
		::GetPrivateProfileStringW(_T("Video Config Info"),_T("PictureMode"),_T(""),_temp.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		_temp.ReleaseBuffer();
		if(_temp == L"")
			m_pictureTyoe_control.SetCurSel(0);
		else
		{
			if(_temp == L"JPGE")
				m_pictureTyoe_control.SetCurSel(m_pictureTyoe_control.FindString(0, L"JPGE"));
			else
			{
				GetDlgItem(IDC_JPGE_STATIC)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_JPG_SIZE_COMBO)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_JPG_QUALITY_COMBO)->ShowWindow(SW_HIDE);
				m_pictureTyoe_control.SetCurSel(m_pictureTyoe_control.FindString(0, L"BMP"));
			}
		}

		UpdateData(FALSE);
	}
	else
		m_log.WriteErrorLog(L"读取配置文件错误!");

	ReadVideoConfig(m_camera_control.GetCurSel() + 1);

	return TRUE;  // return TRUE unless you set the focus to a control

}

void CVideoConfig::ReadVideoConfig(int _camera)
{
	CString _temp = L"";
	CString _IP = L"IP";
	CString _Port = L"Port";
	CString _User = L"User";
	CString _Pwd = L"Pwd";
	_temp.Format(L"%d", _camera);
	_IP += _temp;
	_Port += _temp;
	_User += _temp;
	_Pwd += _temp;
	ifFind = finder.FindFile(_T("./Config.ini"));
	if( ifFind )  
	{  
		CString _temp = L"";
		::GetPrivateProfileStringW(_T("Video Config Info"),_IP,_T(""),_temp.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		_temp.ReleaseBuffer();
		if(_temp != L"")
		{
			int _index = _temp.Find(L".");
			int _nField0 = _ttoi(_temp.Left(_index));
			_temp = _temp.Right(_temp.GetLength()-_index-1);
			_index = _temp.Find(L".");
			int _nField1 = _ttoi(_temp.Left(_index));
			_temp = _temp.Right(_temp.GetLength()-_index-1);
			_index = _temp.Find(L".");
			int _nField2 = _ttoi(_temp.Left(_index));
			int _nField3 = _ttoi(_temp.Right(_temp.GetLength()-_index-1));
			m_ip_control.SetAddress(_nField0, _nField1, _nField2, _nField3);
		}
		::GetPrivateProfileStringW(_T("Video Config Info"),_Port,_T(""),m_port_string.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		::GetPrivateProfileStringW(_T("Video Config Info"),_User,_T(""),m_user_string.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		::GetPrivateProfileStringW(_T("Video Config Info"),_Pwd,_T(""),m_pwd_string.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini")); 

		UpdateData(FALSE);
	} 
}

int CVideoConfig::WriteVideoConfig(int _camera)
{
	UpdateData();
	CString DeviceIp;
	BYTE nField0,nField1,nField2,nField3; 
	m_ip_control.GetAddress(nField0,nField1,nField2,nField3);
	DeviceIp.Format(L"%d.%d.%d.%d",nField0,nField1,nField2,nField3);
	if(nField0 == 0 || nField1 == 0 || nField2 == 0 || nField3 == 0)
		return 1;
	m_port_string.Trim();
	m_user_string.Trim();
	m_pwd_string.Trim();
	if(m_port_string == L"")
		return 2;
	if(m_user_string == L"")
		return 3;
	if(m_pwd_string == L"")
		return 4;
	CString _temp = L"";
	CString _IP = L"IP";
	CString _Port = L"Port";
	CString _User = L"User";
	CString _Pwd = L"Pwd";
	_temp.Format(L"%d", _camera);
	_IP += _temp;
	_Port += _temp;
	_User += _temp;
	_Pwd += _temp;
	::WritePrivateProfileStringW(_T("Video Config Info"),_IP,DeviceIp,_T("./Config.ini"));  
	::WritePrivateProfileStringW(_T("Video Config Info"),_Port,m_port_string,_T("./Config.ini"));  
	::WritePrivateProfileStringW(_T("Video Config Info"),_User,m_user_string,_T("./Config.ini"));  
	::WritePrivateProfileStringW(_T("Video Config Info"),_Pwd,m_pwd_string,_T("./Config.ini")); 
	
	return 0;
}

CString CVideoConfig::GetSelectFolderPath()
{
	BROWSEINFO bi;  
	ZeroMemory(&bi, sizeof(BROWSEINFO));  
	bi.hwndOwner = m_hWnd;  
	bi.ulFlags   = BIF_RETURNONLYFSDIRS;  
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  
	BOOL bRet = FALSE;  
	TCHAR szFolder[MAX_PATH*2];  
	szFolder[0] = _T('\0');  
	if (pidl)  
	{  
		if (SHGetPathFromIDList(pidl, szFolder))    
			bRet = TRUE;  
		IMalloc *pMalloc = NULL;  
		if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc)  
		{   
			pMalloc->Free(pidl);   
			pMalloc->Release(); 
			return szFolder;
		}  
	}  

	return L"";
}

void CVideoConfig::OnCbnSelchangeCameraVideocondifCombo()
{
	ReadVideoConfig(m_camera_control.GetCurSel() + 1);
}

void CVideoConfig::OnBnClickedBrowsepicturepathVideocondifButton()
{
	CString _ret = GetSelectFolderPath();
	if(_ret != L"")
		m_picturePath_string = _ret;//选择的文件夹路径
	UpdateData(FALSE);
}


void CVideoConfig::OnBnClickedBrowsevideopathVideocondifButton()
{
	CString _ret = GetSelectFolderPath();
	if(_ret != L"")
		m_videoPath_string = _ret;//选择的文件夹路径
	UpdateData(FALSE);
}


void CVideoConfig::OnBnClickedConfirmVideocondifButton()
{
	UpdateData();
	switch(WriteVideoConfig(m_camera_control.GetCurSel() + 1))
	{
	case 1:
		MessageBox(L"IP不能为空！");
		return ;
	case 2:
		MessageBox(L"端口不能为空！");
		return ;
	case 3:
		MessageBox(L"用户名不能为空！");
		return ;
	case 4:
		MessageBox(L"密码不能为空！");
		return ;
	default:
		break;
	}

	m_picturePath_string.Trim();
	if(m_picturePath_string == L"")
	{
		MessageBox(L"请输入照片存路径！");
		return ;
	}
	m_videoPath_string.Trim();
	if(m_videoPath_string == L"")
	{
		MessageBox(L"请输入录像存路径！");
		return ;
	}

	CString _temp;
	m_pictureTyoe_control.GetLBText(m_pictureTyoe_control.GetCurSel(), _temp);
	::WritePrivateProfileStringW(_T("Video Config Info"),_T("PictureMode"),_temp,_T("./Config.ini"));  
	::WritePrivateProfileStringW(_T("Video Config Info"),_T("PicturePath"),m_picturePath_string,_T("./Config.ini"));  
	::WritePrivateProfileStringW(_T("Video Config Info"),_T("VideoPath"),m_videoPath_string,_T("./Config.ini"));  
	::WritePrivateProfileStringW(_T("Video Config Info"),_T("JPG_SIZE"),m_coJpgSize_string,_T("./Config.ini"));  
	::WritePrivateProfileStringW(_T("Video Config Info"),_T("JPG_QUALITY"),m_coJpgQuality_string,_T("./Config.ini"));  
	MessageBox(L"保存成功！");
}


void CVideoConfig::OnBnClickedResetVideocondifButton()
{
	m_camera_control.SetCurSel(m_camera_control.FindString(0, L"摄像头-上"));

	ifFind = finder.FindFile(_T("./Config.ini"));
	if( ifFind )  
	{  
		CString _temp = L"";
		::GetPrivateProfileStringW(_T("Video Config Info"),_T("PicturePath"),_T(""),m_picturePath_string.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
		m_picturePath_string.ReleaseBuffer();
		::GetPrivateProfileStringW(_T("Video Config Info"),_T("VideoPath"),_T(""),m_videoPath_string.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		m_videoPath_string.ReleaseBuffer();
		::GetPrivateProfileStringW(_T("Video Config Info"),_T("JPG_QUALITY"),_T(""),m_coJpgQuality_string.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));   
		m_coJpgQuality_string.ReleaseBuffer();
		::GetPrivateProfileStringW(_T("Video Config Info"),_T("JPG_SIZE"),_T(""),m_coJpgSize_string.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));   
		m_coJpgSize_string.ReleaseBuffer();
		::GetPrivateProfileStringW(_T("Video Config Info"),_T("PictureMode"),_T(""),_temp.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		_temp.ReleaseBuffer();
		if(_temp == L"")
			m_pictureTyoe_control.SetCurSel(0);
		else
		{
			if(_temp == L"JPGE")
				m_pictureTyoe_control.SetCurSel(m_pictureTyoe_control.FindString(0, L"JPGE"));
			else
			{
				GetDlgItem(IDC_JPGE_STATIC)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_JPG_SIZE_COMBO)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_JPG_QUALITY_COMBO)->ShowWindow(SW_HIDE);
				m_pictureTyoe_control.SetCurSel(m_pictureTyoe_control.FindString(0, L"BMP"));
			}
		}

		UpdateData(FALSE);
	} 

	ReadVideoConfig(m_camera_control.GetCurSel() + 1);
}


void CVideoConfig::OnBnClickedCancelVideocondifButton()
{
	CDialog::OnCancel();
}



void CVideoConfig::OnCbnSelchangePicturetypeVideocondifCombo()
{
	if(m_pictureTyoe_control.GetCurSel() == m_pictureTyoe_control.FindString(0, L"JPGE"))
	{
		GetDlgItem(IDC_JPGE_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_JPG_SIZE_COMBO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_JPG_QUALITY_COMBO)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_JPGE_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JPG_SIZE_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_JPG_QUALITY_COMBO)->ShowWindow(SW_HIDE);
	}
}

