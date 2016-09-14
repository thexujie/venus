#pragma once

VENUS_BEG

class V2D_API PathPoint
{
public:
	PathPoint() : Command(PathCommandInvalid), X(0), Y(0) {}
	PathPoint(PathCommandE command, float_32 fX, float_32 fY) : Command(command), X(fX), Y(fY) {}
	~PathPoint() {}
	PathCommandE Command;
	float_32 X;
	float_32 Y;
};

class V2D_API CPath : public IPath
{
public:
	CPath();
	~CPath();

	void MoveTo(float_32 fX, float_32 fY);
	void LineTo(float_32 fX, float_32 fY);
	void BezierTo3(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2);
	void BezierTo4(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3);

	void CurveToX(float_32 * pfX, float_32 * pfY, int_x iCount);

	void BeginIter(int_x & iIndex) const;
	bool DoIter(int_x & iIndex, PathCommandE & command, float_32 & fX, float_32 & fY) const;
	void EndIter(int_x & iIndex) const;

	vector<PathPoint> PathPoints;
};

VENUS_END
