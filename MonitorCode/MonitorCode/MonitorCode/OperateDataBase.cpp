#include "stdafx.h"
#include "OperateDataBase.h"
#include "Log.h"

extern Log m_log;
extern CString m_config;

OperateDataBase::OperateDataBase(void)
{
	CString _ip, _database, _uid, _pwd;
	CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����  
	BOOL ifFind = finder.FindFile(m_config);  
	if( ifFind )  
	{  
		::GetPrivateProfileStringW(_T("Database connection Info"),_T("IP"),_T(""),_ip.GetBuffer(MAX_PATH),MAX_PATH,m_config);  
		::GetPrivateProfileStringW(_T("Database connection Info"),_T("Database"),_T(""),_database.GetBuffer(MAX_PATH),MAX_PATH,m_config);  
		::GetPrivateProfileStringW(_T("Database connection Info"),_T("UID"),_T(""),_uid.GetBuffer(MAX_PATH),MAX_PATH,m_config);  
		::GetPrivateProfileStringW(_T("Database connection Info"),_T("PWD"),_T(""),_pwd.GetBuffer(MAX_PATH),MAX_PATH,m_config);    
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

tb_CarCheckInfo OperateDataBase::getLastRecordByCarCardID(CString _carCardID)
{
	CString _select = L"select top 1 * from tb_CarCheckInfo where carCardID = '" + _carCardID + L"' order by ID desc";
	M_pRecordset = M_pConnection->Execute(_bstr_t(_select), NULL, adCmdText);
	tb_CarCheckInfo _ret_carCheckInfo;
	if (!M_pRecordset->adoEOF)
	{
		_ret_carCheckInfo.cargoSize = M_pRecordset->GetCollect("cargoSize");
		_ret_carCheckInfo.cargoCarSize = M_pRecordset->GetCollect("cargoCarSize");
		_ret_carCheckInfo.cargoWeight = M_pRecordset->GetCollect("cargoWeight");
		_ret_carCheckInfo.weighPlace = M_pRecordset->GetCollect("weighPlace");
		if((CString)M_pRecordset->GetCollect("inOut") == L"��")
			_ret_carCheckInfo.inOut = true;
		else
			_ret_carCheckInfo.inOut = false;
	}
	
	return _ret_carCheckInfo;
}

tb_CarCheckInfo OperateDataBase::getPrevRecordByCarCardID(CString _carCardID)
{
	CString _select = L"select top 2 * from tb_CarCheckInfo where carCardID = '" + _carCardID + L"' order by ID desc";
	M_pRecordset = M_pConnection->Execute(_bstr_t(_select), NULL, adCmdText);
	tb_CarCheckInfo _ret_carCheckInfo;
	BOOL m_point = false;  //���ڱ��ֻ��ȡ��һ����¼
	
	while(!M_pRecordset->adoEOF)
	{
		if(m_point)
		{
			_ret_carCheckInfo.cargoSize = M_pRecordset->GetCollect("cargoSize");
			_ret_carCheckInfo.cargoCarSize = M_pRecordset->GetCollect("cargoCarSize");
			_ret_carCheckInfo.cargoWeight = M_pRecordset->GetCollect("cargoWeight");
			_ret_carCheckInfo.weighPlace = M_pRecordset->GetCollect("weighPlace");
			if((CString)M_pRecordset->GetCollect("inOut") == L"��")
				_ret_carCheckInfo.inOut = true;
			else
				_ret_carCheckInfo.inOut = false;
		}
		m_point = true;
		M_pRecordset->MoveNext();
	}
	
	return _ret_carCheckInfo;
}

void OperateDataBase::setAbnormalToCarCheckInfoTable(CString _carCardID, int _value)
{
	CString _temp;
	_temp.Format(L"%d", _value);
	CString _update = L"update tb_CarCheckInfo set abnormal = " + _temp + 
		L" where recordTime in ( select max(recordTime) from tb_CarCheckInfo where carCardID = '" + _carCardID + L"')";
	M_pRecordset = M_pConnection->Execute(_bstr_t(_update), NULL, adCmdText);
}