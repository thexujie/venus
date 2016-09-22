#pragma once

#include "CButtonBase.h"

VENUS_BEG

class V2D_API CButton : public CButtonBase
{
public:
	CButton();
	~CButton();

	const oid_t & GetOid() const override;
	
	/**
	* @brief 设置图标。
	* @param pImage 指定的图标。
	 */
	void SetIcon(IImage * pIcon);

	/**
	 * @brief 设置图标相对于文本的位置。
	 * @param eIconAlign 图标相对于文本的位置。
	 * @note 只支持上、下、左、右四个当中的一个。
	 * @throw 如果给出了不受支持的参数，抛出 exp_illegal_argument 。
	 */
	void SetIconAlign(AlignE eIconAlign);
	
	/**
	 * @brief 设置内容的对齐方式，指定内容如何对齐到客户区。
	 * @param eContextAlign 对齐方式。
	 * @note 只支持上、下、左、右、水平居中、垂直居中以及他们其中不冲突的二者的组合。
	 */
	void SetContextAlign(AlignE eContextAlign);
	/**
	 * 按钮关联的一个 DialogResultE，这在按钮被选中时按下 Enter/Space 时会设置所在窗口的 DialogResult。
	 * 默认为 DialogResultNone，即不会触发任何 SetDialogResult。
	 */
	void SetDialogResult(DialogResultE eDialogResult);
	DialogResultE GetDialogResult() const;
public:
	void OnTextChanged() override;
	sizeix GetContentSize() const override;

	/**
	 * @copydoc CControl::OnPaint
	 */
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;
	void OnKeyDown(KeyCodeE eKeyCode) override;
	void OnKeyUp(KeyCodeE eKeyCode) override;
	bool OnHotKey() override;

public:
	void OnMouseClickL(pointix point) override;
protected:

	void DrawIconOnly(IPaint * pPaint) const;
	void DrawTextOnly(IPaint * pPaint) const;
	void DrawIconText(IPaint * pPaint) const;

	int_x GetTextSize() const;
	sizeix GetIconSize() const;
	void DrawButtonIcon(IPaint * pPaint, int_x iX, int_x iY) const;
	void DrawButtonText(IPaint * pPaint, int_x iX, int_x iY) const;

protected:
	IImage * m_pIcon;
	//! 文字和/或图标的对齐方式。
	AlignE m_eContextAlign;
	//! 图标相对文字的方位（上下左右）。
	AlignE m_eIconAlign;

	DialogResultE m_eDialogResult;

public:
	static const oid_t & Oid() { return OID_Button; }
};

VENUS_END
