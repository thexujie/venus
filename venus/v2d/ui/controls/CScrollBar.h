#pragma once

#include "CControl.h"

VENUS_BEG

class V2D_API CScrollBar : public CControl, public IScroll
{
public:
	CScrollBar();
	~CScrollBar();

	IScroll * ToIScroll() const;
public:
	/**
	 * @brief 设置最小值。
	 * @param iMin 最小值。
	 * @note 如果给定的最小值比当前值要大，会抛出 exp_illegal_argument 异常。
	 *  这种情况下应该先修改 Value 再修改 MinValue，最好的方法是调用 @ref SetValues 函数。
	 */
	void SetMinValue(int_x iMin);
	/**
	 * @brief 查询最小值。
	 * @return 返回最小值。
	 */
	int_x GetMinValue() const;
	int_x GetRangeValue() const;
	
	/**
	 * @brief 设置最大值。
	 * @param iMax 给定的最大值。
	 * @note 如果给定的最大值比当前值要小，会抛出 exp_illegal_argument 异常。
	 *  这种情况下应该先修改 Value 再修改 MaxValue，最好的方法是调用 @ref SetValues 函数。
	 */
	void SetMaxValue(int_x iMax);
	/**
	 * @brief 查询最大值。
	 * @return 返回最大值。
	 */
	int_x GetMaxValue() const;
	
	/**
	 * @brief 设置当前值。
	 * @param iValue 给定的值。
	 * @note 如果给定的值小于最小值，则将进度条设定为最小值；如果给定的值大于最大值，则将进度条设定为最大值。
	 * 不会抛出异常。
	 */
	void SetValue(int_x iValue);
	/**
	 * @brief 查询当前值。
	 * @return 返回当前值。
	 */
	int_x GetValue() const;

	/**
	 * @brief 设置最小值、最大值和当前值。给定的值必须是合法的，否则抛出 exp_illegal_argument 异常。
	 * @param iMin 最小值。
	 * @param iMax 最大值。
	 * @param iValue 当前值。
	 */
	void SetRangeInfo(int_x iMin, int_x iMax, int_x iValue);

	/**
	 * @copybrief SetRangeInfo(float_32 fMin, float_32 fMax, float_32 fValue) 。
	 * @param range 范围信息，最小值、最大值、当前值。
	 */
	void SetRangeInfo(const RangeInfoT & range);

	/**
	 * @brief 获取最小值、最大值和当前值。
	 * @param piMin 接收最小值的指针。
	 * @param piMax 接收最大值的指针。
	 * @param piValue 接收当前值的指针。
	 */
	void GetRangeInfo(int_x * piMin, int_x * piMax, int_x * piValue) const;

	/**
	 * @copybrief GetRangeInfo(float_32 * pfMin, float_32 * pfMax, float_32 * pfValue) 。
	 * @return 返回范围信息。
	 */
	RangeInfoT GetRangeInfo() const;
	/**
	 * @brief 获取当前比例(0 ~ 1.0f)。
	 * @return 返回当前比例。取值范围是[0, 1.0f]。
	 */
	float_32 GetRate() const;

	void SetDirection(AxisE eDirection);
	AxisE GetDirection() const;
	void SetLineValue(int_x iLine);
	int_x GetLineValue() const;
	void SetPageValue(int_x iPage);
	int_x GetPageValue() const;
	void SetScrollInfo(int_x iMin, int_x iMax, int_x iValue, int_x iLine, int_x iPage);
	void SetScrollInfo(const ScrollInfoT & scroll);
	ScrollInfoT GetScrollInfo() const;
	void LineUp();
	void PageUp();
	void LineDown();
	void PageDown();

	void MinValueChanged();
	void MaxValueChanged();
	void ValueChanged();
	void LineValueChanged();
	void PageValueChanged();

	void SetScrollTarget(IControl * pScrollTarget);
	IControl * GetScrollTarget() const;
public:
	const oid_t & GetOid() const;
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;
	void OnMouseDownL(pointix point);
	void OnMouseUpL(pointix point);
	void OnMouseMove(pointix point);
	void OnMouseOut(pointix point);
protected:
	ScrollPointE GetMouseScrollPoint(pointix point, int_x * piBarOff = nullptr) const;
	void SetScrollPoint(ScrollPointE eScrollPoint);

	void GetBarInfoX(int_x iValue, int_x * piLineW, int_x * piBarX, int_x * piBarW) const;
	void GetBarInfoY(int_x iValue, int_x * piLineH, int_x * piBarY, int_x * piBarH) const;

	void DrawScrollX(IPaint * pPaint) const;
	void DrawScrollY(IPaint * pPaint) const;

protected:
	//! 滚动条的方向(AxisX AxisY)
	AxisE m_eDirection;
	//! 最小值(初始化为0.0f)。
	int_x m_iMin;
	//! 最大值(初始化为100.0f)。
	int_x m_iMax;
	//! 当前值(初始化为50.0f)。
	int_x m_iValue;
	//! 滚动一行时，当前值的增量(初始值是1.0f)。
	int_x m_iLine;
	//! 滚动一页时，当前值的增量(初始值是10.0f)。
	int_x m_iPage;
	//! 滚动条的当前位置。
	ScrollPointE m_eScrollPoint;
	//! 是否正在拖拽。
	bool m_bDraging;
	//! 拖拽开始时鼠标位置和滑块起始边缘的 偏移。
	int_x m_iDragOff;
	//! 滚动条控制的可滚动对象。
	IControl * m_pScrollTarget;
public:
	event<IControl *, int_x> OnValueChanged;
};

VENUS_END
