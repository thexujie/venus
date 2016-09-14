#include "stdafx.h"
#include "CPath.h"

VENUS_BEG

CPath::CPath()
{

}

CPath::~CPath()
{

}
void CPath::MoveTo(float_32 fX, float_32 fY)
{
	PathPoints.add(PathPoint(PathCommandMoveTo, fX, fY));
}
void CPath::LineTo(float_32 fX, float_32 fY)
{
	PathPoints.add(PathPoint(PathCommandLineTo, fX, fY));
}
void CPath::BezierTo3(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2)
{
	PathPoints.add(PathPoint(PathCommandBezierTo3, fX1, fY1));
	PathPoints.add(PathPoint(PathCommandBezierTo3, fX2, fY2));
}
void CPath::BezierTo4(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3)
{
	PathPoints.add(PathPoint(PathCommandBezierTo4, fX1, fY1));
	PathPoints.add(PathPoint(PathCommandBezierTo4, fX2, fY2));
	PathPoints.add(PathPoint(PathCommandBezierTo4, fX3, fY3));
}
void CPath::CurveToX(float_32 * pfX, float_32 * pfY, int_x iCount)
{
	for(int_x cnt = 0; cnt < iCount; ++cnt)
		PathPoints.add(PathPoint(PathCommandBezierTo4, pfX[cnt], pfY[cnt]));
}

void CPath::BeginIter(int_x & iIndex) const
{
	iIndex = 0;
}

bool CPath::DoIter(int_x & iIndex, PathCommandE & command, float_32 & fX, float_32 & fY) const
{
	if(iIndex == PathPoints.size())
	{
#ifdef _DEBUG
		command = PathCommandInvalid;
		fX = fY = 0;
#endif
		return false;
	}
	else
	{
		const PathPoint & point = PathPoints[iIndex++];
		command = point.Command;
		fX = point.X;
		fY = point.Y;
		return true;
	}
}

void CPath::EndIter(int_x & iIndex) const
{
	iIndex = 0;
}

VENUS_END
