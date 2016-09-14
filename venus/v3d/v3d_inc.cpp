#include "stdafx.h"
#include "v3d_inc.h"

VENUS_BEG

I3DFileDriver * Get3DFileDriver()
{
	return GetService<I3DFileDriver>(OID_I3DFileDriver);
}

I3DVedioDriver * Get3DVedioDriver()
{
	return GetService<I3DVedioDriver>(OID_I3DVedioDriver);
}

VENUS_END
