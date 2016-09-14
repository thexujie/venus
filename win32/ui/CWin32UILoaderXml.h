#pragma once

VENUS_BEG

class WIN32_API CWin32UILoaderXml : public CUILoaderXml
{
public:
	CWin32UILoaderXml();
	~CWin32UILoaderXml();
	err_t ParseFolderTree(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
};

VENUS_END

