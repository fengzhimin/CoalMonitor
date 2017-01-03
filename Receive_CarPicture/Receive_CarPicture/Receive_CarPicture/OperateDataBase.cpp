#include "stdafx.h"
#include "OperateDataBase.h"
#include "Log.h"

extern Log m_log;

OperateDataBase::OperateDataBase(void)
{
	CString _ip, _database, _uid, _pwd;
	CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����  
	BOOL ifFind = finder.FindFile(_T("./Config.ini"));  
	if( ifFind )  
	{  
		::GetPrivateProfileStringW(_T("Database connection Info"),_T("IP"),_T(""),_ip.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		::GetPrivateProfileStringW(_T("Database connection Info"),_T("Database"),_T(""),_database.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		::GetPrivateProfileStringW(_T("Database connection Info"),_T("UID"),_T(""),_uid.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
		::GetPrivateProfileStringW(_T("Database connection Info"),_T("PWD"),_T(""),_pwd.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));    
		//���������ReleaseBuffer()�������޷��ٺ�������ַ���������  
		_ip.ReleaseBuffer();  
		_database.ReleaseBuffer();  
		_uid.ReleaseBuffer();  
		_pwd.ReleaseBuffer(); 
	}
	else
		m_log.WriteErrorLog(L"��ȡ�����ļ�ʧ�ܣ�");

	//�������ݿ�
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
		m_log.WriteErrorLog(L"�������ݿ�ʧ�ܣ�\rIP="+_ip+L"\rDataBase="+_database+L"\ruid="+_uid+L"\rpwd="+_pwd);
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