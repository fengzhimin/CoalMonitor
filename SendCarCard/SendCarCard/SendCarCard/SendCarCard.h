
// SendCarCard.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "SkinH.h"
#pragma comment(lib,"SkinH.lib")

// CSendCarCardApp:
// �йش����ʵ�֣������ SendCarCard.cpp
//

class CSendCarCardApp : public CWinApp
{
public:
	CSendCarCardApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CSendCarCardApp theApp;