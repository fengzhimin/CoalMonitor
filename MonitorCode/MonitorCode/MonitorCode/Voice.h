#pragma once

#include <sapi.h>
#pragma comment(lib,"sapi.lib")

class CVoice
{
private:
	ISpVoice *pVoice;
public:
	CVoice(void);
	~CVoice(void);
	bool Initial();  //��ʼ�������豸  ��ʼ���ɹ�����true  ���򷵻�false
	void SpeakInfo(CString _str);
};

