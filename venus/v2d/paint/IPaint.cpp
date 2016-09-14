#include "stdafx.h"
#include "IPaint.h"

VENUS_BEG

void Set2DDevice(I2DDevice * pDevice)
{
	SetService(OID_IDevice2D, pDevice);
}

I2DDevice * Get2DDevice()
{
	return GetService<I2DDevice>(OID_IDevice2D);
}

VENUS_END
