#ifndef _DEVICETHREADPARAMETER
#define _DEVICETHREADPARAMETER

#include "CarCardThreadParameter.h"
#include "DeviceOperation.h"

struct DeviceThreadParameter
{
	CarCardThreadParameter m_carCardThreadParameter;
	CDeviceOperation *m_deviceOperation;   //�豸��������
};
#endif