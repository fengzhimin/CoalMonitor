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
	/**
	*功能：更新一条记录的车牌号
	*参数说明：
	*_carCardID：车卡ID
	*_weighPlace：拉货站点号
	*_inOut：进/出 
	*_plateNumber：车牌号
	*_tableName：需要更新的表名
	*/
	void updateCar(CString _carCardID, CString _weighPlace, CString _inOut, CString _plateNumber, CString _tableName);
};

