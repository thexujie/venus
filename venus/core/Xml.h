#pragma once

#include "base/text.h"
#include "base/vector.h"

VENUS_BEG

enum err_xml_e
{
	err_xml_min = err_user,
	err_xml_bad_declaration,
	err_xml_bad_close_name,
	err_xml_bad_node,
	err_xml_bad_attribute,
	err_xml_bad_cdata,
	err_xml_bad_comment,
};

enum xml_node_type_e
{
	xml_node_type_invalid = 0,

	xml_node_type_declaration,
	xml_node_type_root,
	xml_node_type_node,
	xml_node_type_CDATA,
	xml_node_type_comment,
};

CORE_API void xml_escape(textw & text);

class CORE_API xml_attr
{
public:
	xml_attr();
	xml_attr(const char_16 * szName, const char_16 * szValue);
	~xml_attr();
	bool is_empty() const;
	bool is_valid() const;
	operator textw & () { return value; }
	operator const textw & () const { return value; }
public:
	textw name;
	textw value;
};

class CORE_API xml_node
{
public:
	xml_node(xml_node_type_e eType = xml_node_type_invalid) : type(eType) {}
	~xml_node() {}

	xml_node(xml_node && another)
	{
		type = another.type;
		name = another.name;
		text = another.text;
		attributes = move(another.attributes);
		children = move(another.children);
	}

	void clear()
	{
		type = xml_node_type_invalid;
		name.clear();
		text.clear();
		attributes.clear();
		children.clear();
	}

	bool is_empty() const;
	bool is_valid() const;

	const xml_attr & operator[](const char_16 * szAttribute) const;
	const xml_node & operator()(const char_16 * szChild) const;
	//none-const matching firstly.
	//xml_attribute & operator[](const char_16 * szAttribute);
	//xml_node & operator()(const char_16 * szChild);

	bool has_attribute(const char_16 * szAttribute) const;
	bool has_child(const char_16 * szChild) const;
public:
	xml_node_type_e type;
	textw name;
	textw text;
	vector<xml_attr> attributes;
	vector<xml_node> children;
};

class CORE_API xml_doc : public xml_node
{
public:
	xml_doc();
	~xml_doc();

	err_t load_text(const char_16 * szText, int_x iLength = -1);
	err_t load_file(const char_16 * szFile);
	void clear();
protected:
	err_t skip_whitespace(char_16 *& szXml, const char_16 * szEnd);
	err_t read_BOM(char_16 *& szXml, const char_16 * szEnd);

	err_t read_node(xml_node & node, char_16 *& szXml, const char_16 * szEnd);
	err_t read_attribute(xml_attr & attribute, char_16 *& szXml, const char_16 * szEnd);

	bool is_whitespace(char_16 ch);
public:
	FileBomE bom;
	textw text;
	xml_node declaration;
};

VENUS_END
