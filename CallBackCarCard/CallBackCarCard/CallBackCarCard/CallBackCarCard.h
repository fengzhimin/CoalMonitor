
// CallBackCarCard.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "SkinH.h"
#pragma comment(lib,"SkinH.lib")

// CCallBackCarCardApp:
// �йش����ʵ�֣������ CallBackCarCard.cpp
//

class CCallBackCarCardApp : public CWinApp
{
public:
	CCallBackCarCardApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCallBackCarCardApp theApp;