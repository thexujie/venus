#include "stdafx.h"
#include "CSpace.h"

VENUS_BEG

CSpace::CSpace()
{
	m_bStatic = true;
	m_bTransparent = true;
	m_bDisSelectAble = true;
}

CSpace::~CSpace()
{

}

const cid_t & CSpace::GetOid() const
{
	return OID_Space;
}

void CSpace::OnAdded(IControl * pContainer)
{
	switch(pContainer->GetLayoutMode())
	{
	case LayoutModeAbsolute:
		break;
	case LayoutModeHorizontal:
		m_eWidthMode = WHModeAbs;
		m_eHeightMode = WHModeFill;
		break;
	case LayoutModeVertical:
		m_eWidthMode = WHModeFill;
		m_eHeightMode = WHModeAbs;
		break;
	case LayoutModeTable:
		break;
	default:
		break;
	}
	CControl::OnAdded(pContainer);
}

VENUS_END
