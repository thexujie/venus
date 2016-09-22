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
	* @brief ����ͼ�ꡣ
	* @param pImage ָ����ͼ�ꡣ
	 */
	void SetIcon(IImage * pIcon);

	/**
	 * @brief ����ͼ��������ı���λ�á�
	 * @param eIconAlign ͼ��������ı���λ�á�
	 * @note ֻ֧���ϡ��¡������ĸ����е�һ����
	 * @throw ��������˲���֧�ֵĲ������׳� exp_illegal_argument ��
	 */
	void SetIconAlign(AlignE eIconAlign);
	
	/**
	 * @brief �������ݵĶ��뷽ʽ��ָ��������ζ��뵽�ͻ�����
	 * @param eContextAlign ���뷽ʽ��
	 * @note ֻ֧���ϡ��¡����ҡ�ˮƽ���С���ֱ�����Լ��������в���ͻ�Ķ��ߵ���ϡ�
	 */
	void SetContextAlign(AlignE eContextAlign);
	/**
	 * ��ť������һ�� DialogResultE�����ڰ�ť��ѡ��ʱ���� Enter/Space ʱ���������ڴ��ڵ� DialogResult��
	 * Ĭ��Ϊ DialogResultNone�������ᴥ���κ� SetDialogResult��
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
	//! ���ֺ�/��ͼ��Ķ��뷽ʽ��
	AlignE m_eContextAlign;
	//! ͼ��������ֵķ�λ���������ң���
	AlignE m_eIconAlign;

	DialogResultE m_eDialogResult;

public:
	static const oid_t & Oid() { return OID_Button; }
};

VENUS_END
