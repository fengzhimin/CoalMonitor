#ifndef DEVICEOPERATION_H
#define DEVICEOPERATION_H

#include "GeneralDef.h"
#include "CarCardThreadParameter.h"

struct SendParameter
{
	SOCKET *_sockClient;
	CString _picturePath;   //图片存放路径
	CString _pictureType;   //图片类型
	CString _carCardID;
	int _siteID;   //站点号
	bool _in_out;  //进或者出
};

#pragma once
class CDeviceOperation
{
public:
	DWORD  m_readCarCardThreadID;
	HANDLE hThread;
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
	BOOL ifFind;
	CString m_videoIP[5];   //摄像头IP
	int m_videoPort[5];     //摄像头端口
	CString m_videoUser[5]; //摄像头用户名
	CString m_videoPwd[5];  //摄像头登录密码
	CString m_pictureSize;     //图片的大小
	CString m_pictureQuality;    //图片的质量   
	CString m_pictureType;  //图片的类型
	CString m_videoPath;    //录像的存放路径
	int m_serialCom;     //读卡设备端口号
	int m_baudRate;      //读卡设备波特率
	LOCAL_DEVICE_INFO m_videoDeviceInfo[5];   //摄像头设备
	int m_iCurChanIndex[5];               //当前通道在数组中索引
	LONG m_lPlayHandle[5];
	BOOL m_sendPictureing;     //是否正在发送图片
public:
	NET_DVR_CLIENTINFO ClientInfo[5];
	struct CarCardThreadParameter m_carCardThreadParameter;
public:
	
	CDeviceOperation(void);
	~CDeviceOperation(void);
	bool Init();   //设备初始化函数
	//读取车卡信息 将读取车卡信息存放在m_threadParameter结构体中
	void ReadCarCardInfo(CDialogEx *_dialog, int _showID);
	//登录摄像头
	bool LoginVideo(int _camera);
	//退出摄像头
	bool LogoutVideo(int _camera);
	//CString转char*
	void CStringToChar(CString _cstring, char *_ch);
	//开始一路播放
	bool StartPlay(int _camera);
	//停止播放
	bool StopPlay(int _camera);
	//抓图
	bool CDeviceCapture(int _camera, CString _pictureName);
	//删除抓来的图片
	static void  DeleteCarPicture(CString _carCardID);
	//发送图片
	bool SendPicture(CString _carCardID, int _siteID, bool _in_out);
	//开始录像
	bool CDeviceStartRecord(int _camera, CString _pictureName);
	//停止录像
	bool CDeviceStopRecord(int _camera);
	//读取摄像头配置文件
	void ReadVideoConfig(int _camera);  
};

#endif