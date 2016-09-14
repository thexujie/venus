#pragma once

#include "CControl.h"

VENUS_BEG

const oid_t OID_Panel = {L"panel"};

class V2D_API CPanel : public CControl
{
public:
	CPanel();
	~CPanel();
	const oid_t & GetOid() const;
};

VENUS_END