#include "stdafx.h"
#include "Log.h"
#include <locale>

#define ERROR_FILE  "./error.log";
#define OPERATE_FILE  "./operate.log";

BOOL Log::m_writeError = false;
BOOL Log::m_writeOperate = false;

Log::Log(void)
{
	CString _temp;
	GetPrivateProfileStringA("Load Log Info", "Config", "FALSE", _temp.GetBuffer(MAX_PATH),MAX_PATH,"./Config.ini");
	_temp.ReleaseBuffer();
	if(_temp == L"TRUE")
		m_loadLog = true;
	else
		m_loadLog = false;
}

Log::~Log(void)
{
	
}

void Log::WriteErrorLog(CString strErrorLogInfo)
{
	if(m_loadLog)
	{
		while (m_writeError)
			Sleep(100);   //防止死锁
		m_writeError = true;
		try
		{
			CString strErrorPath = ERROR_FILE;
			errorOutFile.Open(strErrorPath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
			CString strData;
			CTime tt = CTime::GetCurrentTime();
			strData = "Error: " + tt.Format("[%Y-%B-%d %A, %H:%M:%S] ") + strErrorLogInfo;
			strData += "\n";
			char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
			setlocale( LC_CTYPE, "chs" );//新的区域设定
			errorOutFile.SeekToEnd();
			errorOutFile.WriteString( strData );
			setlocale( LC_CTYPE, old_locale );  
			free( old_locale );//还原区域设定
			errorOutFile.Close();
		}
		catch(CFileException *fx)
		{
			fx->Delete();
		}
		m_writeError = false;
	}
}

void Log::WriteOperateLog(CString strOperateLogInfo)
{
	if(m_loadLog)
	{
		while (m_writeOperate)
			Sleep(100);   //防止死锁
		m_writeOperate = true;
		try
		{
			CString strOperatePath = OPERATE_FILE;
			operateOutFile.Open(strOperatePath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
			CString strData;
			CTime tt = CTime::GetCurrentTime();
			strData = "Operate: " + tt.Format("[%Y-%B-%d %A, %H:%M:%S] ") + strOperateLogInfo;
			strData += "\n";
			char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
			setlocale( LC_CTYPE, "chs" );//新的区域设定
			operateOutFile.SeekToEnd();
			operateOutFile.WriteString( strData );
			setlocale( LC_CTYPE, old_locale );  
			free( old_locale );//还原区域设定
			operateOutFile.Close();
		}
		catch(CFileException *fx)
		{
			fx->Delete();
		}
		m_writeOperate = false;
	}
}