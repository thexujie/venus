#include "stdafx.h"
#include "CPanel.h"

VENUS_BEG

CPanel::CPanel()
{
	m_bNotAcceptScroll = true;
}

CPanel::~CPanel()
{

}

const cid_t & CPanel::GetOid() const
{
	return OID_Panel;
}

VENUS_END
