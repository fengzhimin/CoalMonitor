#ifndef _CARCARDTHREADPARAMETER_H
#define _CARCARDTHREADPARAMETER_H

#include "stdafx.h"

struct CarCardThreadParameter
{
	CDialogEx *m_dialog;     //�Ի���ָ��
	int m_ShowID;            //��ʾ��Ϣ�Ŀؼ�ID
	CString m_carCardInfo;   //������Ϣ
	bool m_isSuccess;        //�Ƿ��ȡ������Ϣ�ɹ�	
	bool m_noCarCard;        //��ʶû�г���
};


#endif