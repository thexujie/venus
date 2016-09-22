#pragma once

#ifdef WIN32_DEVICE_GDIP

#ifdef _DEBUG  
#pragma push_macro("new")
#undef new
namespace Gdiplus  
{  
	namespace DllExports  
	{  
#include <GdiplusMem.h>  
	};  

#ifndef _GDIPLUSBASE_H  
#define _GDIPLUSBASE_H  

	class GdiplusBase  
	{  
	public:  
		void (operator delete)(void* in_pVoid)  
		{  
			DllExports::GdipFree(in_pVoid);  
		}  

		void* (operator new)(size_t in_size)  
		{  
			return DllExports::GdipAlloc(in_size);  
		}  

		void (operator delete[])(void* in_pVoid)  
		{  
			DllExports::GdipFree(in_pVoid);  
		}  

		void* (operator new[])(size_t in_size)  
		{  
			return DllExports::GdipAlloc(in_size);  
		}  

		void * (operator new)(size_t nSize, LPCSTR lpszFileName, int nLine)  
		{  
			return DllExports::GdipAlloc(nSize);  
		}  

		void operator delete(void* p, LPCSTR lpszFileName, int nLine)  
		{  
			DllExports::GdipFree(p);  
		}  

		void * (operator new)(size_t nSize, int nNormalBlockType, LPCSTR lpszFileName, int nLine)  
		{  
			return DllExports::GdipAlloc(nSize);  
		}  

		void operator delete(void* p, int nNormalBlockType, LPCSTR lpszFileName, int nLine)  
		{  
			DllExports::GdipFree(p);  
		}  

	};  
#endif // #ifndef _GDIPLUSBASE_H  
} 
#pragma pop_macro("new")
#endif // _DEBUG

//#define _COM_NO_STANDARD_GUIDS_
#include <comdef.h>
#include <GdiPlus.h>

VENUS_BEG



#pragma comment(lib, "GdiPlus.lib")

#define GDIP_PROC_DECL(name) static thread_local decltype(name) * ##name;

namespace Gdip
{
	using namespace Gdiplus;
	using namespace DllExports;

	GDIP_PROC_DECL(GdiplusStartup);
	GDIP_PROC_DECL(GdiplusShutdown);

	GDIP_PROC_DECL(GdipCreateFromHDC);
	GDIP_PROC_DECL(GdipDeleteGraphics);
	GDIP_PROC_DECL(GdipGetDpiY);
	GDIP_PROC_DECL(GdipSetSmoothingMode);
	GDIP_PROC_DECL(GdipGetSmoothingMode);
	GDIP_PROC_DECL(GdipSetPixelOffsetMode);
	GDIP_PROC_DECL(GdipSetTextRenderingHint);
	GDIP_PROC_DECL(GdipGetTextRenderingHint);
	GDIP_PROC_DECL(GdipSetPageUnit);
	GDIP_PROC_DECL(GdipFlush);
	GDIP_PROC_DECL(GdipDrawLine);
	GDIP_PROC_DECL(GdipDrawArc);
	GDIP_PROC_DECL(GdipDrawBezier);
	GDIP_PROC_DECL(GdipDrawRectangle);
	GDIP_PROC_DECL(GdipDrawEllipse);
	GDIP_PROC_DECL(GdipGraphicsClear);
	GDIP_PROC_DECL(GdipFillRectangle);
	GDIP_PROC_DECL(GdipFillPolygon);
	GDIP_PROC_DECL(GdipFillPolygonI);
	GDIP_PROC_DECL(GdipFillEllipse);
	GDIP_PROC_DECL(GdipDrawImage);
	GDIP_PROC_DECL(GdipDrawImageRectRect);
	GDIP_PROC_DECL(GdipSetClipRectI);
	GDIP_PROC_DECL(GdipResetClip);
	GDIP_PROC_DECL(GdipDrawString);


	GDIP_PROC_DECL(GdipCreatePen1);
	GDIP_PROC_DECL(GdipDeletePen);

	GDIP_PROC_DECL(GdipDeleteBrush);
	GDIP_PROC_DECL(GdipCreateSolidFill);

	GDIP_PROC_DECL(GdipCreateFont);
	GDIP_PROC_DECL(GdipDeleteFont);
	GDIP_PROC_DECL(GdipGetFontStyle);

	GDIP_PROC_DECL(GdipCreateFontFamilyFromName);
	GDIP_PROC_DECL(GdipDeleteFontFamily);
	GDIP_PROC_DECL(GdipGetLineSpacing);
	GDIP_PROC_DECL(GdipGetEmHeight);
	GDIP_PROC_DECL(GdipGetCellAscent);
	GDIP_PROC_DECL(GdipGetCellDescent);

	GDIP_PROC_DECL(GdipStringFormatGetGenericTypographic);
	GDIP_PROC_DECL(GdipDeleteStringFormat);
	GDIP_PROC_DECL(GdipCloneStringFormat);
	GDIP_PROC_DECL(GdipSetStringFormatAlign);
	GDIP_PROC_DECL(GdipSetStringFormatLineAlign);
	GDIP_PROC_DECL(GdipSetStringFormatTrimming);
	GDIP_PROC_DECL(GdipSetStringFormatFlags);

	GDIP_PROC_DECL(GdipMeasureString);

	//----------------------------------------------------
	GDIP_PROC_DECL(GdipCreateBitmapFromScan0);
	GDIP_PROC_DECL(GdipCreateBitmapFromHBITMAP);
	GDIP_PROC_DECL(GdipDisposeImage);

	GDIP_PROC_DECL(GdipSaveImageToFile);
	GDIP_PROC_DECL(GdipGetImageEncodersSize);
	GDIP_PROC_DECL(GdipGetImageEncoders);

	CORE_API err_t InitialGdip(HMODULE hModule);
}

WIN32_API void InitializeGdipGraphics(Gdiplus::GpGraphics * pGraphics);
WIN32_API void SetGraphicsShapeRenderLevel(Gdiplus::GpGraphics * pGraphics, ShapeRenderLevelE eRenderLevel);
WIN32_API ShapeRenderLevelE GetGraphicsShapeRenderLevel(Gdiplus::GpGraphics * pGraphics);
WIN32_API void SetGraphicsTextRenderLevel(Gdiplus::GpGraphics * pGraphics, FontRenderlevelE eRenderLevel);
WIN32_API FontRenderlevelE GetGraphicsTextRenderLevel(Gdiplus::GpGraphics * pGraphics);

WIN32_API BOOL SaveImageToFile(HBITMAP hBitmap, LPCWSTR szFileName, ImageFormatE format);
WIN32_API BOOL SaveImageToFile(Gdiplus::GpImage * pImage, LPCWSTR szFileName, ImageFormatE format);
//WIN32_API GpsImage * ImageFromResource(UINT nID, LPCWSTR szType);


class WIN32_API GpsPath
{
public:
	GpsPath();
	GpsPath(const IShape * pShape);
	void GenerateShape(const IShape * pShape, float_32 fOrignX, float_32 fOrignY);
	void Line(const pointf32 * pPoint1, const pointf32 * pPoint2);
	void Line(const pointix * pPoint1, const pointix * pPoint2);
	void Clear();

	Gdiplus::GpPath * pPath;
};


VENUS_END

#endif //WIN32_DEVICE_GDIP
