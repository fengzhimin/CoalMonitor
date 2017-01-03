#include "stdafx.h"
#include "OperateDataBase.h"
#include "Log.h"

extern Log m_log;

#define MAXLENGTH  24

OperateDataBase::OperateDataBase(void)
{
	m_merchantID = m_agreementID = "";
	m_carCardCount = m_useCarCardCount = -1;   
	CString _ip = "", _database = "", _uid = "", _pwd = "";
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用  
	BOOL ifFind = finder.FindFile(_T("./Config.ini"));  
	if( ifFind )  
	{  
		USES_CONVERSION; 
		::GetPrivateProfileStringA("Database connection Info","IP","",_ip.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");  
		::GetPrivateProfileStringA("Database connection Info","Database","",_database.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");  
		::GetPrivateProfileStringA("Database connection Info","UID","",_uid.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");  
		::GetPrivateProfileStringA("Database connection Info","PWD", "",_pwd.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");     
		_ip.ReleaseBuffer();  
		_database.ReleaseBuffer();  
		_uid.ReleaseBuffer();  
		_pwd.ReleaseBuffer(); 
	}
	else
		m_log.WriteErrorLog("读取配置文件失败！");

	//链接数据库
	::CoInitialize(NULL);
	CString _connectStr = "driver={SQL Server};server=" + _ip + ";uid=" + _uid + ";pwd=" + _pwd + ";";
	try
	{
		M_pConnection.CreateInstance(__uuidof(Connection));
		M_pConnection->ConnectionString = _bstr_t((LPCTSTR)_connectStr);
		M_pConnection->Open("", "", "", adConnectUnspecified);
		M_pConnection->DefaultDatabase = _bstr_t((LPCTSTR)_database);
	}
	catch (_com_error e)
	{

		m_log.WriteErrorLog("连接数据库失败！\rIP="+_ip+"\rDataBase="+_database+"\ruid="+_uid+"\rpwd="+_pwd);
		AfxMessageBox(e.Description());
		return;
	}
}


OperateDataBase::~OperateDataBase(void)
{
	M_pConnection->Close();
	::CoUninitialize();
}

BOOL OperateDataBase::MerchantIsExist(CString _merchantID)
{
	CString _select = "select * from tb_AgreementInfo where merchantCardID = '" + _merchantID + "'";
	M_pRecordset = M_pConnection->Execute(_bstr_t(_select), NULL, adCmdText);
	if (!M_pRecordset->adoEOF)
	{
		m_merchantID = _merchantID;
		return true;  //已经存在该记录！
	}
	else
		return false;
}

BOOL OperateDataBase::AgreementIsExist(CString _agreementID)
{
	CString _select = "select * from tb_AgreementInfo where merchantCardID = '" + m_merchantID + "' and agreementNo = '" + _agreementID + "'";
	M_pRecordset = M_pConnection->Execute(_bstr_t(_select), NULL, adCmdText);
	if (!M_pRecordset->adoEOF)
	{
		m_agreementID = _agreementID;
		return true;  
	}
	else
		return false;
}

void OperateDataBase::getUnFinishAgreementByMerchantID(CString _merchantID)
{
	m_agreementIDArray.RemoveAll();
	CString _select = "select * from tb_AgreementInfo where merchantCardID = '" + _merchantID + "' and carCardCount != useCarCardCount";
	M_pRecordset = M_pConnection->Execute(_bstr_t(_select), NULL, adCmdText);
	
	while(!M_pRecordset->adoEOF)
	{
		m_agreementIDArray.Add(M_pRecordset->GetCollect("agreementNo"));
		M_pRecordset->MoveNext();
	}
}

void OperateDataBase::getCarCardInfo()
{
	CString _select = "select * from tb_AgreementInfo where merchantCardID = '" + m_merchantID + "' and agreementNo = '" + m_agreementID + "'";
	M_pRecordset = M_pConnection->Execute(_bstr_t(_select), NULL, adCmdText);
	if (!M_pRecordset->adoEOF)
	{
		m_carCardCount = M_pRecordset->GetCollect("carCardCount");
		m_useCarCardCount = M_pRecordset->GetCollect("useCarCardCount");
	}
	else
	{
		m_carCardCount = m_useCarCardCount = -1;
	}
}

char OperateDataBase::getHex(int n)
{
	return "0123456789ABCDEF"[n];
}

CString OperateDataBase::getNextCarCard(int _merchantIDLength, int _agreementIDLength)
{
	int _carCardIDLength = MAXLENGTH - _merchantIDLength - _agreementIDLength;
	int _nextCarCardID = m_useCarCardCount + 1;
	char *_carCardID = new char[_carCardIDLength];
	for(int i = 0; i < _carCardIDLength; i++)
		_carCardID[i] = '0';
	while(_nextCarCardID)
	{
		_carCardID[--_carCardIDLength] = getHex(_nextCarCardID % 16);
		_nextCarCardID /= 16;
	}

	//m_agreementID为18位，前9位为商户ID
	CString _ret_str = m_agreementID + _carCardID;

	return _ret_str.Left(MAXLENGTH);
}

BOOL OperateDataBase::AddUseCarCard()
{
	if((m_useCarCardCount + 1) > m_carCardCount)
		return false;
	else
		++m_useCarCardCount;
	CString _select = "update tb_AgreementInfo set useCarCardCount = useCarCardCount + 1 where merchantCardID = '" + m_merchantID + "' and agreementNo = '" + m_agreementID + "'";
	M_pRecordset = M_pConnection->Execute(_bstr_t(_select), NULL, adCmdText);
	
	return true;
}

void OperateDataBase::AddRetureCarCard()
{
	CString _select = "update tb_AgreementInfo set returnCarCardCount = returnCarCardCount + 1 where merchantCardID = '" + m_merchantID + "' and agreementNo = '" + m_agreementID + "'";
	M_pRecordset = M_pConnection->Execute(_bstr_t(_select), NULL, adCmdText);
}