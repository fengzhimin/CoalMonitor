
// Receive_CarPicture.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "SkinPPWTL.h" //������ط���   
#pragma comment(lib,"SkinPPWTL.lib")  

// CReceive_CarPictureApp:
// �йش����ʵ�֣������ Receive_CarPicture.cpp
//

class CReceive_CarPictureApp : public CWinApp
{
public:
	CReceive_CarPictureApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CReceive_CarPictureApp theApp;