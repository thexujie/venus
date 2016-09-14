#include "stdafx.h"
#include "CWin32UILoaderXml.h"

VENUS_BEG

static const CUILoaderXml::ui_load_info_t DEFAULT_LOADER_MAPS_WIN32[] =
{
	{OID_CFolderTreeView, (CUILoaderXml::ui_load_info_t::ParseFunT)&CWin32UILoaderXml::ParseFolderTree, typeid(CFolderTreeView), CUILoaderXml::CreateFunTmpl<CFolderTreeView>},
};

CWin32UILoaderXml::CWin32UILoaderXml()
{
	int_x iDefCount = sizeof(DEFAULT_LOADER_MAPS_WIN32) / sizeof(ui_load_info_t);
	for(int_x cnt = 0; cnt < iDefCount; ++cnt)
		m_infos.add(DEFAULT_LOADER_MAPS_WIN32[cnt]);
}

CWin32UILoaderXml::~CWin32UILoaderXml()
{

}

err_t CWin32UILoaderXml::ParseFolderTree(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	CFolderTreeView * pFolderTree = (CFolderTreeView *)pControl;
	if(node.has_attribute(L"root"))
		pFolderTree->SetRoot(node[L"root"].value);

	return ParseControl(node, pControl, meta, defines);
}

VENUS_END
