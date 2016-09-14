#include "stdafx.h"
#include "C3DFileDriver.h"

VENUS_BEG
C3DFileDriver::C3DFileDriver()
{

}

C3DFileDriver::~C3DFileDriver()
{

}

err_t C3DFileDriver::Initialize()
{
	return err_ok;
}

I3DFile * C3DFileDriver::GetFile(const char_16 * szFile)
{
	C3DFile * pFileData = new C3DFile();
	int_x err = pFileData->Load(szFile);
	if(err)
	{
		SafeRelease(pFileData);
		return nullptr;
	}
	else
		return pFileData;
}

VENUS_END
