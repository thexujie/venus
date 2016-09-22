#pragma once

VENUS_BEG

/**
 * 仅用于布局时占位用
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
