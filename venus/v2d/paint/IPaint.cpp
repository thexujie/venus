#include "stdafx.h"
#include "IPaint.h"

VENUS_BEG

void Set2DDevice(IDevice2D * pDevice)
{
	SetService(CID_IDevice2D, pDevice);
}

IDevice2D * Device2D()
{
	return GetService<IDevice2D>(CID_IDevice2D);
}

VENUS_END
