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
	const oid_t & GetOid() const;

	void OnAdded(IControl * pContainer);
};
VENUS_END
