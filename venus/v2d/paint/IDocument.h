#pragma once

VENUS_BEG

typedef sizeix textsize_t;

struct tlsize_t
{
	int_x w;
	int_x h;
};


struct tlrect_t
{
	int_x x;
	int_x y;
	int_x w;
	int_x h;
};


enum paragraph_tag_e
{
	paragraph_tag_none = 0x00,
	//! L"\r"
	paragraph_tag_cr = 0x11,
	//! L"\n"
	paragraph_tag_lf = 0x21,
	//! L"\r\n"
	paragraph_tag_crlf = 0x32,
	//! L"\u2028"
	paragraph_tag_ls = 0x41,
	//! L"\u2029"
	paragraph_tag_ps = 0x51,

	TextParagraphTagCharCountMask = 0xF,
};

struct doc_source_t
{
	int_x length;
	const char_16 * text;
};

// text range
struct trange_t
{
    int_x index = -1;
    int_x length = 0;
};

// glyph range
struct grange_t
{
    int_x index = -1;
    int_x length = 0;
};

// cluster range
struct crange_t
{
	int_x index = -1;
	int_x length = 0;
};

// line range
struct lrange_t
{
    int_x index = -1;
    int_x length = 0;
};

struct doc_paragraph_t
{
	// �ı���Χ
	trange_t trange;
	// ������
	paragraph_tag_e tag;
};

/**
* ��ʾһ����ʾ�ڲ����еĶ�����Ԫ��һ�����߸���� char_16 һ���ʾһ�� cluster��
*/
struct tl_cluster_t
{
	// cluster ����
	int_x cluster;
	// �к�
	int_x line;
	// ����
	int_x para;
	// �ı���Χ
	trange_t trange;
	// cluster �ڲ����е�λ��
	tlrect_t tlrect;

	bool whitespace : 1;
	//! �Ƿ��ֹ�ڸ��ַ�������
	bool softbreak : 1;
	//! �Ƿ��Ƕ�����
	bool paragraphtag : 1;
	//! �Ƿ������ַ�
	bool softhyphen : 1;
	//! �Ƿ��Ǵ��ҵ�����Ķ�˳��
	bool right2left : 1;
	//! �Ƿ����б��
	bool linetag : 1;
};

struct tl_line_t
{
	int_x line;
	int_x para;
	trange_t trange;
	crange_t crange;
	tlrect_t tlrect;
};

struct tl_para_t
{
	int_x para;
	trange_t trange;
	crange_t crange;
	lrange_t lrange;
	tlrect_t tlrect;
};

// ��ʾ���η�Χ
struct tl_range_t
{
	int_x line;
	int_x para;
	trange_t trange;
	crange_t crange;
	tlrect_t tlrect;
};

struct tl_metrics_t
{
	int_x length;

	int_x tl_x;
	int_x tl_y;
	int_x tl_width;
	int_x tl_height;

	int_x cluster_length;
	int_x line_length;
};

class V2D_API IDocSource
{
public:
	virtual ~IDocSource() {}
	virtual doc_source_t GetDocSource() const = 0;
	virtual textformat_t GetDefFormat() const = 0;
};

class V2D_API IDocLayout : public IObject
{
public:
	virtual err_t Initialize(IDocSource * pSrouce) = 0;
	virtual IDocSource * GetSource() const = 0;
};

class V2D_API ITextLayout : public IDocLayout
{
public:
	virtual err_t Layout(trange_t range, int_x iWidth, paragraph_tag_e tag = paragraph_tag_none) = 0;
	virtual err_t Branch(int_x iWidth) = 0;

	virtual tl_metrics_t GetMetrics() const = 0;

	virtual tl_cluster_t FindCluster(int_x iIndex) const = 0;
	virtual tl_cluster_t GetCluster(int_x iCluster) const = 0;
	virtual tl_line_t GetLine(int_x iLine) const = 0;

	virtual tl_range_t HitTest(pointix point) const = 0;
	virtual int_x HitTestRange(int_x iIndex, int_x iLength, tl_range_t * rects, int_x iCount) const = 0;
};

class V2D_API ITextDocument : public IDocLayout
{
public:
	virtual ~ITextDocument() {}

	virtual err_t Layout(int_x iWidth) = 0;
	virtual err_t Update(int_x iWidth) = 0;

	virtual err_t SetText(const char_16 * szText, int_x iLength) = 0;
	virtual err_t InsertText(int_x iIndex, const char_16 * szText, int_x iLength, int_x iWidth, const textformat_t & format) = 0;
	virtual err_t DeleteText(int_x iIndex, int_x iLength, int_x iWidth, const textformat_t & format) = 0;
	virtual int_x GetTextLength() const = 0;
	virtual const char_16 * GetText() const = 0;

	virtual err_t Layout(int_x iWidth, TextWrapE eTextWrap) = 0;
	virtual err_t UpdateDocument(int_x iWidth, const textformat_t & format) = 0;
	virtual err_t UpdateDocument(int_x iWidth, const textformat_t & format, int_x iParaIndex, int_x iParaLength) = 0;

	virtual int_x GetParagraphCount() const = 0;
	virtual const ITextLayout * GetParagraph(int_x iParagraph) const = 0;
	virtual int_x FindParagraph(int_x iIndex) const = 0;

	virtual tl_cluster_t GetClusterFromIndex(int_x iIndex) const = 0;
	virtual tl_cluster_t GetCluster(int_x iCluster) const = 0;
	virtual tl_cluster_t HitTest(int_x iX, int_x iY) const = 0;
	virtual tl_cluster_t HitTest(int_x iIndex) const = 0;
	virtual int_x HitTestRange(int_x iIndex, int_x iLength,
		tl_cluster_t * pPoints, int_x iCount,
		int_x iY = 0, int_x iHeight = 0) const = 0;

	virtual tl_metrics_t GetMetrics() const = 0;

	virtual void SetFontFamily(int_x iIndex, int_x iLength, const char_16 * szFamily) = 0;
	virtual void SetFontSize(int_x iIndex, int_x iLength, int_x iSize) = 0;
	virtual void SetFontItalic(int_x iIndex, int_x iLength, int_x iItalic) = 0;
	virtual void SetObject(int_x iIndex, int_x iLength, void * pObject) = 0;
};

V2D_API int_x GetParagraphTagLength(paragraph_tag_e tag);
V2D_API vector<doc_paragraph_t> TextParseParagraphs(const char_16 * szText, int_x iLength);


VENUS_END
