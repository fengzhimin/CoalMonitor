
// ClientCode.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
  
#include "SkinPPWTL.h" //������ط���   
#pragma comment(lib,"SkinPPWTL.lib")  


// CClientCodeApp:
// �йش����ʵ�֣������ ClientCode.cpp
//

class CClientCodeApp : public CWinApp
{
public:
	CClientCodeApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CClientCodeApp theApp;