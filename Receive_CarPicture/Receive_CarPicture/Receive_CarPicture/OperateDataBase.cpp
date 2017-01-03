#include "stdafx.h"
#include "OperateDataBase.h"
#include "Log.h"

extern Log m_log;

OperateDataBase::OperateDataBase(void)
{
	CString _ip, _database, _uid, _pwd;
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
	BOOL ifFind = finder.FindFile(_T("./Config.ini"));  
	if( ifFind )  
	{  
		::GetPrivateProfileStringW(_T("Database connection Info"),_T("IP"),_T(""),_ip.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		::GetPrivateProfileStringW(_T("Database connection Info"),_T("Database"),_T(""),_database.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		::GetPrivateProfileStringW(_T("Database connection Info"),_T("UID"),_T(""),_uid.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		::GetPrivateProfileStringW(_T("Database connection Info"),_T("PWD"),_T(""),_pwd.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));    
		//在这里必须ReleaseBuffer()，否则无法再后面进行字符串的连接  
		_ip.ReleaseBuffer();  
		_database.ReleaseBuffer();  
		_uid.ReleaseBuffer();  
		_pwd.ReleaseBuffer(); 
	}
	else
		m_log.WriteErrorLog(L"读取配置文件失败！");

	//链接数据库
	::CoInitialize(NULL);
	CString _connectStr = L"driver={SQL Server};server=" + _ip + L";uid=" + _uid + L";pwd=" + _pwd + L";";
	try
	{
		M_pConnection.CreateInstance(__uuidof(Connection));
		M_pConnection->ConnectionString = _bstr_t((LPCTSTR)_connectStr);
		M_pConnection->Open("", "", "", adConnectUnspecified);
		M_pConnection->DefaultDatabase = _bstr_t((LPCTSTR)_database);
	}
	catch (_com_error e)
	{
		m_log.WriteErrorLog(L"连接数据库失败！\rIP="+_ip+L"\rDataBase="+_database+L"\ruid="+_uid+L"\rpwd="+_pwd);
		AfxMessageBox(e.Description());

		return;
	}
}

OperateDataBase::~OperateDataBase(void)
{
	M_pConnection->Close();
	::CoUninitialize();
}

void OperateDataBase::updateCar(CString _carCardID, CString _weighPlace, CString _inOut, CString _plateNumber, CString _tableName)
{
	CString _SQL = L"update " + _tableName + L" set plateNumber = '" + _plateNumber + L"' where carCardID = '" + 
		_carCardID + L"' and weighPlace = '" + _weighPlace + L"' and inOut = '" + _inOut + L"'";
	M_pRecordset = M_pConnection->Execute(_bstr_t(_SQL), NULL, adCmdText);
}