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
	const oid_t & GetOid() const;

	void OnAdded(IControl * pContainer);
};
VENUS_END
