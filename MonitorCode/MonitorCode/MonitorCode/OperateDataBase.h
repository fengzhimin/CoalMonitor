#include "stdafx.h"
#include "tb_AgreementAndCarCardInfo.h"
#include "tb_CarCheckInfo.h"

#pragma once
class OperateDataBase
{
private:
	_ConnectionPtr M_pConnection;
	_RecordsetPtr M_pRecordset;

public:
	OperateDataBase(void);
	~OperateDataBase(void);
	tb_CarCheckInfo getLastRecordByCarCardID(CString _carCardID);
	tb_CarCheckInfo getPrevRecordByCarCardID(CString _carCardID);
	void setAbnormalToCarCheckInfoTable(CString _carCardID, int _value);
};

