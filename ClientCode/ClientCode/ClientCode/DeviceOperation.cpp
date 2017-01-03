#include "stdafx.h"
#include "DeviceOperation.h"
#include "CarCardInfo.h"
#include "HCNetSDK.h"
#include "Log.h"
#pragma comment(lib, "HCNetSDK.lib")
#include "..//dll/disdll.h"
#pragma comment(lib, "..//dll/disdll.lib")

#define SEND_DATA_BUF_SIZE 0x400   //1024(数据块)
#define SEND_INFO_BUF_SIZE 0x12    //18(发送信息)

CString m_Receive_IP;   //接受图片的IP
int m_Receive_Port;     //接受图片的端口
CString m_picturePath;  //图片的存放路径
extern Log m_log;
static DWORD WINAPI  ReadCarCardThread(void *pArg);
static DWORD WINAPI  SendPicThread(void *pArg);

CDeviceOperation::CDeviceOperation(void)
{
	for(int i = 0; i < 5; i++)
	{
		m_iCurChanIndex[i] = -1;
		m_lPlayHandle[i] = -1;
	}
	ifFind = finder.FindFile(_T("./Config.ini"));
}


CDeviceOperation::~CDeviceOperation(void)
{

}

bool CDeviceOperation::Init()
{
	if( ifFind )  
	{  
		CString _temp = L"";
		//读取读卡设备配置信息
		::GetPrivateProfileStringW(_T("SerialCom Config Info"),_T("Com"),_T("com1"),_temp.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
		_temp.ReleaseBuffer();  
		m_serialCom = _temp.GetAt(3) - '0';
		::GetPrivateProfileStringW(_T("BaudRate Config Info"),_T("BaudRate"),_T("9600"),_temp.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
		_temp.ReleaseBuffer();  
		m_baudRate = _ttoi(_temp);
		//读取摄像头配置信息
		for(int i = 0; i < 5; i++)
			ReadVideoConfig(i);
		::GetPrivateProfileStringW(_T("Video Config Info"),_T("PicturePath"),_T(""),m_picturePath.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
		m_picturePath.ReleaseBuffer();
		m_picturePath.Replace(L"\\", L"/");
		::GetPrivateProfileStringW(_T("Video Config Info"),_T("VideoPath"),_T(""),m_videoPath.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
		m_videoPath.ReleaseBuffer();
		m_videoPath.Replace(L"\\", L"/");
		::GetPrivateProfileStringW(_T("Video Config Info"),_T("PictureMode"),_T(""),m_pictureType.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
		m_pictureType.ReleaseBuffer();
		::GetPrivateProfileStringW(_T("Video Config Info"),_T("JPG_SIZE"),_T(""),m_pictureSize.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
		m_pictureSize.ReleaseBuffer();
		::GetPrivateProfileStringW(_T("Video Config Info"),_T("JPG_QUALITY"),_T(""),m_pictureQuality.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
		m_pictureQuality.ReleaseBuffer();
		//发送图片的服务器地址
		::GetPrivateProfileStringW(_T("NetWork Config Info"),_T("Receive_Pic_IP"),_T(""),m_Receive_IP.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
		m_Receive_IP.ReleaseBuffer();
		::GetPrivateProfileStringW(_T("NetWork Config Info"),_T("Receive_Pic_Port"),_T(""),_temp.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
		_temp.ReleaseBuffer();
		m_Receive_Port = _ttoi(_temp);
	}
	else
	{
		m_log.WriteErrorLog(L"读取配置文件错误!");
		m_serialCom = 1;
		m_baudRate = 9600;
	}

	if (!DeviceInit("", m_serialCom, m_baudRate))
	{
		MessageBox(NULL,L"读卡器串口初始化失败！", L"错误",MB_OK); 
		m_log.WriteErrorLog(L"读卡器串口初始化失败！");
		return false;
	}

	if (!DeviceConnect())
	{
		MessageBox(NULL,L"读卡器串口连接失败！", L"错误",MB_OK); 
		m_log.WriteErrorLog(L"读卡器串口连接失败！");
		return false;
	}

	return true;
}

void CDeviceOperation::ReadVideoConfig(int _camera)
{
	if(_camera > 4)
		return ;   
	CString _temp = L"";
	CString _IP = L"IP";
	CString _Port = L"Port";
	CString _User = L"User";
	CString _Pwd = L"Pwd";
	_temp.Format(L"%d", _camera+1);
	_IP += _temp;
	_Port += _temp;
	_User += _temp;
	_Pwd += _temp;
	ifFind = finder.FindFile(_T("./Config.ini"));
	if( ifFind )  
	{  
		CString _temp = L"";
		::GetPrivateProfileStringW(_T("Video Config Info"),_IP,_T(""),m_videoIP[_camera].GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		m_videoIP[_camera].ReleaseBuffer();
		::GetPrivateProfileStringW(_T("Video Config Info"),_Port,_T(""),_temp.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		_temp.ReleaseBuffer();
		m_videoPort[_camera] = _ttoi(_temp);
		::GetPrivateProfileStringW(_T("Video Config Info"),_User,_T(""),m_videoUser[_camera].GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		m_videoUser[_camera].ReleaseBuffer();
		::GetPrivateProfileStringW(_T("Video Config Info"),_Pwd,_T(""),m_videoPwd[_camera].GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini")); 
		m_videoPwd[_camera].ReleaseBuffer();
	} 
}

void CDeviceOperation::ReadCarCardInfo(CDialogEx *_dialog, int _showID)
{
	m_carCardThreadParameter.m_dialog = _dialog;
	m_carCardThreadParameter.m_ShowID = _showID;
	m_carCardThreadParameter.m_carCardInfo = L"";
	m_carCardThreadParameter.m_isSuccess = false;
	m_carCardThreadParameter.m_noCarCard = false;
	//开启线程			
	hThread = CreateThread(NULL,0,ReadCarCardThread,&m_carCardThreadParameter,0,&m_readCarCardThreadID);  
	CloseHandle(hThread);
}

//全局函数
static DWORD WINAPI  ReadCarCardThread(void *pArg)
{
	CarCardThreadParameter *m_parameter = (struct CarCardThreadParameter*)pArg;
	int index;
	unsigned char begin = 2;
	unsigned char length = 6; // 选择项索引从0开始
	unsigned char buf[128] = {0};// 接收缓冲区	
	int readAgainRate;
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
	BOOL ifFind = finder.FindFile(_T("./Config.ini"));  
	if( ifFind )  
	{  
		CString _temp = L"";
		::GetPrivateProfileStringW(_T("ReadAgainRate Config Info"),_T("Rate"),_T("10"),_temp.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
		_temp.ReleaseBuffer();  
		readAgainRate = _ttoi(_temp);
	}
	else
		readAgainRate = 10;

	int _warn_count = 0;
	CVoice _voice;
	_voice.Initial();
	_voice.SpeakInfo(L"请刷卡！");
	while(1)
	{
		if ( ReadTagData(0,  1, begin, length, &buf[0]))
		{
			CString temp;
			m_parameter->m_carCardInfo = CString("");
			for(int i = 3; i < length *2 + 3; ++i) // 前3字节为传入的bank、begin、length
			{
				temp.Format(L"%02X ", buf[i]);
				m_parameter->m_carCardInfo += temp;
			}

			//m_parameter->m_dialog->SetDlgItemText(m_parameter->m_ShowID, m_parameter->m_carCardInfo);
			m_parameter->m_carCardInfo.Replace(L" ", L"");
			m_parameter->m_isSuccess = true;
			_voice.SpeakInfo(L"读卡成功!");
			m_log.WriteOperateLog(L"读卡成功! 卡号：" + m_parameter->m_carCardInfo);
			ExitThread(0);
		}
		else
		{
			if(_warn_count == readAgainRate)
			{
				if(m_parameter->m_noCarCard)
					ExitThread(0);   //表示没有车卡
				_voice.SpeakInfo(L"请重新刷卡！");
				m_parameter->m_isSuccess = false;
				//m_parameter->m_dialog->SetDlgItemText(m_parameter->m_ShowID, L"读卡失败！");
				_warn_count = 0;
			}
			else
				_warn_count++;
		}
	}	
}

void CDeviceOperation::CStringToChar(CString _cstring, char *_ch)
{
	int _length = _cstring.GetLength() + 1;
	for(int i = 0; i < _length; i++)
		_ch[i] = _cstring[i];
}

bool CDeviceOperation::LoginVideo(int _camera)
{
	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;
	memset(&DeviceInfoTmp,0,sizeof(NET_DVR_DEVICEINFO_V30));

	char _videoIP[15];
	CStringToChar(m_videoIP[_camera], _videoIP);
	char _videoUser[10];
	CStringToChar(m_videoUser[_camera], _videoUser);
	char _videoPwd[25];
	CStringToChar(m_videoPwd[_camera], _videoPwd);
	if(!NET_DVR_Init())
	{
		MessageBox(NULL,L"摄像头SDK初始化失败！", L"错误",MB_OK); 
		CString _error;
		_error.Format(L"%d", _camera);
		m_log.WriteErrorLog(L"摄像头 "+_error+L" SDK初始化失败！");
	}
	LONG lLoginID = NET_DVR_Login_V30(_videoIP, m_videoPort[_camera], _videoUser, _videoPwd, &DeviceInfoTmp);
	if(lLoginID == -1)
	{
		DWORD err=NET_DVR_GetLastError();
		CString m_csErr;
        m_csErr.Format(L"登录摄像头出错，错误代码%d",err);
		CString _error;
		_error.Format(L"%d", _camera);
		m_log.WriteErrorLog(L"摄像头 "+_error+L" 登录出错！");
		MessageBox(NULL, m_csErr, L"错误", MB_OK);
		return FALSE;
	}
    m_videoDeviceInfo[_camera].lLoginID = lLoginID;
	m_videoDeviceInfo[_camera].iDeviceChanNum = DeviceInfoTmp.byChanNum;
    m_videoDeviceInfo[_camera].iIPChanNum = DeviceInfoTmp.byIPChanNum;
    m_videoDeviceInfo[_camera].iStartChan  = DeviceInfoTmp.byStartChan;
    m_videoDeviceInfo[_camera].iIPStartChan  = DeviceInfoTmp.byStartDChan;

	m_iCurChanIndex[_camera] = DeviceInfoTmp.byStartChan;
	//设置时间
	CTime CurTime = CTime::GetCurrentTime();
	NET_DVR_TIME _curTime;
	_curTime.dwYear = CurTime.GetYear();
	_curTime.dwMonth = CurTime.GetMonth();
	_curTime.dwDay = CurTime.GetDay();
	_curTime.dwHour = CurTime.GetHour();
	_curTime.dwMinute = CurTime.GetMinute();
	_curTime.dwSecond = CurTime.GetSecond();
	if(!NET_DVR_SetDVRConfig(m_videoDeviceInfo[_camera].lLoginID, NET_DVR_SET_TIMECFG, NULL, &_curTime, sizeof(NET_DVR_TIME)))
	{
		DWORD err=NET_DVR_GetLastError();
		CString m_csErr;
        m_csErr.Format(L"设置时间出错，错误代码%d",err);
		CString _error;
		_error.Format(L"%d", _camera);
		m_log.WriteErrorLog(L"摄像头 "+_error+L" 设置时间出错！");
		MessageBox(NULL, m_csErr, L"错误", MB_OK);

		return FALSE;
	}

	return TRUE;
}

bool CDeviceOperation::LogoutVideo(int _camera)
{
	if(NET_DVR_Logout_V30(m_videoDeviceInfo[_camera].lLoginID))
	{
		NET_DVR_Cleanup();
		m_iCurChanIndex[_camera] = -1;
		return true;
	}
	else
		return false;
}

/*************************************************
函数名:    	StartPlay
函数描述:	开始一路播放
输入参数:   _camera标示那个摄像头			
返回值:		成功返回True  否则返回false
**************************************************/
bool CDeviceOperation::StartPlay(int _camera)
{
	ClientInfo[_camera].lChannel     = m_iCurChanIndex[_camera];
	ClientInfo[_camera].lLinkMode    = 0;
	ClientInfo[_camera].byProtoType = 0;
    ClientInfo[_camera].sMultiCastIP = NULL;
	TRACE("Channel number:%d\n",ClientInfo[_camera].lChannel);

	m_lPlayHandle[_camera] = NET_DVR_RealPlay_V30(m_videoDeviceInfo[_camera].lLoginID,&ClientInfo[_camera],NULL,NULL,TRUE);
	if(-1 == m_lPlayHandle[_camera])
	{
		DWORD err=NET_DVR_GetLastError();
		CString m_csErr;
        m_csErr.Format(L"播放出错，错误代码%d",err);
		CString _error;
		_error.Format(L"%d", _camera);
		m_log.WriteErrorLog(L"摄像头 "+_error+L" 播放出错！");
		MessageBox(NULL, m_csErr, L"错误", MB_OK);
		return false;
	}

	return true;
}

/*************************************************
函数名:    	StopPlay
函数描述:	停止播放
输入参数:   _camera标示那个摄像头
输出参数:   			
返回值:	成返回True  失败返回False	
**************************************************/
bool CDeviceOperation::StopPlay(int _camera)
{
	if(m_lPlayHandle[_camera] != -1)
	{
		if(!NET_DVR_StopRealPlay(m_lPlayHandle[_camera]))
			return false;
		m_lPlayHandle[_camera]=-1;
	}
	else
		return false;

	return true;
}

bool CDeviceOperation::CDeviceCapture(int _camera, CString _pictureName)
{
	CString _PicName = m_picturePath + L"/" + _pictureName;
	char PicName[256] = {0};
	if(m_pictureType == L"BMP")  //bmp
	{
		_PicName += L".bmp";
		CStringToChar(_PicName, PicName);
		if(!NET_DVR_CapturePicture(m_lPlayHandle[_camera],PicName))
		{
			DWORD err=NET_DVR_GetLastError();
			CString m_csErr;
			m_csErr.Format(L"抓图出错，错误代码%d",err);
			CString _error;
			_error.Format(L"%d", _camera);
			m_log.WriteErrorLog(L"摄像头 "+_error+L" 抓图出错！");
			AfxMessageBox(m_csErr);
			return false;
		}
	}
	else if(m_pictureType == L"JPGE")  //jgp
	{
		_PicName += L".jpg";
		CStringToChar(_PicName, PicName);
		//组建jpg结构
		NET_DVR_JPEGPARA JpgPara = {0};
		if(m_pictureQuality == L"最好")
			JpgPara.wPicSize = 0;
		else if(m_pictureQuality == L"较好")
			JpgPara.wPicSize = 1;
		else if(m_pictureQuality == L"一般")
			JpgPara.wPicSize = 2;
		else
			JpgPara.wPicSize = 0;

		if(m_pictureSize == L"CIF")
			JpgPara.wPicSize = 0;
		else if(m_pictureSize == L"QCIF")
			JpgPara.wPicSize = 1;
		else if(m_pictureSize == L"D1")
			JpgPara.wPicSize = 2;
		else if(m_pictureSize == L"UXGA")
			JpgPara.wPicSize = 3;
		else if(m_pictureSize == L"SVGA")
			JpgPara.wPicSize = 4;
		else if(m_pictureSize == L"HD720p")
			JpgPara.wPicSize = 5;
		else if(m_pictureSize == L"VGA")
			JpgPara.wPicSize = 6;
		else if(m_pictureSize == L"XVGA")
			JpgPara.wPicSize = 7;
		else if(m_pictureSize == L"HD900p")
			JpgPara.wPicSize = 8;
		else
			JpgPara.wPicSize = 9;

		LONG iCurChan = m_iCurChanIndex[_camera];

		if(!NET_DVR_CaptureJPEGPicture(m_videoDeviceInfo[_camera].lLoginID, iCurChan, &JpgPara, PicName))
		{
			DWORD err=NET_DVR_GetLastError();
			CString m_csErr;
			m_csErr.Format(L"抓图出错，错误代码%d",err);
			CString _error;
			_error.Format(L"%d", _camera);
			m_log.WriteErrorLog(L"摄像头 "+_error+L" 抓图出错！");
			AfxMessageBox(m_csErr);
			return false;
		}
	}

	return true;
}

/**_carCardID：刷卡的车辆ID号
*车照文件名由车卡ID-站点号-进/出-(0-4)
*1-6：代表不同的站点
*1：进  0：出
*0-4：五个摄像头拍摄的照片
*例如：000000001000000001000001-1-0-0.jpg
**/
bool CDeviceOperation::SendPicture(CString _carCardID, int _siteID, bool _in_out)
{
	SOCKET *sockClient = new SOCKET;
	*sockClient = socket(AF_INET,SOCK_STREAM,0);
	if(INVALID_SOCKET == *sockClient)  
    {  
		AfxMessageBox(L"上传图片套接字创建失败！");  
		m_log.WriteErrorLog(L"上传图片套接字创建失败！");
        return false;  
    } 
	SOCKADDR_IN addrSrv; 
	USES_CONVERSION;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(T2A(m_Receive_IP));
	//本机地址,服务器在本机开启 
	addrSrv.sin_family=AF_INET;  
	addrSrv.sin_port=htons(m_Receive_Port);
	//设置端口号
	int _ret = connect(*sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if(_ret == -1)
	{
		AfxMessageBox(L"上传图片套接字连接失败！");  
		m_log.WriteErrorLog(L"上传图片套接字连接失败！");
        return false;
	}
	//连接服务器
	DWORD  m_sendPicThreadID;
	HANDLE m_sendPichThread;

	struct SendParameter *m_sendParameter = new struct SendParameter;
	m_sendParameter->_sockClient = sockClient;
	m_sendParameter->_picturePath = m_picturePath;
	m_sendParameter->_carCardID = _carCardID;
	m_sendParameter->_siteID = _siteID;
	m_sendParameter->_in_out = _in_out;
	if(m_pictureType == L"BMP")
		m_sendParameter->_pictureType = L".bmp";
	else
		m_sendParameter->_pictureType = L".jpg";
	//开启线程			
	m_sendPichThread = CreateThread(NULL,0,SendPicThread,m_sendParameter,0,&m_sendPicThreadID);  
	CloseHandle(m_sendPichThread);

	return true;
}

/**
*发送协议
*数据块由1024(0x400)个字节组成, 发送信息有18(0x12)个字节组成
*发送信息第一个字符为'H'为发送文件名
*发送信息第一个字符为'E'为发送数据块
*发送信息第一个字符为'N'为不存在任何图片
*发送信息第一个字符为'F'为发送最后一个数据块数据块 
*发送信息第二个字符为图片的个数
*发送信息第3~18字符为发送数据块的大小
*返回值为"OK"为成功接收
*返回值为"fail"为则创建本地文件失败
*返回值为"finish"为成功接收所有图片
*返回值为"error"为接收失败
**/
static DWORD WINAPI  SendPicThread(void *pArg)
{
	struct SendParameter *_parameter = (struct SendParameter *)pArg;
	char _send_Buf[SEND_DATA_BUF_SIZE+SEND_INFO_BUF_SIZE] = {'\0'};
	char _recvBuf[10];
	BOOL _isExistPic = false;
	for(int _pictureNumber = 0; _pictureNumber < 5; _pictureNumber++)
	{
		WIN32_FIND_DATA FindFileData;
		CString _pictureID;
		_pictureID.Format(L"%d", _pictureNumber);
		CString _siteID;
		_siteID.Format(L"%d", _parameter->_siteID);
		CString _in_out;
		_in_out.Format(L"%d", _parameter->_in_out);
		CString _pictureName = _parameter->_carCardID + L"-" + _siteID + L"-" + _in_out + L"-" + _pictureID;
		_pictureName = _parameter->_picturePath + L"/" + _pictureName;
		_pictureName += _parameter->_pictureType;
		if (INVALID_HANDLE_VALUE == FindFirstFile(_pictureName, &FindFileData))
		{
			AfxMessageBox(_pictureName + L" 文件路径错误或文件不存在!");
			m_log.WriteErrorLog(_pictureName + L" 文件路径错误或文件不存在!");
			continue;
		}
		memset(_send_Buf, '\0', SEND_DATA_BUF_SIZE+SEND_INFO_BUF_SIZE);
		_send_Buf[0] = 'H';
		_send_Buf[1] = '0'+_pictureNumber;
		//文件大小 
		_itoa(FindFileData.nFileSizeLow, &_send_Buf[2], 10);
		//文件名 
		for (int i = SEND_INFO_BUF_SIZE; i <= (SEND_DATA_BUF_SIZE+SEND_INFO_BUF_SIZE) && FindFileData.cFileName[i-SEND_INFO_BUF_SIZE] != '\0'; i++)
			_send_Buf[i-1] = FindFileData.cFileName[i-SEND_INFO_BUF_SIZE];
		FILE* m_file;
		USES_CONVERSION;
		if (!(m_file = fopen(T2A(_pictureName), "rb")))
		{
			AfxMessageBox(L"读取" + _pictureName + L"文件失败！");
			m_log.WriteErrorLog(L"读取" + _pictureName + L"文件失败！");
			continue;
		}
		_isExistPic = true;
		int _send_size = send(*(_parameter->_sockClient),_send_Buf,(SEND_DATA_BUF_SIZE+SEND_INFO_BUF_SIZE),0);
		while(1)
		{
			memset(_send_Buf, '\0', SEND_DATA_BUF_SIZE+SEND_INFO_BUF_SIZE);
			_send_size = fread(&_send_Buf[SEND_INFO_BUF_SIZE], 1, SEND_DATA_BUF_SIZE, m_file);
			memset(_recvBuf, '\0', 10);
			recv(*(_parameter->_sockClient), _recvBuf, 10, 0);
			if(!strcmp(_recvBuf, "fail"))
			{
				fclose(m_file);
				break;
			}
			else if(!strcmp(_recvBuf, "error"))
			{
				closesocket(*(_parameter->_sockClient));
				delete _parameter->_sockClient;
				delete _parameter;
				ExitThread(0);
			}

			if (_send_size < SEND_DATA_BUF_SIZE)
			{
				_send_Buf[0] = 'F';
				_send_Buf[1] = '0'+_pictureNumber;
				_itoa(_send_size, &_send_Buf[2], 10); 
				send(*(_parameter->_sockClient), _send_Buf, 
					(SEND_DATA_BUF_SIZE+SEND_INFO_BUF_SIZE), 0);
				fclose(m_file);
				break;
			}
			else
			{
				_send_Buf[0] = 'E';
				_send_Buf[1] = '0'+_pictureNumber;
				_itoa(_send_size, &_send_Buf[2], 10);  
				send(*(_parameter->_sockClient),_send_Buf,
					(SEND_DATA_BUF_SIZE+SEND_INFO_BUF_SIZE),0);
			}
		}
	}

	memset(_send_Buf, '\0', SEND_DATA_BUF_SIZE+SEND_INFO_BUF_SIZE);
	_send_Buf[0] = 'N';
	int _send_size = send(*(_parameter->_sockClient),_send_Buf,(SEND_DATA_BUF_SIZE+SEND_INFO_BUF_SIZE),0);

	memset(_recvBuf, '\0', 10);
	recv(*(_parameter->_sockClient), _recvBuf, 10, 0);
	if(!strcmp(_recvBuf, "finish"))
		CDeviceOperation::DeleteCarPicture(_parameter->_carCardID);

	//关闭套接字
	closesocket(*(_parameter->_sockClient));
	delete _parameter->_sockClient;
	delete _parameter;
	ExitThread(0);
}

void CDeviceOperation::DeleteCarPicture(CString _carCardID)
{           
    CFileFind   _fileFind;     
	CString _pictureName = m_picturePath+"/"+_carCardID;
    BOOL bFound = _fileFind.FindFile(m_picturePath+L"\\*", 0);     
	while(bFound)     
    {     
        bFound = _fileFind.FindNextFile();     
		CString _temp = _fileFind.GetFileName();
        if(_fileFind.GetFileName() == "." || _fileFind.GetFileName()== "..")     
            continue;
		_temp = _temp.Left(_temp.GetLength()-10);  //获取车卡ID
		SetFileAttributes(_fileFind.GetFilePath(), FILE_ATTRIBUTE_NORMAL);
		DeleteFile(_fileFind.GetFilePath());          
    }     
    _fileFind.Close();        
} 

//开始录像
bool CDeviceOperation::CDeviceStartRecord(int _camera, CString _pictureName)
{
	char RecName[256] = {0};
	_pictureName = m_videoPath + L"/" + _pictureName + L".mp4";
	CStringToChar(_pictureName, RecName);
	if(!NET_DVR_SaveRealData(m_lPlayHandle[_camera],RecName))
	{
		DWORD err=NET_DVR_GetLastError();
		CString m_csErr;
		m_csErr.Format(L"启动录像出错，错误代码%d",err);
		CString _error;
		_error.Format(L"%d", _camera);
		m_log.WriteErrorLog(L"摄像头 "+_error+L" 启动录像出错！");
		AfxMessageBox(m_csErr);
		return false;
	}

	return true;
}

//停止录像
bool CDeviceOperation::CDeviceStopRecord(int _camera)
{
	if(!NET_DVR_StopSaveRealData(m_lPlayHandle[_camera]))
	{
		DWORD err=NET_DVR_GetLastError();
		CString m_csErr;
        m_csErr.Format(L"停止录像出错，错误代码%d",err);
		CString _error;
		_error.Format(L"%d", _camera);
		m_log.WriteErrorLog(L"摄像头 "+_error+L" 停止录像出错！");
		AfxMessageBox(m_csErr);
		return false;
	}

	return true;
}