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
		m_log.WriteErrorLog(L"读取配置文件错误!");

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

void OperateDataBase::Add(tb_AgreementAndCarCardInfo object)
{
	CString _select = L"select * from tb_AgreementAndCarCardInfo where agreementNo = '" + 
		object.agreementNo + L"' and carCardID = '" + object.carCardID + "'";
	M_pRecordset = M_pConnection->Execute(_bstr_t(_select), NULL, adCmdText);
	if (!M_pRecordset->adoEOF)
		return ;  //已经存在该记录！

	CString _SQL[23];
	_SQL[0] = object.agreementNo;
	_SQL[1] = object.carCardID;
	_SQL[2] = object.plateNumber;
	_SQL[3] = object.emptyCarSize;
	_SQL[4] = object.cargoSize;
	_SQL[5] = object.cargoCarSize;
	_SQL[6] = object.carShapeInfo;
	_SQL[7].Format(L"%f",object.emptyCarWeight);
	_SQL[8].Format(L"%f",object.CarWeight);
	_SQL[9].Format(L"%f",object.cargoWeight);
	_SQL[10].Format(L"%f",object.cargoDensity);
	_SQL[11].Format(L"%f",object.deductWeight);
	_SQL[12] = object.carPicture1;
	_SQL[13] = object.carPicture2;
	_SQL[14] = object.carPicture3;
	_SQL[15] = object.carPicture4;
	_SQL[16] = object.carPicture5;
	_SQL[17] = object.weighPlace;
	_SQL[18].Format(L"%d",object.abnormal);
	if(object.inOut)
		_SQL[19] = L"进";
	else
		_SQL[19] = L"出";
	_SQL[20] = object.recordTime.Format("'%Y-%m-%d %H:%M:%S'");
	_SQL[21] = object.firstTime.Format("'%Y-%m-%d %H:%M:%S'");
	_SQL[22] = object.lastTime.Format("'%Y-%m-%d %H:%M:%S'");
	CString _strSQL = L"insert into tb_AgreementAndCarCardInfo values ('";
	for(int i = 0; i < 6; i++)
		_strSQL = _strSQL + _SQL[i] + L"','";
	_strSQL = _strSQL + _SQL[6] + L"',";
	for(int i = 7; i < 11; i++)
		_strSQL = _strSQL + _SQL[i] + L",";
	_strSQL = _strSQL + _SQL[11] + L",'";
	for(int i = 12; i < 17; i++)
		_strSQL = _strSQL + _SQL[i] + L"','";
	_strSQL = _strSQL + _SQL[17] + L"',";
	_strSQL = _strSQL + _SQL[18] + L",'";
	_strSQL = _strSQL + _SQL[19] + L"',";
	for(int i = 20; i < 22; i++)
		_strSQL = _strSQL + _SQL[i] + L",";
	_strSQL += _SQL[22] + L")";
	M_pConnection->Execute(_bstr_t(_strSQL), NULL, adCmdText);
}

void OperateDataBase::Add(tb_AgreementInfo object)
{
	CString _select = L"select * from tb_AgreementInfo where agreementNo = '" + 
		object.agreementNo + L"' and merchantCardID = '" + object.merchantCardID + "'";
	M_pRecordset = M_pConnection->Execute(_bstr_t(_select), NULL, adCmdText);
	if (!M_pRecordset->adoEOF)
		return ;  //已经存在该记录！

	CString _SQL[10];
	_SQL[0] = object.agreementNo;
	_SQL[1] = object.merchantCardID;
	_SQL[2] = object.signDate.Format("'%Y-%m-%d %H:%M:%S'");
	_SQL[3] = object.validatyPeriod.Format("'%Y-%m-%d %H:%M:%S'");
	_SQL[4].Format(L"%f",object.coalTon);
	_SQL[5].Format(L"%d",object.carCardCount);
	_SQL[6].Format(L"%d",object.useCarCardCount);
	_SQL[7].Format(L"%f",object.actualCoalTon);
	_SQL[8].Format(L"%d",object.returnCarCardCount);
	if(object.agreementStatus)
		_SQL[9] = L"完成";
	else
		_SQL[9] = L"未完成";
	CString _strSQL = L"insert into tb_AgreementInfo values ('";
	_strSQL = _strSQL + _SQL[0] + L"','";
	_strSQL = _strSQL + _SQL[1] + L"',";
	for(int i = 2; i < 8; i++)
		_strSQL = _strSQL + _SQL[i] + L",";
	_strSQL = _strSQL + _SQL[8] + L",'";
	_strSQL = _strSQL + _SQL[9] + L"')";
	M_pConnection->Execute(_bstr_t(_strSQL), NULL, adCmdText);
}

void OperateDataBase::Add(tb_CarCheckInfo object)
{
	CString _SQL[22];
	_SQL[0] = object.agreementNo;
	_SQL[1] = object.carCardID;
	_SQL[2] = object.plateNumber;
	_SQL[3] = object.emptyCarSize;
	_SQL[4] = object.cargoSize;
	_SQL[5] = object.cargoCarSize;
	_SQL[6].Format(L"%f",object.emptyCarWeight);
	_SQL[7].Format(L"%f",object.CarWeight);
	_SQL[8].Format(L"%f",object.cargoWeight);
	_SQL[9].Format(L"%f",object.cargoDensity);
	_SQL[10].Format(L"%f",object.deductWeight);
	_SQL[11] = object.carPicture1;
	_SQL[12] = object.carPicture2;
	_SQL[13] = object.carPicture3;
	_SQL[14] = object.carPicture4;
	_SQL[15] = object.carPicture5;
	_SQL[16] = object.weighPlace;
	_SQL[17].Format(L"%d",object.abnormal);
	if(object.inOut)
		_SQL[18] = L"进";
	else
		_SQL[18] = L"出";
	_SQL[19] = object.recordTime.Format("'%Y-%m-%d %H:%M:%S'");
	_SQL[20] = object.firstTime.Format("'%Y-%m-%d %H:%M:%S'");
	_SQL[21] = object.lastTime.Format("'%Y-%m-%d %H:%M:%S'");
	CString _strSQL = L"insert into tb_NoCarCheckInfo values ('";
	for(int i = 0; i < 5; i++)
		_strSQL = _strSQL + _SQL[i] + L"','";
	_strSQL = _strSQL + _SQL[5] + L"',";
	for(int i = 6; i < 10; i++)
		_strSQL = _strSQL + _SQL[i] + L",";
	_strSQL = _strSQL + _SQL[10] + L",'";
	for(int i = 11; i < 16; i++)
		_strSQL = _strSQL + _SQL[i] + L"','";
	_strSQL = _strSQL + _SQL[16] + L"',";
	_strSQL = _strSQL + _SQL[17] + L",'";
	_strSQL = _strSQL + _SQL[18] + L"',";
	for(int i = 19; i < 21; i++)
		_strSQL = _strSQL + _SQL[i] + L",";
	_strSQL += _SQL[21] + L")";
	M_pConnection->Execute(_bstr_t(_strSQL), NULL, adCmdText);
}

void OperateDataBase::Add(tb_NoCarCardInfo object)
{
	CString _SQL[22];
	_SQL[0] = object.agreementNo;
	_SQL[1] = object.carCardID;
	_SQL[2] = object.plateNumber;
	_SQL[3] = object.emptyCarSize;
	_SQL[4] = object.cargoSize;
	_SQL[5] = object.cargoCarSize;
	_SQL[6].Format(L"%f",object.emptyCarWeight);
	_SQL[7].Format(L"%f",object.CarWeight);
	_SQL[8].Format(L"%f",object.cargoWeight);
	_SQL[9].Format(L"%f",object.cargoDensity);
	_SQL[10].Format(L"%f",object.deductWeight);
	_SQL[11] = object.carPicture1;
	_SQL[12] = object.carPicture2;
	_SQL[13] = object.carPicture3;
	_SQL[14] = object.carPicture4;
	_SQL[15] = object.carPicture5;
	_SQL[16] = object.weighPlace;
	_SQL[17].Format(L"%d",object.reasonNumber);
	if(object.inOut)
		_SQL[18] = L"进";
	else
		_SQL[18] = L"出";
	_SQL[19] = object.recordTime.Format("'%Y-%m-%d %H:%M:%S'");
	_SQL[20] = object.firstTime.Format("'%Y-%m-%d %H:%M:%S'");
	_SQL[21] = object.lastTime.Format("'%Y-%m-%d %H:%M:%S'");
	CString _strSQL = L"insert into tb_NoCarCardInfo values ('";
	for(int i = 0; i < 5; i++)
		_strSQL = _strSQL + _SQL[i] + L"','";
	_strSQL = _strSQL + _SQL[5] + L"',";
	for(int i = 6; i < 10; i++)
		_strSQL = _strSQL + _SQL[i] + L",";
	_strSQL = _strSQL + _SQL[10] + L",'";
	for(int i = 11; i < 16; i++)
		_strSQL = _strSQL + _SQL[i] + L"','";
	_strSQL = _strSQL + _SQL[16] + L"',";
	_strSQL = _strSQL + _SQL[17] + L",'";
	_strSQL = _strSQL + _SQL[18] + L"',";
	for(int i = 19; i < 21; i++)
		_strSQL = _strSQL + _SQL[i] + L",";
	_strSQL += _SQL[21] + L")";
	M_pConnection->Execute(_bstr_t(_strSQL), NULL, adCmdText);
}

void OperateDataBase::Add(tb_MerchantInfo object)
{
	CString _select = L"select * from tb_MerchantInfo where loginName = '" + 
		object.loginName + L"' and merchantCardID = '" + object.merchantCardID + "'";
	M_pRecordset = M_pConnection->Execute(_bstr_t(_select), NULL, adCmdText);
	if (!M_pRecordset->adoEOF)
		return ;  //已经存在该记录！

	CString _SQL[11];
	_SQL[0] = object.loginName;
	_SQL[1] = object.passwd;
	_SQL[2] = object.company;
	_SQL[3] = object.merchantCardID;
	_SQL[4] = object.registerDate.Format("'%Y-%m-%d %H:%M:%S'");
	_SQL[5] = object.registerName;
	_SQL[6] = object.registerID;
	_SQL[7] = object.registerPhone;
	_SQL[8] = object.registerAddress;
	_SQL[9].Format(L"%d",object.loginNumber);
	_SQL[10] = object.lastLoginTime.Format("'%Y-%m-%d %H:%M:%S'");
	
	CString _strSQL = L"insert into tb_MerchantInfo values ('";
	for(int i = 0; i < 3; i++)
		_strSQL = _strSQL + _SQL[i] + L"','";
	_strSQL = _strSQL + _SQL[3] + L"',";
	_strSQL = _strSQL + _SQL[4] + L",'";
	for(int i = 5; i < 8; i++)
		_strSQL = _strSQL + _SQL[i] + L"','";
	_strSQL = _strSQL + _SQL[8] + L"',";
	_strSQL = _strSQL + _SQL[9] + L",";
	_strSQL = _strSQL + _SQL[10] + L")";
	M_pConnection->Execute(_bstr_t(_strSQL), NULL, adCmdText);
}