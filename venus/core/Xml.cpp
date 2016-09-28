#include "stdafx.h"
#include "xml.h"

VENUS_BEG
const xml_attr xml_attr_EMPTY;
const xml_node xml_node_EMPTY;

void xml_escape(textw & text)
{
	text.replace(L"&lt;", 4, L"<", 1);
	text.replace(L"&gt;", 4, L">", 1);
	text.replace(L"&amp;", 5, L"&", 1);
	text.replace(L"&quot;", 6, L"\"", 1);
	text.replace(L"&apos;", 6, L"\'", 1);
	text.replace(L"&nbsp;", 6, L" ", 1);
	//text.replace(L"&copy;", 6, L"?", 1);
	//text.replace(L"&reg;", 5, L"?", 1);
}

xml_attr::xml_attr()
{

}

xml_attr::xml_attr(const char_16 * szName, const char_16 * szValue) :
	name(szName), value(szValue)
{

}

xml_attr::~xml_attr()
{

}

bool xml_attr::is_empty() const
{
	return this == nullptr || this == &xml_attr_EMPTY || name.is_empty();
}

bool xml_attr::is_valid() const
{
	return this != nullptr && this != &xml_attr_EMPTY && name.is_valid();
}

bool xml_node::is_empty() const
{
	return this == nullptr || this == &xml_node_EMPTY;
}

bool xml_node::is_valid() const
{
	return this != nullptr && this != &xml_node_EMPTY;
}

const xml_attr & xml_node::operator[](const char_16 * szAttribute) const
{
	if(!this)
		return xml_attr_EMPTY;

	for(int_x cnt = 0, len = attributes.size(); cnt < len; ++cnt)
	{
		const xml_attr & attribute = attributes[cnt];
		if(attribute.name == szAttribute)
			return attribute;
	}
	return xml_attr_EMPTY;
}

const xml_node & xml_node::operator()(const char_16 * szChild) const
{
	if(!this)
		return xml_node_EMPTY;

	for(int_x cnt = 0, len = children.size(); cnt < len; ++cnt)
	{
		const xml_node & child = children[cnt];
		if(child.name == szChild)
			return child;
	}
	return xml_node_EMPTY;
}

bool xml_node::has_attribute(const char_16 * szAttribute) const
{
	for(int_x cnt = 0, len = attributes.size(); cnt < len; ++cnt)
	{
		const xml_attr & attribute = attributes[cnt];
		if(attribute.name == szAttribute)
			return true;
	}
	return false;
}

bool xml_node::has_child(const char_16 * szChild) const
{
	for(int_x cnt = 0, len = children.size(); cnt < len; ++cnt)
	{
		const xml_node & child = children[cnt];
		if(child.name == szChild)
			return true;
	}
	return false;
}

//xml_attribute & xml_node::operator[](const char_16 * szAttribute)
//{
//	if(!this)
//		return *(xml_attribute *)nullptr;
//
//	for(int_x cnt = 0, len = attributes.size(); cnt < len; ++cnt)
//	{
//		xml_attribute & attribute = attributes[cnt];
//		if(attribute.name == szAttribute)
//			return attribute;
//	}
//	return *(xml_attribute *)nullptr;
//}
//
//xml_node & xml_node::operator()(const char_16 * szChild)
//{
//	if(!this)
//		return *(xml_node *)nullptr;
//
//	for(int_x cnt = 0, len = children.size(); cnt < len; ++cnt)
//	{
//		xml_node & child = children[cnt];
//		if(child.name == szChild)
//			return child;
//	}
//	return *(xml_node *)nullptr;
//}

xml_doc::xml_doc() :
	xml_node(xml_node_type_root), bom(FileBomInvalid)
{
}

xml_doc::~xml_doc()
{

}

err_t xml_doc::load_text(const char_16 * szText, int_x iLength)
{
	if(iLength < 0)
		iLength = textlen(szText);

	text.set(szText, iLength);

	char_16 * szXml = text;
	const char_16 * szEnd = szXml + text.length();
	err_t err = err_ok;
	read_BOM(szXml, szEnd);
	while(szXml < szEnd)
	{
		skip_whitespace(szXml, szEnd);
		if(szXml == szEnd)
			return err_ok;

		if(szXml[0] == L'<')
		{
			// declaration. <?xml?>
			if(szEnd - szXml >= 2 && szXml[1] == L'?')
			{
				szXml += 2;
				if(szEnd - szXml < 3 + 2)
					return err_xml_bad_declaration;

				if(textequalex(szXml, 3, L"xml", 3, false))
				{
					xml_attr attr;
					declaration.type = xml_node_type_declaration;
					szXml += 3;
					skip_whitespace(szXml, szEnd);
					while(szXml[0] != L'?' || szXml[1] != L'>')
					{
						read_attribute(attr, szXml, szEnd);
						declaration.attributes.add(attr);

						if(szXml == szEnd)
							return err_xml_bad_declaration;

						skip_whitespace(szXml, szEnd);

						if(szEnd - szXml < 2)
							return err_xml_bad_declaration;
					}
					szXml += 2;
				}

			}
			else
			{
				if(szEnd - szXml < 2)
					return err_xml_bad_node;

				err = read_node(*this, szXml, szEnd);
				if(err)
					return err;
			}
		}
		else
			return err_xml_bad_node;
	}

	return 0;
}

err_t xml_doc::load_file(const char_16 * szFile)
{
	CFileStream file(szFile, StreamModeRead);
	byte_buffer_t buffer = file.ReadFile();
	return load_text((const char_16 *)buffer.buffer, buffer.size / 2);
}

void xml_doc::clear()
{
	declaration.clear();
	xml_node::clear();
}

err_t xml_doc::skip_whitespace(char_16 *& szXml, const char_16 * szEnd)
{
	bool bWhile = true;
	while(bWhile && szXml < szEnd)
	{
		if(is_whitespace(*szXml))
			++szXml;
		else
			bWhile = false;
	}
	return err_ok;
}

err_t xml_doc::read_BOM(char_16 *& szXml, const char_16 * szEnd)
{
	if(szEnd - szXml < 3)
		return err_ok;

	if(szXml[0] == 0xFEFF)
	{
		szXml += 1;
		bom = FileBomUtf16SmallEdian;
	}
	else if(szXml[0] == 0xFFFE)
	{
		szXml += 1;
		bom = FileBomUtf16BigEdian;
	}

	return err_ok;
}

err_t xml_doc::read_node(xml_node & node, char_16 *& szXml, const char_16 * szEnd)
{
	verify(szXml[0] == L'<');

	// </>
	if(szEnd - szXml < 3)
		return err_xml_bad_node;

	err_t err = err_ok;

	if(szXml[1] == L'!')
	{
		// comment
		if(szXml[2] == L'-' && szXml[3] == L'-')
		{
			// 7 <!---->
			if(szEnd - szXml < 7)
				return err_xml_bad_comment;

			// 4 <!--
			szXml += 4;
			const char_16 * szComment = szXml;
			// 3 -->
			while(szXml + 3 < szEnd)
			{
				if(szXml[0] != L'-')
					szXml += 1;
				else if(szXml[1] != L'-')
					szXml += 2;
				else if(szXml[2] != L'>')
					szXml += 1;
				else
				{
					node.type = xml_node_type_comment;
					node.text.set(szComment, szXml - szComment);
					szXml += 3;
					return err_ok;
				}
			}
			return err_end;
		}

		// cdata
		if(szXml[2] == L'[' &&
			szXml[3] == L'C' && szXml[4] == L'D' && szXml[5] == L'A' && szXml[6] == L'T' && szXml[7] == L'A' &&
			szXml[8] == L'[')
		{
			// 12 <![CDATA[]]>
			if(szEnd - szXml < 12)
				return err_xml_bad_comment;

			// 9 <![CDATA[
			szXml += 9;
			const char_16 * szCDATA = szXml;

			// 3 -->
			while(szXml + 3 < szEnd)
			{
				if(szXml[0] != L']')
					szXml += 1;
				else if(szXml[1] != L']')
					szXml += 2;
				else if(szXml[2] != L'>')
					szXml += 3;
				else
				{
					node.type = xml_node_type_CDATA;
					node.text.set(szCDATA, szXml - szCDATA);
					szXml += 3;
					return err_ok;
				}
			}

			return err_end;
		}
	}

	// <
	++szXml;

	xml_attr attr;
	xml_node nodeChild;
	nodeChild.type = xml_node_type_node;

	skip_whitespace(szXml, szEnd);
	const char_16 * szName = szXml;
	const char_16 * szText = nullptr;
	bool bName = true;
	bool bAttribute = true;
	while(bName && szXml < szEnd)
	{
		if(szXml[0] == L'/')
		{
			if(szEnd - szXml < 2 || szXml[1] != L'>')
				return err_xml_bad_node;
			else
			{
				node.name.set(szName, szXml - szName);
				szXml += 2;
				return err_ok;
			}
		}
		else if(szXml[0] == L' ')
		{
			node.name.set(szName, szXml - szName);
			++szXml;
			break;
		}
		else if(szXml[0] == L'>')
		{
			node.name.set(szName, szXml - szName);
			++szXml;
			bAttribute = false;
			break;
		}
		else
			++szXml;
	}
	if(szXml == szEnd)
		return err_end;

	// attribute
	skip_whitespace(szXml, szEnd);
	while(bAttribute && szXml < szEnd)
	{
		switch(szXml[0])
		{
		case L' ':
			skip_whitespace(szXml, szEnd);
			break;
		case L'/':
			if(szEnd - szXml >= 2)
			{
				szXml += 2;
				return err_ok;
			}
			else
				return err_xml_bad_node;
			break;
		case L'>':
			++szXml;
			bAttribute = false;
			break;
		default:
			attr.name.clear();
			attr.value.clear();
			err = read_attribute(attr, szXml, szEnd);
			if(err == err_ok)
				node.attributes.add(attr);
			else
				return err;
			break;
		}
	}

	// children
	bool bText = false;
	skip_whitespace(szXml, szEnd);
	while(szXml < szEnd)
	{
		skip_whitespace(szXml, szEnd);
		if(szXml == szEnd)
			return err_ok;

		if(szXml[0] == L'<')
		{
			// close tag.
			if(szXml[1] == L'/')
			{
				szXml += 2;
				int_x iNameLength = node.name.length();
				if(szEnd - szXml < iNameLength + 1)
					return err_xml_bad_close_name;

				if(textequalex(szXml, iNameLength, node.name.buffer(), iNameLength, true))
				{
					if(szXml[iNameLength] != L'>')
						return err_xml_bad_node;

					szXml += iNameLength + 1;
					return err_ok;
				}
				else
					return err_xml_bad_close_name;
			}
			else
			{
				nodeChild.type = xml_node_type_node;
				nodeChild.name.clear();
				nodeChild.text.clear();
				nodeChild.attributes.clear();
				nodeChild.children.clear();
				err = read_node(nodeChild, szXml, szEnd);
				if(err)
					return err;

				xml_node & newNode = node.children.add();
				newNode.type = nodeChild.type;
				newNode.name = nodeChild.name;
				newNode.text = nodeChild.text;
				newNode.attributes.swap(nodeChild.attributes);
				newNode.children.swap(nodeChild.children);
			}
		}
		else
		{
			if(bText)
				return err_xml_bad_node;

			szText = szXml;
			while(szXml < szEnd)
			{
				if(szXml[0] == L'<')
				{
					node.text.set(szText, szXml - szText);
					xml_escape(node.text);
					bText = true;
					break;
				}
				else
					++szXml;
			}

			if(szXml == szEnd)
				return err_end;
		}
	}

	return err_end;
}

err_t xml_doc::read_attribute(xml_attr & attribute, char_16 *& szXml, const char_16 * szEnd)
{
	skip_whitespace(szXml, szEnd);
	const char_16 * szAttr = szXml;
	while(szXml < szEnd)
	{
		if(*szXml == L'=')
		{
			const char_16 * szTemp = szXml;
			while(szTemp > szAttr)
			{
				if(is_whitespace(*(szTemp - 1)))
					--szTemp;
				else
					break;
			}
			attribute.name.set(szAttr, szTemp - szAttr);
			++szXml;
			break;
		}
		else {}
		++szXml;
	}

	if(szEnd - szXml < 2 || (szXml[0] != L'\"' && szXml[0] != L'\''))
		return err_xml_bad_attribute;
	++szXml;
	const char_16 * szValue = szXml;
	while(szXml < szEnd)
	{
		if(szXml[0] == L'\"' || szXml[0] == L'\'')
		{
			attribute.value.set(szValue, szXml - szValue);
			xml_escape(attribute.value);
			++szXml;
			break;
		}
		++szXml;
	}
	return err_ok;
}

bool xml_doc::is_whitespace(char_16 ch)
{
	switch(ch)
	{
	case L' ':
	case L'\r':
	case L'\n':
	case L'\t':
		return true;
	default:
		return false;
	}
}

VENUS_END
