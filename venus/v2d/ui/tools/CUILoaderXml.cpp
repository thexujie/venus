#include "stdafx.h"
#include "CUILoaderXml.h"

VENUS_BEG

static intx2 to_intx2(const textw & str)
{
	const char_16 * szText = str.buffer();
	const char_16 * szEnd = szText + str.size();
	char_16 * szStop = 0;

	intx2 vec;
	vec.x = wcstol(szText, &szStop, 10);
	if(szEnd > szStop)
		vec.y = wcstol(szStop + 1, &szStop, 10);
	return vec;
};

static edgeix to_edge(const textw & str)
{
	const char_16 * szText = str.buffer();
	const char_16 * szEnd = szText + str.size();
	char_16 * szStop = 0;

	edgeix edge;
	edge.left = wcstol(szText, &szStop, 10);
	if(szEnd > szStop)
		edge.top = wcstol(szStop + 1, &szStop, 10);
	if(szEnd > szStop)
		edge.right = wcstol(szStop + 1, &szStop, 10);
	if(szEnd > szStop)
		edge.bottom = wcstol(szStop + 1, &szStop, 10);
	return edge;
};

static rectix to_rect(const textw & str)
{
	const char_16 * szText = str.buffer();
	const char_16 * szEnd = szText + str.size();
	char_16 * szStop = 0;

	rectix rect;
	rect.x = wcstol(szText, &szStop, 10);
	if(szEnd > szStop)
		rect.y = wcstol(szStop + 1, &szStop, 10);
	if(szEnd > szStop)
		rect.w = wcstol(szStop + 1, &szStop, 10);
	if(szEnd > szStop)
		rect.h = wcstol(szStop + 1, &szStop, 10);
	return rect;
};

static AlignE to_align(const textw & str)
{
	AlignE align = AlignNone;
	for(int_x cnt = 0; cnt < str.length(); ++cnt)
	{
		switch(str[cnt])
		{
		case L'l': align = AlignLeft; break;
		case L't': align = AlignTop; break;
		case L'r': align = AlignRight; break;
		case L'b': align = AlignBottom; break;
		case L'h': align = AlignCenterX; break;
		case L'v': align = AlignCenterY; break;
		default:
			break;
		}
	}
	return align;
};

static anchor_t to_anchor(const textw & str)
{
	anchor_t anchor;
	for(int_x cnt = 0; cnt < str.length(); ++cnt)
	{
		switch(str[cnt])
		{
		case L'l': anchor.type |= AlignLeft; break;
		case L't': anchor.type |= AlignTop; break;
		case L'r': anchor.type |= AlignRight; break;
		case L'b': anchor.type |= AlignBottom; break;
		case L'h': anchor.type |= AlignCenterX; break;
		case L'v': anchor.type |= AlignCenterY; break;
		default:
			break;
		}
	}
	return anchor;
};

static ImeModeE to_imemode(const textw & str)
{
	if(str.equals(L"default", -1, false))
		return ImeModeDefault;
	else if(str.equals(L"on", -1, false))
		return ImeModeOn;
	else if(str.equals(L"off", -1, false))
		return ImeModeOff;
	else if(str.equals(L"disable", -1, false))
		return ImeModeDisable;
	else
		return ImeModeDefault;
};

static LayoutModeE to_layoutmode(const textw & str)
{
	if(str.equals(L"absolute", -1, false))
		return LayoutModeAbsolute;
	if(str.equals(L"horizontal", -1, false))
		return LayoutModeHorizontal;
	else if(str.equals(L"vertical", -1, false))
		return LayoutModeVertical;
	else if(str.equals(L"table", -1, false))
		return LayoutModeTable;
	else
		return LayoutModeAbsolute;
};

static BorderTypeE to_bordertype(const textw & str)
{
	if(str.equals(L"none", 4, false))
		return BorderTypeNone;
	if(str.equals(L"border1", 7, false))
		return BorderType1;
	if(str.equals(L"border2", 7, false))
		return BorderType2;
	if(str.equals(L"border3", 7, false))
		return BorderType3;
	if(str.equals(L"border4", 7, false))
		return BorderType4;
	if(str.equals(L"border5", 7, false))
		return BorderType5;
	if(str.equals(L"border1ia", 9, false))
		return BorderType1Inactive;
	if(str.equals(L"border2ia", 9, false))
		return BorderType2Inactive;
	if(str.equals(L"border3ia", 9, false))
		return BorderType3Inactive;
	if(str.equals(L"border4ia", 9, false))
		return BorderType4Inactive;
	if(str.equals(L"border5ia", 9, false))
		return BorderType5Inactive;
	if(str.equals(L"user", 4, false))
		return BorderTypeUser;
	else
		return BorderTypeNone;
};

static WHModeE to_whmode(const textw & str)
{
	if(str.equals(L"fill", -1, false))
		return WHModeFill;
	else if(str.equals(L"auto", -1, false))
		return WHModeAuto;
	else
		return WHModeAbs;
};

static const CUILoaderXml::ui_load_info_t DEFAULT_LOADER_MAPS[] =
{
	{OID_Control, &CUILoaderXml::ParseControl, typeid(CControl), CUILoaderXml::CreateFunTmpl<CControl>},
	{OID_Space, &CUILoaderXml::ParseControl, typeid(CSpace), CUILoaderXml::CreateFunTmpl<CSpace>},
	{OID_Form, &CUILoaderXml::ParseForm, typeid(CForm), CUILoaderXml::CreateFunTmpl<CForm>},
	{OID_Static, &CUILoaderXml::ParseStatic, typeid(CStatic), CUILoaderXml::CreateFunTmpl<CStatic>},
	{OID_Lable, &CUILoaderXml::ParseControl, typeid(CLabel), CUILoaderXml::CreateFunTmpl<CLabel>},
	{OID_Button, &CUILoaderXml::ParseButton, typeid(CButton), CUILoaderXml::CreateFunTmpl<CButton>},
	{OID_Radio, &CUILoaderXml::ParseControl, typeid(CRadio), CUILoaderXml::CreateFunTmpl<CRadio>},
	{OID_CheckBox, &CUILoaderXml::ParseControl, typeid(CCheckBox), CUILoaderXml::CreateFunTmpl<CCheckBox>},
	{OID_ScrollBar, &CUILoaderXml::ParseControl, typeid(CScrollBar), CUILoaderXml::CreateFunTmpl<CScrollBar>},
	{OID_ProgressBar, &CUILoaderXml::ParseProgressBar, typeid(CProgressBar), CUILoaderXml::CreateFunTmpl<CProgressBar>},
	{OID_TextLine, &CUILoaderXml::ParseTextLine, typeid(CTextLine), CUILoaderXml::CreateFunTmpl<CTextLine>},
	{OID_ListBox, &CUILoaderXml::ParseControl, typeid(CListBox), CUILoaderXml::CreateFunTmpl<CListBox>},
	{OID_TextBox, &CUILoaderXml::ParseTextBox, typeid(CTextBox), CUILoaderXml::CreateFunTmpl<CTextBox>},
	{OID_GroupBox, &CUILoaderXml::ParseControl, typeid(CGroupBox), CUILoaderXml::CreateFunTmpl<CGroupBox>},
	{OID_TabControl, &CUILoaderXml::ParseTabControl, typeid(CTabControl), CUILoaderXml::CreateFunTmpl<CTabControl>},
	{OID_MenuBar, &CUILoaderXml::ParseMenuBar, typeid(CMenuBar), CUILoaderXml::CreateFunTmpl<CMenuBar>},
	{OID_Layout, &CUILoaderXml::ParseLayout, typeid(CLayout), CUILoaderXml::CreateFunTmpl<CLayout>},
	{OID_Panel, &CUILoaderXml::ParseControl, typeid(CPanel), CUILoaderXml::CreateFunTmpl<CPanel>},
	{OID_Pane, &CUILoaderXml::ParsePane, typeid(CPane), CUILoaderXml::CreateFunTmpl<CPane>},

	{OID_ListView, &CUILoaderXml::ParseListView, typeid(CListView), CUILoaderXml::CreateFunTmpl<CListView>},
	{OID_TreeView, &CUILoaderXml::ParseTreeView, typeid(CTreeView), CUILoaderXml::CreateFunTmpl<CTreeView>},
	{OID_FoldingView, &CUILoaderXml::ParseScrollView, typeid(CFoldingView), CUILoaderXml::CreateFunTmpl<CFoldingView>},

	{OID_Pane, &CUILoaderXml::ParseControl, typeid(CPane), CUILoaderXml::CreateFunTmpl<CPane>},
	{OID_DockPane, &CUILoaderXml::ParseControl, typeid(CDockPane), CUILoaderXml::CreateFunTmpl<CDockPane>},
	{OID_DockFrame, &CUILoaderXml::ParseDockFrame, typeid(CDockFrame), CUILoaderXml::CreateFunTmpl<CDockFrame>},
};

CUILoaderXml::CUILoaderXml()
{
	int_x iDefCount = sizeof(DEFAULT_LOADER_MAPS) / sizeof(ui_load_info_t);
	for(int_x cnt = 0; cnt < iDefCount; ++cnt)
		m_infos.add(DEFAULT_LOADER_MAPS[cnt]);
}

CUILoaderXml::~CUILoaderXml()
{

}

void CUILoaderXml::Load(const char_x * szFile, IControl * pControl)
{
	textw text = CTextReader::ReadAllFile(szFile);
	Load(text.buffer(), text.length(), pControl);
}

void CUILoaderXml::Load(const char_16 * szText, int_x iLength, IControl * pControl)
{
	xml_doc doc;
	doc.load_text(szText, iLength);

	CLoaderDefines defines;
	if(doc.name.equals(L"ui", 2, false))
	{
		for(int_x cnt = 0, size = doc.children.size(); cnt < size; ++cnt)
		{
			const xml_node & child = doc.children[cnt];
			if(child.name.equals(L"defines", 7, false))
			{
				defines.load(child);
				break;
			}
		}

		for(int_x cnt = 0, size = doc.children.size(); cnt < size; ++cnt)
		{
			const xml_node & child = doc.children[cnt];
			if(child.name.equals(L"form", 4, false))
			{
				Load(child, pControl, defines);
				break;
			}
		}
	}
	else
		Load(doc, pControl, defines);
}

void CUILoaderXml::Load(const xml_node & node, IControl * pControl)
{
	CMetaHelper meta;
	CLoaderDefines defines;
	pControl->GetMeta(&meta);
	Parse(node, pControl, meta, defines);
	pControl->OnVisualSizeChanged();
}

void CUILoaderXml::Load(const xml_node & node, IControl * pControl, CLoaderDefines & defines)
{
	CMetaHelper meta;
	pControl->GetMeta(&meta);
	Parse(node, pControl, meta, defines);
}

err_t CUILoaderXml::Parse(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	const ui_load_info_t * pInfo = FindInfo(node);
	if(!pInfo)
		return err_no_context;

	textw name = node[L"name"].value;
	if(!pControl)
	{
		meta_t * pInstance = meta.find_meta(name);
		if(pInstance && pInstance->info == pInfo->tinfo)
			pControl = (IControl *)pInstance->data;
		else
		{
			pControl = pInfo->CreateFun();
			verify(pControl);
			if(pControl)
				pControl->SetDynamic(true);
		}
	}
	else
	{
	}
	pControl->SetName(name);

	// 先注册消息。
	CMetaHelper metaChild;
	pControl->GetMeta(&metaChild);
	for(int_x cnt = 0, size = node.attributes.size(); cnt < size; ++cnt)
	{
		const xml_attr & attr = node.attributes[cnt];
		ParseEvent(attr.name, attr.value, metaChild, meta);
	}

	// 再解释属性。
	err_t err = (this->*(pInfo->ParseFun))(node, pControl, meta, defines);
	if(err == err_ok)
	{
		ParseChild(node, pControl, meta, defines);
		pControl->OnLoaded();
		return err_ok;
	}
	else if(err == err_end)
	{
		// ParseFun 自己处理了 children.
		return err_ok;
	}
	else
		return err;
}

err_t CUILoaderXml::ParseChild(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	for(int_x cnt = 0; cnt < node.children.size(); ++cnt)
	{
		const xml_node & child = node.children[cnt];
		if(child.name.equals(L"defines", 7, false))
		{
			defines.load(child);
			continue;
		}

		IControl * pChild = nullptr;
		err_t err = Parse(child, pChild, meta, defines);
		if(err)
			log0(L"Can't parse %s, err %d.", child.name.buffer(), err);
		else if(pChild)
			pControl->AddControl(pChild);
		else {}
	}
	return err_ok;
}

err_t CUILoaderXml::ParseEvent(const textw & name, const textw &value, CMetaHelper & metaEvent, CMetaHelper & metaFun)
{
	event_base * pEvent = metaEvent.find_evt(name);
	if(!pEvent)
		return err_no_context;

	function_base * pFun = metaFun.find_fun(value);
	if(!pFun)
		return err_no_context;

	pEvent->connect_base(*pFun);
	return err_ok;
}

template<typename ValT>
static bool xml_attr_parse_text(const xml_node & node, const char_16 * szAttribute, const ValT & fun)
{
	const xml_attr & attr = node[szAttribute];
	if(attr.is_valid())
	{
		fun(attr.value);
		return true;
	}
	return false;
}

template<typename ValT>
static bool xml_attr_parse_bool(const xml_node & node, const char_16 * szAttribute, const ValT & fun)
{
	const xml_attr & attr = node[szAttribute];
	if(attr.is_valid())
	{
		fun(attr.value.tobool());
		return true;
	}
	return false;
}

template<typename ValT>
static bool xml_attr_parse_intx(const xml_node & node, const char_16 * szAttribute, const ValT & fun)
{
	const xml_attr & attr = node[szAttribute];
	if(attr.is_valid())
	{
		fun(attr.value.toix());
		return true;
	}
	return false;
}

template<typename ValT>
static bool xml_attr_parse_f32(const xml_node & node, const char_16 * szAttribute, const ValT & fun)
{
	const xml_attr & attr = node[szAttribute];
	if(attr.is_valid())
	{
		fun(attr.value.tof32());
		return true;
	}
	return false;
}

template<typename ValT, typename LamdaT>
static bool xml_attr_parse_type(const xml_node & node, const char_16 * szAttribute, const LamdaT & lamda, const ValT & fun)
{
	const xml_attr & attr = node[szAttribute];
	if(attr.is_valid())
	{
		fun(lamda(attr.value));
		return true;
	}
	return false;
}

err_t CUILoaderXml::ParseControl(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	if(!pControl)
		return err_invalidarg;

	xml_attr_parse_bool(node, L"visible", [&pControl](bool value) { pControl->SetVisible(value); });
	xml_attr_parse_bool(node, L"enable", [&pControl](bool value) { pControl->SetEnable(value); });
	xml_attr_parse_bool(node, L"static", [&pControl](bool value) { pControl->SetStatic(value); });
	xml_attr_parse_bool(node, L"transparent", [&pControl](bool value) { pControl->SetTransparent(value); });
	xml_attr_parse_bool(node, L"clipchild", [&pControl](bool value) { pControl->SetClipChild(value); });

	xml_attr_parse_bool(node, L"selectable", [&pControl](bool value) { pControl->SetSelectAble(value); });
	xml_attr_parse_bool(node, L"focusable", [&pControl](bool value) { pControl->SetFocusAble(value); });

	xml_attr_parse_bool(node, L"acceptdrop", [&pControl](bool value) { pControl->SetAcceptDrop(value); });
	xml_attr_parse_bool(node, L"accepttab", [&pControl](bool value) { pControl->SetAcceptTab(value); });
	xml_attr_parse_bool(node, L"acceptenter", [&pControl](bool value) { pControl->SetAcceptEnter(value); });
	xml_attr_parse_bool(node, L"acceptscroll", [&pControl](bool value) { pControl->SetAcceptScroll(value); });

	xml_attr_parse_type(node, L"bordertype", to_bordertype, [&pControl](BorderTypeE value) { pControl->SetBorderType(value); });

	rectix rect = pControl->GetRect();
	xml_attr_parse_intx(node, L"x", [&rect](int_x value) { rect.x = value; });
	xml_attr_parse_intx(node, L"y", [&rect](int_x value) { rect.y = value; });
	xml_attr_parse_intx(node, L"width", [&rect](int_x value) { rect.w = value; });
	xml_attr_parse_intx(node, L"height", [&rect](int_x value) { rect.h = value; });
	xml_attr_parse_type(node, L"position", to_intx2, [&rect](const intx2 & value) { rect.position = value; });
	xml_attr_parse_type(node, L"size", to_intx2, [&rect](const intx2 & value) { rect.size = value; });
	xml_attr_parse_type(node, L"rect", to_rect, [&rect](const rectix & value) { rect = value; });
	pControl->SetRect(rect);

	xml_attr_parse_type(node, L"margin", to_edge, [&pControl](const edgeix & value) { pControl->SetMargin(value); });
	xml_attr_parse_type(node, L"padding", to_edge, [&pControl](const edgeix & value) { pControl->SetPadding(value); });
	xml_attr_parse_type(node, L"anchor", to_anchor, [&pControl](const anchor_t & value) { pControl->SetAnchor(value); });

	xml_attr_parse_type(node, L"widthmode", to_whmode, [&pControl](const WHModeE & value) { pControl->SetWidthMode(value); });
	xml_attr_parse_type(node, L"heightmode", to_whmode, [&pControl](const WHModeE & value) { pControl->SetHeightMode(value); });
	xml_attr_parse_f32(node, L"weight", [&pControl](float_32 value) { pControl->SetWeight(value); });

	xml_attr_parse_intx(node, L"fcolor", [&pControl](int_x value) { pControl->SetForeColor((uint_32)value); });
	xml_attr_parse_intx(node, L"bcolor", [&pControl](int_x value) { pControl->SetBackColor((uint_32)value); });

	xml_attr_parse_text(node, L"text", [&pControl](textw value) { pControl->SetText(value); });

	xml_attr_parse_bool(node, L"autohidescrollbarx", [&pControl](bool value) { pControl->SetAutoHideScrollBarX(value); });
	xml_attr_parse_bool(node, L"autohidescrollbary", [&pControl](bool value) { pControl->SetAutoHideScrollBarY(value); });

	xml_attr_parse_type(node, L"imemode", to_imemode, [&pControl](const ImeModeE & value) { pControl->SetImeMode(value); });
	xml_attr_parse_type(node, L"layoutmode", to_layoutmode, [&pControl](const LayoutModeE & value) { pControl->SetLayoutMode(value); });

	const xml_node & node_params = node(L"params");
	if(node_params.is_valid())
	{
		for(int_x cnt = 0; cnt < node_params.children.size(); ++cnt)
		{
			const xml_node & node_param = node_params.children[cnt];
			textw name = node_param[L"name"];
			textw value = node_param[L"value"];
			pControl->SetParam(name, value);
		}
	}
	return err_ok;
}

err_t CUILoaderXml::ParseForm(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	CForm * pForm = (CForm *)pControl;
	xml_attr_parse_type(node, L"formtype", [](const textw & str)->FormTypeE
	{
		if(str.equals(L"default", -1, false))
			return FormTypeDefault;
		if(str.equals(L"popup", -1, false))
			return FormTypePopup;
		else if(str.equals(L"layered", -1, false))
			return FormTypeLayered;
		else
			return FormTypeDefault;
	}, [&pForm](FormTypeE eType) { pForm->SetFormType(eType); });

	xml_attr_parse_type(node, L"minsize", to_intx2, [&pForm](const intx2 & value) { pForm->SetMinSize(value); });
	xml_attr_parse_type(node, L"maxsize", to_intx2, [&pForm](const intx2 & value) { pForm->SetMaxSize(value); });
	xml_attr_parse_intx(node, L"captionheight", [&pForm](int_x value) { pForm->SetCaptionHeight(value); });

	xml_attr_parse_bool(node, L"minbox", [&pForm](bool value) { pForm->SetMinBox(value); });
	xml_attr_parse_bool(node, L"maxbox", [&pForm](bool value) { pForm->SetMaxBox(value); });
	xml_attr_parse_bool(node, L"closebox", [&pForm](bool value) { pForm->SetCloseBox(value); });
	xml_attr_parse_type(node, L"resizeborder", to_edge, [&pForm](const edgeix & value) { pForm->SetResizeBorder(value); });

	return ParseControl(node, pControl, meta, defines);
}

err_t CUILoaderXml::ParseStatic(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	CStatic * pStatic = (CStatic *)pControl;
	xml_attr_parse_type(node, L"textalign", to_align, [&pStatic](const AlignE & value) { pStatic->SetTextAlign(value); });
	return ParseControl(node, pControl, meta, defines);
}

err_t CUILoaderXml::ParseButton(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	CButton * pButton = (CButton *)pControl;
	xml_attr_parse_type(node, L"dialogresult", [](const textw & str)
	{
		if(str.equals(L"ok", -1, false))
			return DialogResultOk;
		if(str.equals(L"cancel", -1, false))
			return DialogResultCancel;
		else if(str.equals(L"yes", -1, false))
			return DialogResultYes;
		else if(str.equals(L"no", -1, false))
			return DialogResultNo;
		else
			return DialogResultNone;
	}, [&pButton](DialogResultE value) { pButton->SetDialogResult(value); });
	return ParseControl(node, pControl, meta, defines);
}

err_t CUILoaderXml::ParseTextLine(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	CTextLine * pTextLine = (CTextLine *)pControl;
	xml_attr_parse_text(node, L"defaulttext", [&pTextLine](textw value) { pTextLine->SetDefaultText(value); });
	return ParseControl(node, pControl, meta, defines);
}

err_t CUILoaderXml::ParseTextBox(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	return ParseControl(node, pControl, meta, defines);
}

err_t CUILoaderXml::ParseListView(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	CListView * pListView = (CListView *)pControl;
	const xml_node & columns = node(L"columns");
	if(columns.is_valid())
	{
		struct col_i
		{
			int_x col;
			int_x interest;
		};
		vector<col_i> cols;

		int_x iCol = 0;
		for(int_x cnt = 0; cnt < columns.children.size(); ++cnt)
		{
			const xml_node & child = columns.children[cnt];
			if(child.name == L"column")
			{
				int_x iWidth = 0;
				int_x iId = 0;
				if(child.has_attribute(L"width"))
					iWidth = child[L"width"].value.toix(0);
				if(child.has_attribute(L"main"))
				{
					bool bMain = child[L"main"].value.tobool(false);
					if(bMain)
						pListView->SetMainCol(iCol);
				}
				if(child.has_attribute(L"interest"))
				{
					int_x iInterest = child[L"interest"].value.toix(-1);
					if(iInterest >= 0)
					{
						col_i ci = {iCol, iInterest};
						cols.add(ci);
					}
				}
				pListView->AddCol(child[L"text"].value, iWidth);
				++iCol;
			}
			else
			{
				IControl * pChild = nullptr;
				err_t err = Parse(child, pChild, meta, defines);
				if(err)
					log0(L"Can't parse %s, err %d.", child.name.buffer(), err);
				else if(pChild)
					pControl->AddControl(pChild);
				else {}
			}
		}
		if(cols.valid())
		{
			cols.sort(
				[&](const col_i & left, const col_i & right) { return left.interest > right.interest; }
			);
			vector<int_x> colIndices(cols.size(), cols.size());
			for(int_x cnt = 0; cnt < cols.size(); ++cnt)
				colIndices[cnt] = cols[cnt].col;
			pListView->SetInterestCols(colIndices.buffer(), cols.size());
		}
	}
	ParseControl(node, pControl, meta, defines);
	return err_end;
}

err_t CUILoaderXml::ParseTreeView(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	return ParseControl(node, pControl, meta, defines);
}

err_t CUILoaderXml::ParseScrollView(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	ParseControl(node, pControl, meta, defines);

	CFoldingView * pScrollView = (CFoldingView *)pControl;
	pScrollView->BegUpdate();
	for(int_x cnt = 0; cnt < node.children.size(); ++cnt)
	{
		const xml_node & child0 = node.children[cnt];
		if(child0.name.equals(L"defines", 7, false))
		{
			defines.load(child0);
		}
		else if(child0.name == L"item")
		{
			textw title = child0[L"title"];
			int_x minwidth = 0;
			if(child0.has_attribute(L"minwidth"))
				minwidth = child0[L"minwidth"].value.toix();

			if(child0.children.empty())
			{
				IFoldingViewItem * psvi = pScrollView->AddItem(title, minwidth, nullptr, 0);
			}
			else
			{
				const xml_node & child1 = child0.children[0];
				IControl * pChild = nullptr;
				err_t err = Parse(child1, pChild, meta, defines);
				if(err)
					log0(L"Can't parse %s, err %d.", child0.name.buffer(), err);
				else if(pChild)
				{
					FoldingViewItemT * psvi = new FoldingViewItemT();
					psvi->title = title;
					psvi->minwidth = minwidth;
					psvi->panel = pChild;
					pScrollView->AddItem(psvi);
				}
				else {}
			}
		}
		else
		{
			IControl * pChild = nullptr;
			err_t err = Parse(child0, pChild, meta, defines);
			if(err)
				log0(L"Can't parse %s, err %d.", child0.name.buffer(), err);
			else if(pChild)
				pControl->AddControl(pChild);
			else {}
		}
	}
	pScrollView->EndUpdate();

	return err_end;
}

err_t CUILoaderXml::ParseTabControl(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	ParseControl(node, pControl, meta, defines);

	CTabControl * pTabControl = (CTabControl *)pControl;
	for(int_x cnt = 0; cnt < node.children.size(); ++cnt)
	{
		const xml_node & child0 = node.children[cnt];
		if(child0.name.equals(L"defines", 7, false))
		{
			defines.load(child0);
		}
		else if(child0.name == L"tabpanel")
		{
			int_x iTab = pTabControl->AddTab(child0[L"text"].value);

			for(int_x cnt = 0; cnt < child0.children.size(); ++cnt)
			{
				IControl * pChild = nullptr;
				const xml_node & child1 = child0.children[cnt];
				Parse(child1, pChild, meta, defines);

				if(pChild)
				{
					CMetaHelper metaChild;
					pChild->GetMeta(&metaChild);
					for(int_x cnt = 0, size = child1.attributes.size(); cnt < size; ++cnt)
					{
						const xml_attr & attr = child1.attributes[cnt];
						ParseEvent(attr.name, attr.value, metaChild, meta);
					}
					pTabControl->AddTabControl(iTab, pChild);
				}
			}
		}
		else
		{
			IControl * pChild = nullptr;
			err_t err = Parse(child0, pChild, meta, defines);
			if(err)
				log0(L"Can't parse %s, err %d.", child0.name.buffer(), err);
			else if(pChild)
				pControl->AddControl(pChild);
			else {}
		}
	}

	xml_attr_parse_intx(node, L"selectedindex", [&pTabControl](int_x value) { pTabControl->SetSelectedIndex(value); });
	return err_ok;
}

err_t CUILoaderXml::ParseMenuBar(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	CMenuBar * pMenuBar = (CMenuBar *)pControl;
	for(int_x cnt = 0; cnt < node.children.size(); ++cnt)
	{
		const xml_node & child = node.children[cnt];
		if(child.name.equals(L"defines", 7, false))
		{
			defines.load(child);
		}
		else if(child.name == L"menu")
		{
			IMenu * pMenu = ParseMenu(child, defines);
			pMenuBar->SetShowMenu(pMenu);
			SafeRelease(pMenu);
		}
		else
		{

			IControl * pChild = nullptr;
			err_t err = Parse(child, pChild, meta, defines);
			if(err)
				log0(L"Can't parse %s, err %d.", child.name.buffer(), err);
			else if(pChild)
				pControl->AddControl(pChild);
			else {}
		}
	}
	ParseControl(node, pControl, meta, defines);
	return err_end;
}

err_t CUILoaderXml::ParseLayout(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	return ParseControl(node, pControl, meta, defines);
}

err_t CUILoaderXml::ParsePane(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	ParseControl(node, pControl, meta, defines);

	CPane * pPane = (CPane *)pControl;
	for(int_x cnt = 0; cnt < node.children.size(); ++cnt)
	{
		const xml_node & child = node.children[cnt];
		if(child.name.equals(L"defines", 7, false))
		{
			defines.load(child);
			continue;
		}

		if(child.name == L"panel")
		{
			IControl * pControl = new CPanel();
			pControl->SetDynamic(true);
			Parse(child, pControl, meta, defines);
			pPane->AddPanel(pControl);
		}
		else
		{
			IControl * pChild = nullptr;
			err_t err = Parse(child, pChild, meta, defines);
			if(err)
				log0(L"Can't parse %s, err %d.", child.name.buffer(), err);
			else if(pChild)
				pControl->AddControl(pChild);
			else {}
		}
	}

	xml_attr_parse_intx(node, L"selectedindex", [&pPane](int_x value) { pPane->SetSelectedIndex(value); });
	return err_end;
}

err_t CUILoaderXml::ParseDockFrame(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	ParseControl(node, pControl, meta, defines);

	CDockFrame * pDockFrame = (CDockFrame *)pControl;
	if(node.has_attribute(L"direction"))
		pDockFrame->DockNodeSetHorizontal(node[L"direction"].value.equals(L"horizontal", 10, false));

	for(int_x cnt = 0; cnt < node.children.size(); ++cnt)
	{
		const xml_node & child = node.children[cnt];
		if(child.name.equals(L"defines", 7, false))
		{
			defines.load(child);
		}
		else if(child.name == L"docknode")
		{
			IDockNode * pNode = nullptr;
			ParseDockNode(pNode, child, meta, defines);
			pDockFrame->DockNodeAdd(pNode, false);
			pNode->DockNodeSetContainer(pDockFrame);
		}
		else
		{

			IControl * pChild = nullptr;
			err_t err = Parse(child, pChild, meta, defines);
			if(err)
				log0(L"Can't parse %s, err %d.", child.name.buffer(), err);
			else if(pChild)
				pControl->AddControl(pChild);
			else {}
		}
	}
	pDockFrame->DockNodeUpdateFramework();
	return err_end;
}

err_t CUILoaderXml::ParseProgressBar(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines)
{
	CProgressBar * pProgressBar = (CProgressBar *)pControl;

	xml_attr_parse_type(node, L"marquee", to_intx2, [&pProgressBar](const intx2 & value) { pProgressBar->SetMarquee(value.cx, value.cy); });
	return ParseControl(node, pControl, meta, defines);
}


IMenu * CUILoaderXml::ParseMenu(const xml_node & node, CLoaderDefines & defines)
{
	struct SubMenuT
	{
		IMenu * pMenu;
		textw name;
	};

	vector<SubMenuT> submenus;
	for(int_x cnt = 0; cnt < node.children.size(); ++cnt)
	{
		const xml_node & child = node.children[cnt];
		if(child.name.equals(L"submenu", 7, false))
		{
			SubMenuT submenu;
			submenu.pMenu = ParseMenu(child, defines);
			submenu.name = child[L"name"].value;
			submenus.add(submenu);
		}
	}

	MenuT * pMenu = new MenuT();
	for(int_x cnt = 0; cnt < node.children.size(); ++cnt)
	{
		const xml_node & child = node.children[cnt];
		if(child.name.equals(L"item", 4, false))
		{
			IMenu * pSubMenu = nullptr;
			textw submenuname = child[L"submenu"].value;
			if(submenuname.is_valid())
			{
				for(int_x index = 0; index < submenus.size(); ++index)
				{
					if(submenus[index].name == submenuname)
					{
						pSubMenu = submenus[index].pMenu;
						break;
					}
				}
			}
			else {}

			int_x iId = 0;
			if(child.has_attribute(L"id"))
			{
				textw id = child[L"id"].value;
				textw define = defines.get(id);
				if(define.is_valid())
					iId = define.toix();
				else
					iId = id.toix();
			}

			pMenu->AddItem(iId, child[L"text"].value, pSubMenu);
		}
		else if(child.name.equals(L"split", 5, false))
			pMenu->AddSplit();
		else {}
	}
	for(int_x index = 0; index < submenus.size(); ++index)
		SafeRelease(submenus[index].pMenu);
	return pMenu;
}

void CUILoaderXml::ParseDockNode(IDockNode *& pDockNode, const xml_node & node, CMetaHelper & meta, CLoaderDefines & defines)
{
	if(!pDockNode)
		pDockNode = new CDockNode(nullptr);

	if(node.has_attribute(L"direction"))
		pDockNode->DockNodeSetHorizontal(node[L"direction"].value.equals(L"horizontal", 10, false));

	xml_attr_parse_intx(node, L"fixedsize", [&pDockNode](int_x value) { pDockNode->DockNodeSetFixedSize(value); });
	xml_attr_parse_f32(node, L"weight", [&pDockNode](float_32 value) { pDockNode->DockNodeSetWeight(value); });

	if(node.children.empty())
		return;

	for(int_x cnt = 0, size = node.children.size(); cnt < size; ++cnt)
	{
		const xml_node & child = node.children[cnt];
		if(child.type != xml_node_type_node)
			continue;

		if(child.name.equals(L"docknode", 8, false))
		{
			IDockNode * pNode = nullptr;
			ParseDockNode(pNode, child, meta, defines);
			pDockNode->DockNodeAdd(pNode, false);
		}
		else if(!pDockNode->DockNodeGetDockControl())
		{
			IControl * pControl = nullptr;
			Parse(child, pControl, meta, defines);
			pDockNode->DockNodeSetControl(pControl);
		}
		else
			log1(L"CUILoaderXml::ParseDockNode has too many controls.");
	}
	pDockNode->DockNodeUpdateFramework();
}

const CUILoaderXml::ui_load_info_t * CUILoaderXml::FindInfo(const xml_node & node) const
{
	const ui_load_info_t * pInfo = nullptr;
	for(int_x cnt = 0, size = m_infos.size(); cnt < size; ++cnt)
	{
		const ui_load_info_t & info = m_infos[cnt];
		if(node.name.equals(info.oid.name, -1, false))
			return &info;
	}
	return  nullptr;
}

VENUS_END
