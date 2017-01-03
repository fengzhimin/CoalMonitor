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
	bool Initial();  //初始化语音设备  初始化成功返回true  否则返回false
	void SpeakInfo(CString _str);
};

