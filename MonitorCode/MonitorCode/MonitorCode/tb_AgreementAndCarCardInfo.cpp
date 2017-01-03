#include "stdafx.h"
#include "tb_AgreementAndCarCardInfo.h"


tb_AgreementAndCarCardInfo::tb_AgreementAndCarCardInfo(void)
{
	agreementNo = carCardID = emptyCarSize = cargoSize = plateNumber = L"";
	cargoCarSize = carShapeInfo = L"";
	carPicture1 = carPicture2 = carPicture3 = carPicture4 = carPicture5 = L"";
	recordTime = firstTime = lastTime = CTime::GetCurrentTime();
	emptyCarWeight = CarWeight = cargoWeight = cargoDensity = deductWeight = 0.0;
	abnormal = 0;
	inOut = false;
}

tb_AgreementAndCarCardInfo::~tb_AgreementAndCarCardInfo(void)
{
}
