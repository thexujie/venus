#pragma once

#include "BaseInc.h"

enum ResTypeE
{
	ResTypeFile = 0,
	ResTypeCount,
};

class IResFile : public ObjectT<IObject>
{
public:
	IResFile(ResTypeE eType, int_x iId);
	~IResFile();

public:
	const byte_t * m_pData;
	int_x m_iSize;
};

class Resources
{
public:
	static IResFile * GetFile(int_x iId);
};

