#include "stdafx.h"
#include "GdipInc.h"

#ifdef WIN32_DEVICE_GDIP

VENUS_BEG


using namespace Gdiplus;
using namespace DllExports;

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
		GdipSetPageUnit(pGraphics, UnitPixel);
		GdipSetTextRenderingHint(pGraphics, TextRenderingHintClearTypeGridFit);
		GdipSetPixelOffsetMode(pGraphics, PixelOffsetModeNone);
		GdipSetSmoothingMode(pGraphics, SmoothingModeNone);
	}
}

void SetGraphicsShapeRenderLevel(Gdiplus::GpGraphics * pGraphics, ShapeRenderLevelE eRenderLevel)
{
	switch(eRenderLevel)
	{
	case ShapeRenderLevelDefault:
		GdipSetSmoothingMode(pGraphics, SmoothingModeHighSpeed);
		break;
	case ShapeRenderLevelAnti:
		GdipSetSmoothingMode(pGraphics, SmoothingModeHighQuality);
		break;
	default:
		throw exp_illegal_argument();
	}
}

ShapeRenderLevelE GetGraphicsShapeRenderLevel(Gdiplus::GpGraphics * pGraphics)
{
	SmoothingMode eMode = SmoothingModeNone;
	GdipGetSmoothingMode(pGraphics, &eMode);
	return eMode == SmoothingModeHighSpeed ? ShapeRenderLevelDefault : ShapeRenderLevelAnti;
}

void SetGraphicsTextRenderLevel(GpGraphics * pGraphics, FontRenderlevelE eRenderLevel)
{
	switch(eRenderLevel)
	{
	case FontRenderLevelSystem:
		GdipSetTextRenderingHint(pGraphics, TextRenderingHintSystemDefault);
		break;
	case FontRenderLevelClearTypeGrid:
		GdipSetTextRenderingHint(pGraphics, TextRenderingHintClearTypeGridFit);
		break;
	case FontRenderLevelGray:
		GdipSetTextRenderingHint(pGraphics, TextRenderingHintSingleBitPerPixel);
		break;
	case FontRenderLevelGrayGrid:
		GdipSetTextRenderingHint(pGraphics, TextRenderingHintSingleBitPerPixelGridFit);
		break;
	case FontRenderLevelAntiGray:
		GdipSetTextRenderingHint(pGraphics, TextRenderingHintAntiAlias);
		break;
	case FontRenderLevelAntiGrayGrid:
		GdipSetTextRenderingHint(pGraphics, TextRenderingHintAntiAliasGridFit);
		break;
	default:
		throw exp_not_supported();
	}
}

FontRenderlevelE GetGraphicsTextRenderLevel(Gdiplus::GpGraphics * pGraphics)
{
	TextRenderingHint eHint = TextRenderingHintSystemDefault;
	GdipGetTextRenderingHint(pGraphics, &eHint);
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
	GetImageEncodersSize((UINT *)&num, (UINT *)&size);
	ImageCodecInfo * pImageCodecInfo = (ImageCodecInfo *)(malloc(size));
	GetImageEncoders(num, size, pImageCodecInfo);

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
		GdipCreateBitmapFromHBITMAP(hBitmap, NULL, &pBitmap);
		bool bOk = GdipSaveImageToFile(pBitmap, szFileName, &encoder, nullptr) == Ok;
		delete pBitmap;
	}
	return FALSE;
}

BOOL SaveImageToFile(GpImage * pImage, LPCWSTR szFileName, ImageFormatE format)
{
	CLSID encoder;
	if(GetEncoderClsid(format, encoder))
	{
		Status status = GdipSaveImageToFile(pImage, szFileName, &encoder, nullptr);
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
