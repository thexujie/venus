#pragma once

#include "matrix_tmpl.h"

VENUS_BEG

// 3 * 3 矩阵
// 用于支持2D平面内的图形转换

template<typename ValueT>
class CORE_API matrix5_tmpl : public matrix_tmpl<ValueT, 5, 5>
{
	typedef vec2<ValueT> MatrixPoint;
	typedef matrix_tmpl<ValueT, 5, 5> BaseT;
public :
	matrix5_tmpl()
		: BaseT()
	{
	}
	matrix5_tmpl(const matrix5_tmpl & another)
		: BaseT(another)
	{

	}
	matrix5_tmpl(const ValueT & val)
		: BaseT(val)
	{

	}
	matrix5_tmpl(const ValueT (& _values)[3][3])
		: BaseT(_values)
	{

	}

	matrix5_tmpl(
		const ValueT & __11, const ValueT & __12, 
		const ValueT & __21, const ValueT & __22, 
		const ValueT & __31, const ValueT & __32)
	{
		Set(__11, __12, __21, __22, __31, __32);
	}
	matrix5_tmpl(
		const ValueT & __11, const ValueT & __12, const ValueT & __13, 
		const ValueT & __21, const ValueT & __22, const ValueT & __23, 
		const ValueT & __31, const ValueT & __32, const ValueT & __33)
	{
		Set(__11, __12, __13, __21, __22, __23, __31, __32, __33);
	}

	void Set(
		const ValueT & __11, const ValueT & __12, 
		const ValueT & __21, const ValueT & __22, 
		const ValueT & __31, const ValueT & __32)
	{
		Values[0][0] = __11; Values[0][1] = __12; Values[0][2] = 0; 
		Values[1][0] = __21; Values[1][1] = __22; Values[1][2] = 0;
		Values[2][0] = __31; Values[2][1] = __32; Values[2][2] = 1;
	}
	void Set(
		const ValueT & __11, const ValueT & __12, const ValueT & __13, 
		const ValueT & __21, const ValueT & __22, const ValueT & __23, 
		const ValueT & __31, const ValueT & __32, const ValueT & __33)
	{
		Values[0][0] = __11; Values[0][1] = __12; Values[0][2] = __13; 
		Values[1][0] = __21; Values[1][1] = __22; Values[1][2] = __23;
		Values[2][0] = __31; Values[2][1] = __32; Values[2][2] = __33;
	}

	// 倾斜变换，需要提供参照点(reference position)、倾斜方向(direction)、倾斜角度(angle)(单位为弧度)
	void Shear(const MatrixPoint & posRef, const MatrixPoint & dir, float_32 angle)
	{

	}
	// 平移变换
	void Translate(const ValueT & tx, const ValueT & ty)
	{
		Values[0][0] = 1;  Values[0][1] = 0;  Values[0][2] = 0;
		Values[1][0] = 0;  Values[1][1] = 1;  Values[1][2] = 0;
		Values[2][0] = tx; Values[2][1] = ty; Values[2][2] = 1;
	}
	void RotateX(float_32 angle)
	{
		float_32 fc = cosf(angle), fs = sinf(angle);
		Identity();
		Values[1][1] = fc; Values[1][2] = fs;
		Values[2][1] = -fs; Values[2][2] = fc;
	}
	void RotateY(float_32 angle)
	{
		float_32 fc = cosf(angle), fs = sinf(angle);
		Identity();
		Values[0][0] = fc; Values[0][2] = -fs;
		Values[2][0] = fs; Values[2][2] = fc;
	}
	void RotateZ(float_32 angle)
	{
		float_32 fc = cosf(angle), fs = sinf(angle);
		Identity();
		Values[0][0] = fc; Values[0][0] = fs;
		Values[1][0] = -fs; Values[1][1] = fc;
	}

	void MakeColorGray()
	{
		Identity();

		Values[0][0] = Values[0][1] = Values[0][2] = 0.3f;
		Values[1][0] = Values[1][1] = Values[1][2] = 0.59f;
		Values[2][0] = Values[2][1] = Values[2][2] = 0.11f;
		Values[3][3] = 1.0f;
		Values[4][4] = 1.0f;
	}
};

VENUS_END
