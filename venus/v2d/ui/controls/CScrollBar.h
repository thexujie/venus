#pragma once

#include "CControl.h"

VENUS_BEG

class V2D_API CScrollBar : public CControl, public IScroll
{
public:
	CScrollBar();
	~CScrollBar();

	IScroll * ToIScroll() const override;
public:
	/**
	 * @brief ������Сֵ��
	 * @param iMin ��Сֵ��
	 * @note �����������Сֵ�ȵ�ǰֵҪ�󣬻��׳� exp_illegal_argument �쳣��
	 *  ���������Ӧ�����޸� Value ���޸� MinValue����õķ����ǵ��� @ref SetValues ������
	 */
	void SetMinValue(int_x iMin) override;
	/**
	 * @brief ��ѯ��Сֵ��
	 * @return ������Сֵ��
	 */
	int_x GetMinValue() const override;
	int_x GetRangeValue() const override;
	
	/**
	 * @brief �������ֵ��
	 * @param iMax ���������ֵ��
	 * @note ������������ֵ�ȵ�ǰֵҪС�����׳� exp_illegal_argument �쳣��
	 *  ���������Ӧ�����޸� Value ���޸� MaxValue����õķ����ǵ��� @ref SetValues ������
	 */
	void SetMaxValue(int_x iMax) override;
	/**
	 * @brief ��ѯ���ֵ��
	 * @return �������ֵ��
	 */
	int_x GetMaxValue() const override;
	
	/**
	 * @brief ���õ�ǰֵ��
	 * @param iValue ������ֵ��
	 * @note ���������ֵС����Сֵ���򽫽������趨Ϊ��Сֵ�����������ֵ�������ֵ���򽫽������趨Ϊ���ֵ��
	 * �����׳��쳣��
	 */
	void SetValue(int_x iValue) override;
	/**
	 * @brief ��ѯ��ǰֵ��
	 * @return ���ص�ǰֵ��
	 */
	int_x GetValue() const override;

	/**
	 * @brief ������Сֵ�����ֵ�͵�ǰֵ��������ֵ�����ǺϷ��ģ������׳� exp_illegal_argument �쳣��
	 * @param iMin ��Сֵ��
	 * @param iMax ���ֵ��
	 * @param iValue ��ǰֵ��
	 */
	void SetRangeInfo(int_x iMin, int_x iMax, int_x iValue) override;

	/**
	 * @copybrief SetRangeInfo(float_32 fMin, float_32 fMax, float_32 fValue) ��
	 * @param range ��Χ��Ϣ����Сֵ�����ֵ����ǰֵ��
	 */
	void SetRangeInfo(const RangeInfoT & range) override;

	/**
	 * @brief ��ȡ��Сֵ�����ֵ�͵�ǰֵ��
	 * @param piMin ������Сֵ��ָ�롣
	 * @param piMax �������ֵ��ָ�롣
	 * @param piValue ���յ�ǰֵ��ָ�롣
	 */
	void GetRangeInfo(int_x * piMin, int_x * piMax, int_x * piValue) const override;

	/**
	 * @copybrief GetRangeInfo(float_32 * pfMin, float_32 * pfMax, float_32 * pfValue) ��
	 * @return ���ط�Χ��Ϣ��
	 */
	RangeInfoT GetRangeInfo() const;
	/**
	 * @brief ��ȡ��ǰ����(0 ~ 1.0f)��
	 * @return ���ص�ǰ������ȡֵ��Χ��[0, 1.0f]��
	 */
	float_32 GetRate() const override;

	void SetDirection(AxisE eDirection) override;
	AxisE GetDirection() const override;
	void SetLineValue(int_x iLine) override;
	int_x GetLineValue() const override;
	void SetPageValue(int_x iPage) override;
	int_x GetPageValue() const override;
	void SetScrollInfo(int_x iMin, int_x iMax, int_x iValue, int_x iLine, int_x iPage) override;
	void SetScrollInfo(const ScrollInfoT & scroll) override;
	ScrollInfoT GetScrollInfo() const override;
	void LineUp() override;
	void PageUp() override;
	void LineDown() override;
	void PageDown() override;

	void MinValueChanged() override;
	void MaxValueChanged() override;
	void ValueChanged(int_x iOffset) override;
	void LineValueChanged() override;
	void PageValueChanged() override;

	void SetScrollTarget(IControl * pScrollTarget) override;
	IControl * GetScrollTarget() const override;
public:
	const oid_t & GetOid() const override;
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;
	void OnMouseDownL(pointix point) override;
	void OnMouseUpL(pointix point) override;
	void OnMouseMove(pointix point) override;
	void OnMouseOut(pointix point) override;
protected:
	ScrollPointE GetMouseScrollPoint(pointix point, int_x * piBarOff = nullptr) const;
	void SetScrollPoint(ScrollPointE eScrollPoint);

	void GetBarInfoX(int_x iValue, int_x * piLineW, int_x * piBarX, int_x * piBarW) const;
	void GetBarInfoY(int_x iValue, int_x * piLineH, int_x * piBarY, int_x * piBarH) const;

	void DrawScrollX(IPaint * pPaint) const;
	void DrawScrollY(IPaint * pPaint) const;

protected:
	//! �������ķ���(AxisX AxisY)
	AxisE m_eDirection;
	//! ��Сֵ(��ʼ��Ϊ0.0f)��
	int_x m_iMin;
	//! ���ֵ(��ʼ��Ϊ100.0f)��
	int_x m_iMax;
	//! ��ǰֵ(��ʼ��Ϊ50.0f)��
	int_x m_iValue;
	//! ����һ��ʱ����ǰֵ������(��ʼֵ��1.0f)��
	int_x m_iLine;
	//! ����һҳʱ����ǰֵ������(��ʼֵ��10.0f)��
	int_x m_iPage;
	//! �������ĵ�ǰλ�á�
	ScrollPointE m_eScrollPoint;
	//! �Ƿ�������ק��
	bool m_bDraging;
	//! ��ק��ʼʱ���λ�úͻ�����ʼ��Ե�� ƫ�ơ�
	int_x m_iDragOff;
	//! ���������ƵĿɹ�������
	IControl * m_pScrollTarget;
public:
	event<IControl *, int_x, int_x> OnValueChanged;
};

VENUS_END
