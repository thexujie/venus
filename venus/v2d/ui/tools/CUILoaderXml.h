#pragma once

VENUS_BEG

const oid_t OID_UILoaderXml = {L"UILoaderXml"};

class V2D_API CUILoaderXml : public ObjectT<IUILoader>
{
public:
	class CLoaderDefines
	{
	public:
		struct load_define_t
		{
			textw name;
			textw value;
		};

		CLoaderDefines() {}
		~CLoaderDefines() {}

		void load(const xml_node & node)
		{
			for(int_x cnt = 0, size = node.children.size(); cnt < size; ++cnt)
			{
				const xml_node & child = node.children[cnt];
				if(child.name.equal(L"define", 6, false))
				{
					load_define_t define = {child[L"name"].value, child[L"value"].value};
					if(define.name.is_valid() && !has(define.name))
						m_defines.add(define);
				}
			}
		}

		bool has(textw name)
		{
			for(int_x cnt = 0, size = m_defines.size(); cnt < size; ++cnt)
			{
				const load_define_t & define = m_defines[cnt];
				if(define.name == name)
					return true;
			}
			return false;
		}

		textw get(textw name)
		{
			for(int_x cnt = 0, size = m_defines.size(); cnt < size; ++cnt)
			{
				const load_define_t & define = m_defines[cnt];
				if(define.name == name)
					return define.value;
			}
			return textw();
		}

	protected:
		vector<load_define_t> m_defines;
	};

	struct ui_load_info_t
	{
		// 如果 parse 正确，就返回 err_ok， 如果ParseFunT 自己处理了 chidren，就返回 err_end。
		typedef err_t(CUILoaderXml::*ParseFunT)(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
		oid_t oid;
		ParseFunT ParseFun;
		const type_info & tinfo;
		IControl * (*CreateFun)();
	};

public:
	CUILoaderXml();
	~CUILoaderXml();

	void Load(const char_x * szFile, IControl * pControl);
	void Load(const byte_t * pData, int_x iSize, IControl * pControl);
	void Load(const xml_node & node, IControl * pControl);
	void Load(const xml_node & node, IControl * pControl, CLoaderDefines & defines);

	err_t ParseEvent(const textw & name, const textw & value, CMetaHelper & metaEvent, CMetaHelper & metaFun);
	err_t Parse(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
	err_t ParseChild(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
	err_t ParseControl(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
	err_t ParseForm(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
	err_t ParseStatic(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
	err_t ParseButton(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
	err_t ParseTextLine(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
	err_t ParseTextBox(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
	err_t ParseListView(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
	err_t ParseTreeView(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
	err_t ParseScrollView(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
	err_t ParseTabControl(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
	err_t ParseMenuBar(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
	err_t ParseLayout(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
	err_t ParsePane(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
	err_t ParseDockFrame(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);
	err_t ParseProgressBar(const xml_node & node, IControl *& pControl, CMetaHelper & meta, CLoaderDefines & defines);

	IMenu * ParseMenu(const xml_node & node, CLoaderDefines & defines);
	void ParseDockNode(IDockNode *& pDockNode, const xml_node & node, CMetaHelper & meta, CLoaderDefines & defines);
protected:
	const ui_load_info_t * FindInfo(const xml_node & node) const;
	vector<ui_load_info_t> m_infos;

public:
	template<typename ValT>
	static IControl * CreateFunTmpl()
	{
		return new ValT();
	}
};

VENUS_END
