#pragma once
class tb_AgreementInfo
{
public:
	CString agreementNo;
	CString merchantCardID;
	CTime signDate;
	CTime validatyPeriod;
	float coalTon;
	int carCardCount;
	int useCarCardCount;
	float actualCoalTon;
	int returnCarCardCount;
	bool agreementStatus;

	tb_AgreementInfo(void);
	~tb_AgreementInfo(void);
};

