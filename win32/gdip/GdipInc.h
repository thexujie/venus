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
#pragma comment(lib, "GdiPlus.lib")

VENUS_BEG

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
