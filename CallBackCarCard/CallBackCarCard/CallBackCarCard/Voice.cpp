#include "stdafx.h"
#include "Voice.h"
#include "Log.h"

extern Log m_log;

//语音播报类
//构造函数
CVoice::CVoice()
{
	pVoice = NULL;
}

//析构函数
CVoice::~CVoice()
{
	//释放com资源
	::CoUninitialize();
}

//初始化函数
bool CVoice::Initial()
{
	if(FAILED(::CoInitialize(NULL)))
	{
		m_log.WriteErrorLog("语音设备初始化失败！");
		return false;
	}
	else
		return true;
}

void CVoice::SpeakInfo(LPCWSTR _str)
{
	//获取SpVoice接口
	HRESULT hr = CoCreateInstance(CLSID_SpVoice,NULL,CLSCTX_ALL,IID_ISpVoice,(void**)&pVoice);
	if (SUCCEEDED(hr))
	{
		hr = pVoice->Speak(_str, 0, NULL);
		pVoice->Release();
		pVoice = NULL;
	}
	else
		m_log.WriteErrorLog("语音播报失败！");
}