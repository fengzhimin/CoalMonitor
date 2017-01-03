#include "stdafx.h"

#pragma once
class OperateDataBase
{
private:
	_ConnectionPtr M_pConnection;
	_RecordsetPtr M_pRecordset;
	CString m_merchantID;   //商户ID
	CString m_agreementID;  //合同ID
	
public:
	int m_carCardCount;     //车卡总数
	int m_useCarCardCount;  //以领取(使用)车卡数量
	CStringArray m_agreementIDArray;
	OperateDataBase(void);
	~OperateDataBase(void);
	//查看商户是否存在
	BOOL MerchantIsExist(CString _merchantID);   
	//查看合同是否存在
	BOOL AgreementIsExist(CString _agreementID);
	//通过商户卡ID获取该商户没有领取车卡的合同
	void getUnFinishAgreementByMerchantID(CString _merchantID);
	//获取车卡数量(包括总共车卡数量和已经使用车卡数量)
	void getCarCardInfo();
	//获取车卡编号
	CString getNextCarCard(int _merchantIDLength, int _agreementIDLength);
	//当发卡成功后，将数据库里的已领车卡数量加一处理
	BOOL AddUseCarCard();
	//当收卡成功后，将数据库里的回收车卡数量加一处理
	void AddRetureCarCard();
	char getHex(int n);
};

