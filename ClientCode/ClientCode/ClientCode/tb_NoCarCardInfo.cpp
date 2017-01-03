#include "stdafx.h"
#include "tb_NoCarCardInfo.h"


tb_NoCarCardInfo::tb_NoCarCardInfo(void)
{
	agreementNo = carCardID = emptyCarSize = cargoSize = plateNumber = L"";
	cargoCarSize = L"";
	carPicture1 = carPicture2 = carPicture3 = carPicture4 = carPicture5 = L"";
	recordTime = firstTime = lastTime = CTime::GetCurrentTime();
	emptyCarWeight = CarWeight = cargoWeight = cargoDensity = deductWeight = 0.0;
	reasonNumber = 0;
	inOut = false;
}


tb_NoCarCardInfo::~tb_NoCarCardInfo(void)
{
}
