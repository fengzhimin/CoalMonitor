#pragma once

#ifndef LOG_H
#define LOG_H

class Log
{
public:
	Log(void);
	~Log(void);
	static BOOL m_writeError;   //是否正在写入
	static BOOL m_writeOperate;  //是否正在写入
	CStdioFile errorOutFile, operateOutFile;
	void WriteErrorLog(CString strErrorLogInfo);
	void WriteOperateLog(CString strOperateLogInfo);
	BOOL m_loadLog;   //是否进行日志记录
};

#endif
