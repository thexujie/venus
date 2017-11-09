#include "stdafx.h"
#include "CLayout.h"

VENUS_BEG

CLayout::CLayout()
{
	m_bStatic = true;
	m_bTransparent = true;
	m_bClipChild = true;
}

CLayout::~CLayout()
{

}

const cid_t & CLayout::GetOid() const
{
	return OID_Layout;
}

VENUS_END
