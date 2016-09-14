#pragma once

#include  "CControl.h"

VENUS_BEG

class V2D_API CStatic : public CControl
{
public:
	CStatic();
	~CStatic();
	const oid_t & GetOid() const;

	void SetImage(IImage * pImage);
	void SetTextAlign(AlignE eTextAlign);
	void SetTextWrap(TextWrapE eTextWrap);
public: // CControl
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;
	void QueryDebugMenu(int_x & iBase, IMenu * pMenu) const;
	void OnDebugMenu(int_x iBase, int_x iResult);

private:
	IImage * m_pImage;
	AlignE m_eTextAlign;
	TextWrapE m_eTextWrap;
};

VENUS_END
