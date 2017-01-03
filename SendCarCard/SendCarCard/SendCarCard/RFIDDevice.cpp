#include "stdafx.h"
#include "RFIDDevice.h"
#include "Log.h"
#include "..//dll/disdll.h"
#pragma comment(lib, "..//dll/disdll.lib")

extern Log m_log;

int CRFIDDevice::CharsToHex(char _ch)
{
	if(_ch >= '0' && _ch <= '9')
		return _ch - '0';
	else
	{
		if(_ch >= 'a' && _ch <= 'f')
			return _ch - 87;
		else if(_ch >= 'A' && _ch <= 'F')
			return _ch - 55;
		else
			return 0;
	}
}

BOOL CRFIDDevice::CharsIsValid(CString& str)
{
	int length = str.GetLength();
	if (0 == length)
	{
		AfxMessageBox("û������");
		return FALSE;
	}
	for (int i = 0; i < length;  ++i) // ����ַ��Ƿ�Ϊ��Ч�ַ�
	{
		char ch = str.GetAt(i);
		if ( (ch <  '0')   ||
			(ch > '9' && ch < 'A') ||
			(ch > 'F'  && ch < 'a') ||
			(ch  > 'f'))
		{
			AfxMessageBox("������Ч�ַ�");
			return FALSE;
		}
	}
	if (length % 4 != 0)
	{
		AfxMessageBox("����λ��������4��������");
		return FALSE;
	}
	return TRUE;
}

BOOL CRFIDDevice::Initial(int _serialComm, int _baudRate)
{
	if (!DeviceInit("", _serialComm, _baudRate))
	{
		AfxMessageBox("�豸��ʼ��ʧ��.");
		m_log.WriteErrorLog("RFID�豸��ʼ��ʧ��.");
		return false;
	}

	if (!DeviceConnect())
	{
		AfxMessageBox("�豸����ʧ��.");
		m_log.WriteErrorLog("RFID�豸����ʧ��.");
		return false;
	}
	else
	{
		//�����д����
		return true;
	}
}

BOOL CRFIDDevice::ReInitial(int _serialComm, int _baudRate)
{
	DeviceUninit();
	DeviceDisconnect();
	
	return Initial(_serialComm, _baudRate);
}

BOOL CRFIDDevice::Write(CString _rwdata)
{
	CString m_rwdata = _rwdata;
	if (m_rwdata.GetLength() == 0)
	{
		AfxMessageBox("û������.");
		m_log.WriteErrorLog("RFID�豸û�����ݿ���д��.");
		return false;
	}

	unsigned char begin = 2;

	m_rwdata.Remove(' '); // ȥ������Ŀո�
	m_rwdata.Remove(','); // ȥ������Ķ���

	if (CharsIsValid(m_rwdata))
	{
		int MaxLength = 0;
		MaxLength = 8 - begin;
		BYTE length = m_rwdata.GetLength() / 4; // ����Ϊ����
		 if (length > MaxLength)
		 {
			 	AfxMessageBox("���ݳ��ȳ������ֵ.");
				return false;
		 }
		//	ת��д������
		unsigned char buf[128] = {0};
	
		for (int index = 0; index < length*2; ++index)
		{
			int i = CharsToHex(m_rwdata[2 * index])<<4;
			int j = CharsToHex(m_rwdata[2 * index + 1]);
			buf[index] = CharsToHex(m_rwdata[2 * index])<<4 | CharsToHex(m_rwdata[2 * index + 1]);
		}

		if ( WriteTagData(0, 1, begin, length, &buf[0]))
		{
			return true;
		}
		else
		{
			return false;
		}	
	}
	else
		return false;
}

//��ȡ������Ϣ
//��ȡ�ɹ��򷵻س�����Ϣ
//��ȡʧ���򷵻ؿ�
CString CRFIDDevice::Read()
{
	int readAgainRate;
	unsigned char begin = 2;
	unsigned char length = 6; // ѡ����������0��ʼ
	unsigned char buf[128] = {0};// ���ջ�����	
	CString _ret_CarCardInfo;

	CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����  
	BOOL ifFind = finder.FindFile(_T("./Config.ini"));  
	if( ifFind )  
	{  
		CString _temp = "";
		::GetPrivateProfileStringA("ReadAgainRate Config Info","Rate","5",_temp.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
		_temp.ReleaseBuffer();  
		readAgainRate = _ttoi(_temp);
	}
	else
		readAgainRate = 5;

	int _warn_count = 0;
	while(true)
	{
		if ( ReadTagData(0,  1, begin, length, &buf[0]))
		{
			CString temp;
			for(int i = 3; i < length *2 + 3; ++i) // ǰ3�ֽ�Ϊ�����bank��begin��length
			{
				temp.Format("%02X ", buf[i]);
				_ret_CarCardInfo += temp;
			}

			return _ret_CarCardInfo;
		}
		else
		{
			if(_warn_count == readAgainRate)
			{
				return "";
			}
			else
				_warn_count++;
		}
	}
}