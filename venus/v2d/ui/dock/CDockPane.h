#pragma once

#include "../controls/CPane.h"
#include "IDock.h"

VENUS_BEG

const cid_t OID_DockPane = {L"dockpane"};
class V2D_API CDockPane : public CPane
{
public:
	CDockPane();
	~CDockPane();
	const cid_t & GetOid() const;
};

VENUS_END
