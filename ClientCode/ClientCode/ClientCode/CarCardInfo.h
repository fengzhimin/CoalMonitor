
#ifndef CARCARDINFO_H
#define CARCARDINFO_H

#include "stdafx.h"
#include <sapi.h>
#pragma comment(lib,"sapi.lib")

//����������
class CVoice {
private:
	ISpVoice *pVoice;
public:
	CVoice();
	~CVoice();
	bool Initial();  //��ʼ�������豸  ��ʼ���ɹ�����true  ���򷵻�false
	void SpeakInfo(CString _str);
};

#endif