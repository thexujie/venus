#include "stdafx.h"
#include "GdipInc.h"

#ifdef WIN32_DEVICE_GDIP

VENUS_BEG

using namespace Gdiplus;
using namespace DllExports;

#define GDIP_PROC_INIT(name) Gdip::##name = reinterpret_cast<decltype(name)>(GetProcAddress(hModule, #name));

namespace Gdip
{
	err_t InitialGdip(HMODULE hModule)
	{
		GdiplusStartup = (decltype(GdiplusStartup))(GetProcAddress(hModule, "GdiplusStartup"));
		Win32::DebugFormatLastError();

		//GDIP_PROC_INIT(GdiplusStartup);
		GDIP_PROC_INIT(GdiplusShutdown);

		GDIP_PROC_INIT(GdipCreateFromHDC);
		GDIP_PROC_INIT(GdipDeleteGraphics);
		GDIP_PROC_INIT(GdipGetDpiY);
		GDIP_PROC_INIT(GdipSetSmoothingMode);
		GDIP_PROC_INIT(GdipGetSmoothingMode);
		GDIP_PROC_INIT(GdipSetPixelOffsetMode);
		GDIP_PROC_INIT(GdipSetTextRenderingHint);
		GDIP_PROC_INIT(GdipGetTextRenderingHint);
		GDIP_PROC_INIT(GdipSetPageUnit);
		GDIP_PROC_INIT(GdipFlush);
		GDIP_PROC_INIT(GdipDrawLine);
		GDIP_PROC_INIT(GdipDrawArc);
		GDIP_PROC_INIT(GdipDrawBezier);
		GDIP_PROC_INIT(GdipDrawRectangle);
		GDIP_PROC_INIT(GdipDrawEllipse);
		GDIP_PROC_INIT(GdipGraphicsClear);
		GDIP_PROC_INIT(GdipFillRectangle);
		GDIP_PROC_INIT(GdipFillPolygon);
		GDIP_PROC_INIT(GdipFillPolygonI);
		GDIP_PROC_INIT(GdipFillEllipse);
		GDIP_PROC_INIT(GdipDrawImage);
		GDIP_PROC_INIT(GdipDrawImageRectRect);
		GDIP_PROC_INIT(GdipSetClipRectI);
		GDIP_PROC_INIT(GdipResetClip);
		GDIP_PROC_INIT(GdipDrawString);


		GDIP_PROC_INIT(GdipCreatePen1);
		GDIP_PROC_INIT(GdipDeletePen);

		GDIP_PROC_INIT(GdipDeleteBrush);
		GDIP_PROC_INIT(GdipCreateSolidFill);

		GDIP_PROC_INIT(GdipCreateFont);
		GDIP_PROC_INIT(GdipDeleteFont);
		GDIP_PROC_INIT(GdipGetFontStyle);

		GDIP_PROC_INIT(GdipCreateFontFamilyFromName);
		GDIP_PROC_INIT(GdipDeleteFontFamily);
		GDIP_PROC_INIT(GdipGetLineSpacing);
		GDIP_PROC_INIT(GdipGetEmHeight);
		GDIP_PROC_INIT(GdipGetCellAscent);
		GDIP_PROC_INIT(GdipGetCellDescent);

		GDIP_PROC_INIT(GdipStringFormatGetGenericTypographic);
		GDIP_PROC_INIT(GdipDeleteStringFormat);
		GDIP_PROC_INIT(GdipCloneStringFormat);
		GDIP_PROC_INIT(GdipSetStringFormatAlign);
		GDIP_PROC_INIT(GdipSetStringFormatLineAlign);
		GDIP_PROC_INIT(GdipSetStringFormatTrimming);
		GDIP_PROC_INIT(GdipSetStringFormatFlags);

		GDIP_PROC_INIT(GdipMeasureString);

		//----------------------------------------------------
		GDIP_PROC_INIT(GdipCreateBitmapFromScan0);
		GDIP_PROC_INIT(GdipCreateBitmapFromHBITMAP);
		GDIP_PROC_INIT(GdipDisposeImage);

		GDIP_PROC_INIT(GdipSaveImageToFile);
		GDIP_PROC_INIT(GdipGetImageEncodersSize);
		GDIP_PROC_INIT(GdipGetImageEncoders);

		return err_ok;
	}
}


//GpsRoundRectF::GpsRoundRectF(const RectF32 & rect, const SizeF32 & corner)
//{
//	float_32 fRight = rect.GetRight();
//	float_32 fBottom = rect.GetBottom();
//
//	AddArc(GpsRectF(rect.X, rect.Y, corner.Width * 2.0f, corner.Height * 2.0f), 180, 90); // 左上角
//	AddLine(rect.X + corner.Width - 1, rect.Y, fRight - corner.Width + 1, rect.Y); // 上
//	AddArc(GpsRectF(fRight - (corner.Width * 2.0f), rect.Y, corner.Width * 2.0f, corner.Height * 2.0f), 270, 90); // 右上角
//	AddLine(fRight, rect.Y + corner.Height - 1, fRight, fBottom - corner.Height + 1); // 右
//	AddArc(GpsRectF(fRight - corner.Width * 2.0f, fBottom - corner.Height * 2.0f,
//		corner.Width * 2.0f, corner.Height * 2.0f), 0, 90); // 右下角
//	AddLine(rect.X + corner.Width - 1, fBottom, fRight - corner.Width + 1, fBottom); // 下
//	AddArc(GpsRectF(rect.X, fBottom - corner.Height * 2.0f, corner.Width * 2.0f, corner.Height * 2.0f), 90, 90); // 左下
//	AddLine(rect.X, rect.Y + corner.Height - 1, rect.X, fBottom - corner.Height + 1); // 左
//}

void InitializeGdipGraphics(GpGraphics * pGraphics)
{
	if(pGraphics)
	{
		Gdip::GdipSetPageUnit(pGraphics, UnitPixel);
		Gdip::GdipSetTextRenderingHint(pGraphics, TextRenderingHintClearTypeGridFit);
		Gdip::GdipSetPixelOffsetMode(pGraphics, PixelOffsetModeNone);
		Gdip::GdipSetSmoothingMode(pGraphics, SmoothingModeNone);
	}
}

void SetGraphicsShapeRenderLevel(Gdiplus::GpGraphics * pGraphics, ShapeRenderLevelE eRenderLevel)
{
	switch(eRenderLevel)
	{
	case ShapeRenderLevelDefault:
		Gdip::GdipSetSmoothingMode(pGraphics, SmoothingModeHighSpeed);
		break;
	case ShapeRenderLevelAnti:
		Gdip::GdipSetSmoothingMode(pGraphics, SmoothingModeHighQuality);
		break;
	default:
		throw exp_illegal_argument();
	}
}

ShapeRenderLevelE GetGraphicsShapeRenderLevel(Gdiplus::GpGraphics * pGraphics)
{
	SmoothingMode eMode = SmoothingModeNone;
	Gdip::GdipGetSmoothingMode(pGraphics, &eMode);
	return eMode == SmoothingModeHighSpeed ? ShapeRenderLevelDefault : ShapeRenderLevelAnti;
}

void SetGraphicsTextRenderLevel(GpGraphics * pGraphics, FontRenderlevelE eRenderLevel)
{
	switch(eRenderLevel)
	{
	case FontRenderLevelSystem:
		Gdip::GdipSetTextRenderingHint(pGraphics, TextRenderingHintSystemDefault);
		break;
	case FontRenderLevelClearTypeGrid:
		Gdip::GdipSetTextRenderingHint(pGraphics, TextRenderingHintClearTypeGridFit);
		break;
	case FontRenderLevelGray:
		Gdip::GdipSetTextRenderingHint(pGraphics, TextRenderingHintSingleBitPerPixel);
		break;
	case FontRenderLevelGrayGrid:
		Gdip::GdipSetTextRenderingHint(pGraphics, TextRenderingHintSingleBitPerPixelGridFit);
		break;
	case FontRenderLevelAntiGray:
		Gdip::GdipSetTextRenderingHint(pGraphics, TextRenderingHintAntiAlias);
		break;
	case FontRenderLevelAntiGrayGrid:
		Gdip::GdipSetTextRenderingHint(pGraphics, TextRenderingHintAntiAliasGridFit);
		break;
	default:
		throw exp_not_supported();
	}
}

FontRenderlevelE GetGraphicsTextRenderLevel(Gdiplus::GpGraphics * pGraphics)
{
	TextRenderingHint eHint = TextRenderingHintSystemDefault;
	Gdip::GdipGetTextRenderingHint(pGraphics, &eHint);
	return (FontRenderlevelE)eHint;
}

BOOL GetEncoderClsid(ImageFormatE format, CLSID & clsid)
{
	const char_16 FMT_STRING[][11] = {L"image/bmp", L"image/jpeg", L"image/png", L"image/gif", L"image/tiff"};
	const char_16 * pFormat = NULL;
	switch(format)
	{
	case ImageFormatBmp:
		pFormat = FMT_STRING[0];
		break;
	case ImageFormatJpeg:
		pFormat = FMT_STRING[1];
		break;
	case ImageFormatPng:
		pFormat = FMT_STRING[2];
		break;
	case ImageFormatGif:
		pFormat = FMT_STRING[3];
		break;
	case ImageFormatTiff:
		pFormat = FMT_STRING[4];
		break;
	default:
		clsid = CLSID_NULL;
		return FALSE;
	}

	uint_32 num = 0, size = 0;
	Gdip::GdipGetImageEncodersSize(static_cast<UINT *>(&num), static_cast<UINT *>(&size));
	ImageCodecInfo * pImageCodecInfo = static_cast<ImageCodecInfo *>(malloc(size));
	Gdip::GdipGetImageEncoders(num, size, pImageCodecInfo);

	for(uint_32 cnt = 0; cnt < num; ++cnt)
	{
		const char_16 * pFormatX = pImageCodecInfo[cnt].MimeType;
		if(0 == wcscmp(pImageCodecInfo[cnt].MimeType, pFormat))
		{
			clsid = pImageCodecInfo[cnt].Clsid;

			free(pImageCodecInfo);
			return TRUE;
		}
	}
	free(pImageCodecInfo);
	return FALSE;
}

BOOL SaveImageToFile(HBITMAP hBitmap, LPCWSTR szFileName, ImageFormatE format)
{
	CLSID encoder;
	if(GetEncoderClsid(format, encoder))
	{
		GpBitmap * pBitmap = NULL;
		Gdip::GdipCreateBitmapFromHBITMAP(hBitmap, NULL, &pBitmap);
		bool bOk = Gdip::GdipSaveImageToFile(pBitmap, szFileName, &encoder, nullptr) == Ok;
		delete pBitmap;
	}
	return FALSE;
}

BOOL SaveImageToFile(GpImage * pImage, LPCWSTR szFileName, ImageFormatE format)
{
	CLSID encoder;
	if(GetEncoderClsid(format, encoder))
	{
		Status status = Gdip::GdipSaveImageToFile(pImage, szFileName, &encoder, nullptr);
		return status == Ok;
	}
	return FALSE;
}

//Image * ImageFromResource(UINT nID, LPCWSTR szType)
//{
//	Image * pImage = NULL;
//	HRSRC hRsrc = ::FindResourceW(NULL, (LPCWSTR)nID, szType);
//	if(hRsrc)
//	{
//		HGLOBAL hRes = ::LoadResource(NULL, hRsrc);
//		if(hRes)
//		{
//			VOID * pTheImage = ::LockResource(hRes);
//			if(pTheImage)
//			{
//				DWORD dwTheImage = ::SizeofResource(NULL, hRsrc);
//				ULONG uWrite(0);
//				IStream * pStream = NULL;
//				CreateStreamOnHGlobal(NULL, TRUE, &pStream);
//				pStream->Write(pTheImage, dwTheImage, &uWrite);
//				pImage = Image::FromStream(pStream);
//				pStream->Release();
//				UnlockResource(hRes);
//			}
//			FreeResource(hRes);
//		}
//	}
//	return pImage;
//}

VENUS_END

#endif //WIN32_DEVICE_GDIP
