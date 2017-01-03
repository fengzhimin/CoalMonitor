#include "stdafx.h"
#include "tb_MerchantInfo.h"


tb_MerchantInfo::tb_MerchantInfo(void)
{
	loginName = passwd = company = merchantCardID = L"";
	registerDate = lastLoginTime = CTime::GetCurrentTime();
	registerName = registerID = registerPhone = registerAddress = L"";
	loginNumber = 0;
}


tb_MerchantInfo::~tb_MerchantInfo(void)
{
}
