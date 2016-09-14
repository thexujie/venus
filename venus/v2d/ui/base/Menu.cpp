#include "stdafx.h"
#include "Menu.h"

VENUS_BEG

MenuItemT::MenuItemT() 
	: m_iId(0), m_pImage(nullptr), m_pSubMenu(nullptr), m_iFlags(MenuFlagSplit)
{
}

MenuItemT::MenuItemT(int_x iId, const char_16 * szText, int_x iFlags, IImage * pImage, IMenu * pSubMenu) 
	: m_iId(iId), m_text(szText), m_pImage(pImage), m_pSubMenu(pSubMenu), m_iFlags(iFlags)
{
	SafeAddRef(m_pImage);
	SafeAddRef(m_pSubMenu);
	//m_pImage = GetDefaultContext()->CreateImageFromFile(L"F:\\xujie\\VisualStudioFiles\\Venus\\bin32\\img\\icon1.png");
}

MenuItemT::MenuItemT(const MenuItemT & another)
{
	m_iId = another.m_iId;
	m_pImage = another.m_pImage;
	m_text = another.m_text;
	m_iFlags = another.m_iFlags;
	m_pSubMenu = another.m_pSubMenu;

	SafeAddRef(m_pImage);
	SafeAddRef(m_pSubMenu);
}

MenuItemT::~MenuItemT()
{
	SafeRelease(m_pImage);
	SafeRelease(m_pSubMenu);
}

MenuItemT & MenuItemT::operator = (const MenuItemT & another)
{
	m_iId = another.m_iId;
	m_pImage = another.m_pImage;
	m_text = another.m_text;
	m_iFlags = another.m_iFlags;
	m_pSubMenu = another.m_pSubMenu;

	SafeAddRef(m_pImage);
	SafeAddRef(m_pSubMenu);
	return *this;
}

MenuT::MenuT()
{

}

MenuT::~MenuT()
{

}

void MenuT::AddItem(int_x iId, const char_16 * szText, int_x iFlags)
{
	m_items.add(MenuItemT(iId, szText, iFlags, nullptr, nullptr));
}

void MenuT::AddItem(int_x iId, const char_16 * szText, IMenu * pSubMenu, int_x iFlags)
{
	m_items.add(MenuItemT(iId, szText, iFlags, nullptr, pSubMenu));
}

void MenuT::AddSplit()
{
	m_items.add(MenuItemT());
}

int_x MenuT::GetItemCount() const
{
	return m_items.size();
}

IMenuItem * MenuT::GetItem(int_x iIndex)
{
	return &m_items[iIndex];
}

int_x MenuGetFlags(bool bCheck)
{
	return bCheck ? MenuFlagCheck : 0;
}

VENUS_END
