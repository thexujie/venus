#pragma once

VENUS_BEG

/**
 * �����ڲ���ʱռλ��
 */
class V2D_API CSpace : public CControl
{
public:
	CSpace();
	~CSpace();
	const oid_t & GetOid() const override;

	void OnAdded(IControl * pContainer) override;
};
VENUS_END
