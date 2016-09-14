#pragma once

#include "GdipInc.h"

#ifdef WIN32_DEVICE_GDIP

VENUS_BEG

class GdipModule
{
public:
	GdipModule();
	~GdipModule();

	err_t Initialize();

private:
	HMODULE m_hModule;
};

VENUS_END

#endif //WIN32_DEVICE_GDIP
