#pragma once
class tb_MerchantInfo
{
public:
	CString loginName;
	CString passwd;
	CString company;
	CString merchantCardID;
	CTime registerDate;
	CString registerName;
	CString registerID;
	CString registerPhone;
	CString registerAddress;
	int loginNumber;
	CTime lastLoginTime;

	tb_MerchantInfo(void);
	~tb_MerchantInfo(void);
};

