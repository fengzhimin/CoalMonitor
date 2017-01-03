#include "stdafx.h"
#include "tb_AgreementInfo.h"


tb_AgreementInfo::tb_AgreementInfo(void)
{
	agreementNo = merchantCardID = L"";
	signDate = validatyPeriod = CTime::GetCurrentTime();
	coalTon = actualCoalTon = 0.0;
	carCardCount = useCarCardCount = returnCarCardCount = 0;
	agreementStatus = false;
}


tb_AgreementInfo::~tb_AgreementInfo(void)
{
}
