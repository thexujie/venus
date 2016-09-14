#pragma once

#include "BaseInc.h"

class CDockFloatForm : public CForm
{
public:
	CDockFloatForm(IControl * pControl);
	~CDockFloatForm();
	int_x GetCaptionHeight() const;

	void OnCreate();
	void OnPositionChanged();
protected:
	CDockFrame m_frame;
};
