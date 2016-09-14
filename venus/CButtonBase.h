#pragma once

VENUS_BEG

enum ButtonStateE
{
	ButtonStateNone = 0,
	ButtonStateHoving = 0x00010000,
	ButtonStatePressed = 0x00020000,
	ButtonStateChecked = 0x00040000,
};

class V2D_API CButtonBase : public CControl
{
public:
	CButtonBase();
	CButtonBase(int_x iX, int_x iY, int_x iWidth, int_x iHeight);
	~CButtonBase();

	void ModifyState(ButtonStateE eState, bool bKeep);
	bool CheckState(ButtonStateE eState) const;

public:
	void OnMouseIn(pointix point);
	void OnMouseOut(pointix point);
	void OnMouseDownL(pointix point);
	void OnMouseUpL(pointix point);

protected:
	int_x m_iState;
};

VENUS_END
