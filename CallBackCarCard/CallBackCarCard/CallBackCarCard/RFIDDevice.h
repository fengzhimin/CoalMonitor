#pragma once
class CRFIDDevice
{
public:
	BOOL CharsIsValid(CString& str);
	int CharsToHex(char _ch);
	BOOL Write(CString _rwdata);  
	CString Read();
	BOOL Initial(int _serialComm, int _baudRate);   //初始化RFID设备(链接设备)
	BOOL ReInitial(int _serialComm, int _baudRate);  //重新初始化RFID设备
};

