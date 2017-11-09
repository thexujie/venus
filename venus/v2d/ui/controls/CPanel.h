#pragma once

#include "CControl.h"

VENUS_BEG

const cid_t OID_Panel = {L"panel"};

class V2D_API CPanel : public CControl
{
public:
	CPanel();
	~CPanel();
	const cid_t & GetOid() const override;
};

VENUS_END