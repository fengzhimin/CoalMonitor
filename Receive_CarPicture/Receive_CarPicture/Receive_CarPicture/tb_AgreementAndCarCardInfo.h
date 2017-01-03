#pragma once
class tb_AgreementAndCarCardInfo
{
public:
	CString agreementNo;
	CString carCardID;
	CString plateNumber;
	CString emptyCarSize;
	CString cargoSize;
	CString cargoCarSize;
	CString carShapeInfo;
	float emptyCarWeight;
	float CarWeight;
	float cargoWeight;
	float cargoDensity;
	float deductWeight;
	CString carPicture1;
	CString carPicture2;
	CString carPicture3;
	CString carPicture4;
	CString carPicture5;
	CString weighPlace;
	int abnormal;
	bool inOut;
	CTime recordTime;
	CTime firstTime;
	CTime lastTime;

	tb_AgreementAndCarCardInfo(void);
	~tb_AgreementAndCarCardInfo(void);

};

