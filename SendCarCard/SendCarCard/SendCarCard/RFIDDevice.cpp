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
		AfxMessageBox("没有数据");
		return FALSE;
	}
	for (int i = 0; i < length;  ++i) // 检查字符是否为无效字符
	{
		char ch = str.GetAt(i);
		if ( (ch <  '0')   ||
			(ch > '9' && ch < 'A') ||
			(ch > 'F'  && ch < 'a') ||
			(ch  > 'f'))
		{
			AfxMessageBox("含有无效字符");
			return FALSE;
		}
	}
	if (length % 4 != 0)
	{
		AfxMessageBox("数据位数必须是4的整倍数");
		return FALSE;
	}
	return TRUE;
}

BOOL CRFIDDevice::Initial(int _serialComm, int _baudRate)
{
	if (!DeviceInit("", _serialComm, _baudRate))
	{
		AfxMessageBox("设备初始化失败.");
		m_log.WriteErrorLog("RFID设备初始化失败.");
		return false;
	}

	if (!DeviceConnect())
	{
		AfxMessageBox("设备连接失败.");
		m_log.WriteErrorLog("RFID设备连接失败.");
		return false;
	}
	else
	{
		//允许读写操作
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
		AfxMessageBox("没有数据.");
		m_log.WriteErrorLog("RFID设备没有数据可以写入.");
		return false;
	}

	unsigned char begin = 2;

	m_rwdata.Remove(' '); // 去除加入的空格
	m_rwdata.Remove(','); // 去除加入的逗号

	if (CharsIsValid(m_rwdata))
	{
		int MaxLength = 0;
		MaxLength = 8 - begin;
		BYTE length = m_rwdata.GetLength() / 4; // 换算为字数
		 if (length > MaxLength)
		 {
			 	AfxMessageBox("数据长度超过最大值.");
				return false;
		 }
		//	转换写入数据
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

//读取车卡信息
//读取成功则返回车卡信息
//读取失败则返回空
CString CRFIDDevice::Read()
{
	int readAgainRate;
	unsigned char begin = 2;
	unsigned char length = 6; // 选择项索引从0开始
	unsigned char buf[128] = {0};// 接收缓冲区	
	CString _ret_CarCardInfo;

	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
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
			for(int i = 3; i < length *2 + 3; ++i) // 前3字节为传入的bank、begin、length
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