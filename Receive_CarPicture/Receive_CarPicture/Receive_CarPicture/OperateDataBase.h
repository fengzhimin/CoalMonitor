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
	*���ܣ�����һ����¼�ĳ��ƺ�
	*����˵����
	*_carCardID������ID
	*_weighPlace������վ���
	*_inOut����/�� 
	*_plateNumber�����ƺ�
	*_tableName����Ҫ���µı���
	*/
	void updateCar(CString _carCardID, CString _weighPlace, CString _inOut, CString _plateNumber, CString _tableName);
};

