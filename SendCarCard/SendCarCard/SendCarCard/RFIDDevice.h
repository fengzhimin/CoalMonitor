#pragma once
class CRFIDDevice
{
public:
	BOOL CharsIsValid(CString& str);
	int CharsToHex(char _ch);
	BOOL Write(CString _rwdata);  
	CString Read();
	BOOL Initial(int _serialComm, int _baudRate);   //��ʼ��RFID�豸(�����豸)
	BOOL ReInitial(int _serialComm, int _baudRate);  //���³�ʼ��RFID�豸
};

