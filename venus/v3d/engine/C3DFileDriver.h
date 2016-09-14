#pragma once

VENUS_BEG

class V3D_API C3DFileDriver : public ObjectT<I3DFileDriver>
{
public:
	C3DFileDriver();
	~C3DFileDriver();

	err_t Initialize();
	I3DFile * GetFile(const char_16 * szFile);
};

VENUS_END
