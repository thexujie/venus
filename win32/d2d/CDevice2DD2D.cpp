#include "stdafx.h"
#include "CDevice2DD2D.h"
#include "CTextLayoutD2D.h"
#include "CPaintD2D.h"

VENUS_BEG

CDevice2DD2D::CDevice2DD2D()
	:m_hModuleD2D(NULL), m_hModuleDWrite(NULL),
	m_pFactory(nullptr), m_pDWrite(nullptr)
{
	HRESULT hResult = S_OK;
	m_hModuleD2D = ::LoadLibraryW(L"d2d1.dll");
	if(m_hModuleD2D)
	{
		typedef HRESULT(__stdcall * D2D1CreateFactoryFunT)(
			D2D1_FACTORY_TYPE factoryType,
			const IID & riid,
			const D2D1_FACTORY_OPTIONS * pFactoryOptions,
			void ** factory);
		D2D1CreateFactoryFunT pfn = (D2D1CreateFactoryFunT)GetProcAddress(m_hModuleD2D, "D2D1CreateFactory");
		if(pfn)
			hResult = pfn(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(m_pFactory), nullptr, (void **)&m_pFactory);
		else
		{
			::FreeLibrary(m_hModuleD2D);
			m_hModuleD2D = NULL;
		}
	}

	//#pragma comment(lib, "d2d1.lib")
	//D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);
	if(m_pFactory)
	{
		D2D1_RENDER_TARGET_PROPERTIES d2drtp = D2D1::RenderTargetProperties();
		d2drtp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
		d2drtp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
		ID2D1DCRenderTarget * pRenderTarget = nullptr;

		m_hModuleDWrite = ::LoadLibraryW(L"dwrite.dll");
		if(m_hModuleDWrite)
		{
			typedef HRESULT(__stdcall * DWriteCreateFactoryFunT)(
				DWRITE_FACTORY_TYPE factoryType,
				const IID & iid,
				IUnknown **factory
				);
			DWriteCreateFactoryFunT pfn = (DWriteCreateFactoryFunT)::GetProcAddress(m_hModuleDWrite, "DWriteCreateFactory");
			if(pfn)
				hResult = pfn(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pDWrite), (IUnknown **)&m_pDWrite);
			else
			{
				::FreeLibrary(m_hModuleDWrite);
				m_hModuleDWrite = NULL;
			}
		}
		//#pragma comment(lib, "dwrite.lib")
		//		m_hResult = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pDWrite), (IUnknown **)&m_pDWrite);
	}
}

CDevice2DD2D::~CDevice2DD2D()
{
	for(int_x cnt = 0, len = m_fonts.size(); cnt < len; ++cnt)
		SafeRelease(m_fonts[cnt].pFormat);
	m_fonts.clear();
	SafeRelease(m_pDWrite);
	SafeRelease(m_pFactory);

	if(m_hModuleDWrite)
	{
		::FreeLibrary(m_hModuleDWrite);
		m_hModuleDWrite = NULL;
	}

	if(m_hModuleD2D)
	{
		::FreeLibrary(m_hModuleD2D);
		m_hModuleD2D = NULL;
	}
}

Device2DTypeE CDevice2DD2D::GetType() const
{
	return Device2DTypeDirect2D;
}

IDWriteTextFormat * CDevice2DD2D::GetTextFormat(const textformat_t & format)
{
	if(!m_pDWrite)
		return nullptr;

	int_x iHash = format.Hash();
	for(int_x cnt = 0, len = m_fonts.size(); cnt < len; ++cnt)
	{
		const D2DFontT & d2dFont = m_fonts[cnt];
		if(d2dFont.iHash == iHash)
			return d2dFont.pFormat;
	}

	IDWriteTextFormat * pFormat = nullptr;
	DWRITE_FONT_WEIGHT eWeight = (DWRITE_FONT_WEIGHT)format.font.weight;
	DWRITE_FONT_STYLE eItalic = (DWRITE_FONT_STYLE)format.font.italic;
	DWRITE_FONT_STRETCH eStretch = (DWRITE_FONT_STRETCH)format.font.strikeout;

	chbufferw<MAX_FONTNAME> chbName;
	if(format.font.name[0])
		chbName = format.font.name;
	else
		chbName = Win32::GetDefaultFontName();

	float_32 fDpiX = 0.0f, fDpiY = 0.0f;
	m_pFactory->GetDesktopDpi(&fDpiX, &fDpiY);
	float_32 fFontSize = (float_32)format.font.size * 72.0f / fDpiY;

	IDWriteFontCollection * pFontCollection = nullptr;
	//HRESULT hr = m_pDWrite->GetSystemFontCollection(&pFontCollection);
	//uint_32 uiIndex = 0;
	//BOOL bExist = FALSE;
	//pFontCollection->FindFamilyName(chbName, &uiIndex, &bExist);
	HRESULT hResult = m_pDWrite->CreateTextFormat(chbName, pFontCollection,
		(DWRITE_FONT_WEIGHT)format.font.weight,
		format.font.italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
		fFontSize,
		L"",
		&pFormat);
	SafeRelease(pFontCollection);

	if(format.align & AlignRight)
		pFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	else if(format.align & AlignCenterX)
		pFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	else
		pFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

	if(format.align & AlignBottom)
		pFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
	else if(format.align & AlignCenterY)
		pFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	else
		pFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	IDWriteInlineObject * pTrimmingSign = nullptr;
	DWRITE_TRIMMING trimming = {};
	switch(format.trimming)
	{
	case TextTrimmingChar:
		trimming.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
		break;
	case TextTrimmingCharEllipsis:
		trimming.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
		m_pDWrite->CreateEllipsisTrimmingSign(pFormat, &pTrimmingSign);
		break;
	case TextTrimmingWord:
		trimming.granularity = DWRITE_TRIMMING_GRANULARITY_WORD;
		break;
	case TextTrimmingWordEllipsis:
		trimming.granularity = DWRITE_TRIMMING_GRANULARITY_WORD;
		m_pDWrite->CreateEllipsisTrimmingSign(pFormat, &pTrimmingSign);
		break;
	case TextTrimmingPathEllipsis:
		trimming.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
		trimming.delimiter = L'/';
		m_pDWrite->CreateEllipsisTrimmingSign(pFormat, &pTrimmingSign);
		break;
	default:
		break;
	}
	pFormat->SetTrimming(&trimming, pTrimmingSign);
	pFormat->SetWordWrapping(GetDwTextWrapping(format.wrap));

	if(pFormat)
	{
#ifdef _DEBUG
		D2DFontT d2dFont = {iHash, format, pFormat};
#else
		D2DFontT d2dFont = {iHash, pFormat};
#endif
		m_fonts.add(d2dFont);
		return pFormat;
	}
	else
		return nullptr;
}

//fontmetrics_t CDevice2DD2D::GetFontMetric(const font_t & font)
//{
//	IDWriteTextFormat * pTextFormat = GetTextFormat(font);
//	fontmetrics_t metrics;
//	DWRITE_LINE_SPACING_METHOD eMethod = DWRITE_LINE_SPACING_METHOD_DEFAULT;
//	float_32 fLineSpace = 0.0f, fBaseLine = 0.0f;
//	pTextFormat->GetLineSpacing(&eMethod, &fLineSpace, &fBaseLine);
//
//	metrics.size = font.size;
//	metrics.weight = pTextFormat->GetFontWeight();
//	metrics.italic = pTextFormat->GetFontStyle() == DWRITE_FONT_STYLE_ITALIC;
//	metrics.linespace = round(fLineSpace);
//	metrics.baseline = round(fBaseLine);
//	metrics.underLine = false;
//	metrics.struckout = false;
//	//IDWriteFontFace * pFace = nullptr;
//	//DWRITE_FONT_METRICS fontMetrics;
//	//pFace->GetMetrics()
//	//fontMetrics.
//	return metrics;
//}

ITextLayout * CDevice2DD2D::CreateTextLayout()
{
	return new CTextLayoutD2D(this);
}

textsize_t CDevice2DD2D::GetTextSize(const char_16 * szText, int_x iLength, const font_t & font)
{
	if(!m_pDWrite)
		return textsize_t();

	if(iLength < 0)
		iLength = textlen(szText);

	if(!iLength)
		return textsize_t(0, font.size);

	IDWriteTextLayout * pLayout = nullptr;
	m_pDWrite->CreateTextLayout(szText, (uint_32)iLength, GetTextFormat(font), F32_MAX, F32_MAX, &pLayout);
	DWRITE_TEXT_METRICS dwTexMetricx = {};
	pLayout->GetMetrics(&dwTexMetricx);

	textsize_t tSize(ceil(dwTexMetricx.width), ceil(dwTexMetricx.height));
	SafeRelease(pLayout);
	return tSize;
}

bool d2d_rule_full(image_convert_rule_t * rule);
image_convert_rule_fun_t CDevice2DD2D::GetImageConvertRuleFun() const
{
	return d2d_rule_full;
}

IPaint * CDevice2DD2D::CreatePaint(I2DRTarget * pRenderTarget)
{
	CPaintD2D * pPaint = new CPaintD2D(pRenderTarget, this);
	return pPaint;
}

ID2D1Factory * CDevice2DD2D::GetFactory()
{
	return m_pFactory;
}

IDWriteFactory * CDevice2DD2D::GetDWrite()
{
	return m_pDWrite;
}

VENUS_END
