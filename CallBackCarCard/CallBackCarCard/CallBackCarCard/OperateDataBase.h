#include "stdafx.h"

#pragma once
class OperateDataBase
{
private:
	_ConnectionPtr M_pConnection;
	_RecordsetPtr M_pRecordset;
	CString m_merchantID;   //�̻�ID
	CString m_agreementID;  //��ͬID
	
public:
	int m_carCardCount;     //��������
	int m_useCarCardCount;  //����ȡ(ʹ��)��������
	CStringArray m_agreementIDArray;
	OperateDataBase(void);
	~OperateDataBase(void);
	//�鿴�̻��Ƿ����
	BOOL MerchantIsExist(CString _merchantID);   
	//�鿴��ͬ�Ƿ����
	BOOL AgreementIsExist(CString _agreementID);
	//ͨ���̻���ID��ȡ���̻�û����ȡ�����ĺ�ͬ
	void getUnFinishAgreementByMerchantID(CString _merchantID);
	//��ȡ��������(�����ܹ������������Ѿ�ʹ�ó�������)
	void getCarCardInfo();
	//��ȡ�������
	CString getNextCarCard(int _merchantIDLength, int _agreementIDLength);
	//�������ɹ��󣬽����ݿ�������쳵��������һ����
	BOOL AddUseCarCard();
	//���տ��ɹ��󣬽����ݿ���Ļ��ճ���������һ����
	void AddRetureCarCard();
	char getHex(int n);
};

