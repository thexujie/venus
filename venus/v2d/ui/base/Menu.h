#pragma once

VENUS_BEG

class V2D_API MenuItemT : public IMenuItem
{
public:
	MenuItemT();
	MenuItemT(int_x iId, const char_16 * szText, int_x iFlags, IImage * pImage, IMenu * pSubMenu);
	MenuItemT(const MenuItemT & another);
	~MenuItemT();
	bool IsSpilit() const { return (m_iFlags & MenuFlagSplit) != 0;}
	bool IsChecked() const { return (m_iFlags & MenuFlagCheck) != 0;}
	bool IsSelected() const { return (m_iFlags & MenuFlagSelected) != 0;}
	bool IsEnable() const { return (m_iFlags & MenuFlagDisable) == 0;}
	int_x GetId() const { return m_iId;}
	const char_16 * GetText() const { return m_text;}
	IImage * GetImage() const { return m_pImage;}
	IMenu * GetSubMenu() const { return m_pSubMenu;}
	int_x GetShortKey() const { return 'B';}
	MenuItemT & operator = (const MenuItemT & another);

	int_x m_iId;
	IImage * m_pImage;
	textw m_text;
	IMenu * m_pSubMenu;
	int_x m_iFlags;
};

class V2D_API MenuT : public ObjectT<IMenu>
{
public:
	MenuT();
	~MenuT();
	void AddItem(int_x iId, const char_16 * szText, int_x iFlags = 0);
	void AddItem(int_x iId, const char_16 * szText, IMenu * pSubMenu, int_x iFlags = 0);
	void AddSplit();

	int_x GetItemCount() const;
	IMenuItem * GetItem(int_x iIndex);

private:
	vector<MenuItemT> m_items;
};

V2D_API int_x MenuGetFlags(bool bCheck);

VENUS_END
