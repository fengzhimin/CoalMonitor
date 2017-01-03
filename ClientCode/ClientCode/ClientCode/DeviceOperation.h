#ifndef DEVICEOPERATION_H
#define DEVICEOPERATION_H

#include "GeneralDef.h"
#include "CarCardThreadParameter.h"

struct SendParameter
{
	SOCKET *_sockClient;
	CString _picturePath;   //ͼƬ���·��
	CString _pictureType;   //ͼƬ����
	CString _carCardID;
	int _siteID;   //վ���
	bool _in_out;  //�����߳�
};

#pragma once
class CDeviceOperation
{
public:
	DWORD  m_readCarCardThreadID;
	HANDLE hThread;
	CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����  
	BOOL ifFind;
	CString m_videoIP[5];   //����ͷIP
	int m_videoPort[5];     //����ͷ�˿�
	CString m_videoUser[5]; //����ͷ�û���
	CString m_videoPwd[5];  //����ͷ��¼����
	CString m_pictureSize;     //ͼƬ�Ĵ�С
	CString m_pictureQuality;    //ͼƬ������   
	CString m_pictureType;  //ͼƬ������
	CString m_videoPath;    //¼��Ĵ��·��
	int m_serialCom;     //�����豸�˿ں�
	int m_baudRate;      //�����豸������
	LOCAL_DEVICE_INFO m_videoDeviceInfo[5];   //����ͷ�豸
	int m_iCurChanIndex[5];               //��ǰͨ��������������
	LONG m_lPlayHandle[5];
	BOOL m_sendPictureing;     //�Ƿ����ڷ���ͼƬ
public:
	NET_DVR_CLIENTINFO ClientInfo[5];
	struct CarCardThreadParameter m_carCardThreadParameter;
public:
	
	CDeviceOperation(void);
	~CDeviceOperation(void);
	bool Init();   //�豸��ʼ������
	//��ȡ������Ϣ ����ȡ������Ϣ�����m_threadParameter�ṹ����
	void ReadCarCardInfo(CDialogEx *_dialog, int _showID);
	//��¼����ͷ
	bool LoginVideo(int _camera);
	//�˳�����ͷ
	bool LogoutVideo(int _camera);
	//CStringתchar*
	void CStringToChar(CString _cstring, char *_ch);
	//��ʼһ·����
	bool StartPlay(int _camera);
	//ֹͣ����
	bool StopPlay(int _camera);
	//ץͼ
	bool CDeviceCapture(int _camera, CString _pictureName);
	//ɾ��ץ����ͼƬ
	static void  DeleteCarPicture(CString _carCardID);
	//����ͼƬ
	bool SendPicture(CString _carCardID, int _siteID, bool _in_out);
	//��ʼ¼��
	bool CDeviceStartRecord(int _camera, CString _pictureName);
	//ֹͣ¼��
	bool CDeviceStopRecord(int _camera);
	//��ȡ����ͷ�����ļ�
	void ReadVideoConfig(int _camera);  
};

#endif