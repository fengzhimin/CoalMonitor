#pragma once

#ifndef LOG_H
#define LOG_H

class Log
{
public:
	Log(void);
	~Log(void);
	static BOOL m_writeError;   //�Ƿ�����д��
	static BOOL m_writeOperate;  //�Ƿ�����д��
	CStdioFile errorOutFile, operateOutFile;
	void WriteErrorLog(CString strErrorLogInfo);
	void WriteOperateLog(CString strOperateLogInfo);
	BOOL m_loadLog;   //�Ƿ������־��¼
};

#endif
