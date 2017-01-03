
#ifndef CARCARDINFO_H
#define CARCARDINFO_H

#include "stdafx.h"
#include <sapi.h>
#pragma comment(lib,"sapi.lib")

//语音播报类
class CVoice {
private:
	ISpVoice *pVoice;
public:
	CVoice();
	~CVoice();
	bool Initial();  //初始化语音设备  初始化成功返回true  否则返回false
	void SpeakInfo(CString _str);
};

#endif