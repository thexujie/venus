#pragma once

#include "CControl.h"

VENUS_BEG

class V2D_API CLayout : public CControl
{
public:
	CLayout();
	~CLayout();
	const oid_t & GetOid() const;
};

VENUS_END

