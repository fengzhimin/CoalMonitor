#include "stdafx.h"
#include "tb_AgreementAndCarCardInfo.h"
#include "tb_AgreementInfo.h"
#include "tb_CarCheckInfo.h"
#include "tb_MerchantInfo.h"
#include "tb_NoCarCardInfo.h"

#pragma once
class OperateDataBase
{
private:
	_ConnectionPtr M_pConnection;
	_RecordsetPtr M_pRecordset;

public:
	OperateDataBase(void);
	~OperateDataBase(void);
	void Add(tb_AgreementAndCarCardInfo _agreementAndCarCardInfo);
	void Add(tb_AgreementInfo _agreementInfo);
	void Add(tb_CarCheckInfo _carCheckInfo);
	void Add(tb_MerchantInfo _merchantInfo);
	void Add(tb_NoCarCardInfo _noCarCardInfo);
};

