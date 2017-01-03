#ifndef _CARCARDTHREADPARAMETER_H
#define _CARCARDTHREADPARAMETER_H

#include "stdafx.h"

struct CarCardThreadParameter
{
	CDialogEx *m_dialog;     //对话框指针
	int m_ShowID;            //显示信息的控件ID
	CString m_carCardInfo;   //车卡信息
	bool m_isSuccess;        //是否读取车卡信息成功	
	bool m_noCarCard;        //标识没有车卡
};


#endif