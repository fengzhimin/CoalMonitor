#include "stdafx.h"
#include "Voice.h"
#include "Log.h"

extern Log m_log;

//����������
//���캯��
CVoice::CVoice()
{
	pVoice = NULL;
}

//��������
CVoice::~CVoice()
{
	//�ͷ�com��Դ
	::CoUninitialize();
}

//��ʼ������
bool CVoice::Initial()
{
	if(FAILED(::CoInitialize(NULL)))
	{
		m_log.WriteErrorLog("�����豸��ʼ��ʧ�ܣ�");
		return false;
	}
	else
		return true;
}

void CVoice::SpeakInfo(LPCWSTR _str)
{
	//��ȡSpVoice�ӿ�
	HRESULT hr = CoCreateInstance(CLSID_SpVoice,NULL,CLSCTX_ALL,IID_ISpVoice,(void**)&pVoice);
	if (SUCCEEDED(hr))
	{
		hr = pVoice->Speak(_str, 0, NULL);
		pVoice->Release();
		pVoice = NULL;
	}
	else
		m_log.WriteErrorLog("��������ʧ�ܣ�");
}