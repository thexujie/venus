#pragma once

#include "matrix_tmpl.h"

VENUS_BEG

// 3 * 3 矩阵
// 用于支持2D平面内的图形转换

template<typename ValueT>
class CORE_API matrix3_tmpl : public matrix_tmpl<ValueT, 3, 3>
{
	typedef vec2<ValueT> MatrixPoint;
	typedef matrix_tmpl<ValueT, 3, 3> BaseT;
public :
	matrix3_tmpl()
		: BaseT()
	{
	}
	matrix3_tmpl(const matrix3_tmpl & another)
		: BaseT(another)
	{

	}
	matrix3_tmpl(const ValueT & val)
		: BaseT(val)
	{

	}
	matrix3_tmpl(const ValueT (& _values)[3][3])
		: BaseT(_values)
	{

	}

	matrix3_tmpl(
		const ValueT & __11, const ValueT & __12, 
		const ValueT & __21, const ValueT & __22, 
		const ValueT & __31, const ValueT & __32)
	{
		Set(__11, __12, __21, __22, __31, __32);
	}
	matrix3_tmpl(
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
	void Rotate(float_32 angle)
	{
		float_32 fc = cos(angle), fs = sin(angle);

		Values[0][0] = fc;  Values[0][1] = fs; Values[0][2] = 0;
		Values[1][0] = -fs; Values[1][1] = fc; Values[1][2] = 0;
		Values[2][0] = 0;   Values[2][1] = 0;  Values[2][2] = 1;
	}
	void RotateAt(float_32 angle, const ValueT & rx, const ValueT & ry)
	{
		float_32 fc = cos(angle), fs = sin(angle);

		Values[0][0] = fc; Values[0][1] = fs; Values[0][2] = 0;
		Values[1][0] = -fs; Values[1][1] = fc; Values[1][2] = 0;
		Values[2][0] = -fc * rx + fs * ry + rx;
		Values[2][1] = -fs * rx - fc * ry + ry;
		Values[2][2] = 1;
	}
	void Scale(const float_32 & sx, const float_32 & sy)
	{
		Values[0][0] = sx; Values[0][1] = 0;  Values[0][2] = 0;
		Values[1][0] = 0;  Values[1][1] = sy; Values[1][2] = 0;
		Values[2][0] = 0;  Values[2][1] = 0;  Values[2][2] = 1;
	}
	bool InvertAble(const float_32 & mistake = 0.001f) const
	{
		for(uint_x cnt = 0; cnt < 3; ++cnt)
		{
			ValueT max = 0;
			//全选主元
			//寻找最大元素
			for(uint_x row = cnt, col = cnt; row != 3; ++row)
			{
				for(col = cnt; col != 3; ++col)
				{
					if(abs(Values[row * 3 + col]) > max)
						max = Values[row * 3 + col];
				}
			}

			if(abs(max) < mistake)
			{
				return false;
			}
		}
		return true;
	}

	bool InverseIn(const ValueT & mistake = 0.001f)
	{
		uint_x rows[3], cols[3];
		for(uint_x cnt = 0; cnt < 3; ++cnt)
		{
			ValueT max = 0;
			//全选主元
			//寻找最大元素
			for(uint_x row = cnt, col = cnt; row != 3; ++row)
			{
				for(col = cnt; col != 3; ++col)
				{
					if(abs(Values[row * 3 + col]) > max)
					{
						max = Values[row * 3 + col];
						rows[cnt]=row;
						cols[cnt]=col;
					}
				}
			}

			if(abs(max) < mistake)
			{
				return false;
			}

			//行交换
			if(rows[cnt] != cnt)
			{
				for(uint_x row = 0; row != 3; ++row)
				{
					swap(Values[cnt * 3 + row], Values[rows[cnt] * 3 + row]);
				}
			}

			//列交换
			if(cols[cnt]!= cnt)
			{
				for(uint_x col = 0; col != 3; ++col)
				{
					swap(Values[col * 3 + cnt], Values[col * 3 + cols[cnt]]);
				}
			}

			Values[cnt * 3 + cnt] = 1 / Values[cnt * 3 + cnt];

			for(uint_x col = 0; col != 3; ++col)
			{
				if(col != cnt)
				{
					Values[cnt * 3 + col] *= Values[cnt * 3 + cnt];
				}
			}

			for(uint_x row = 0; row != 3; ++row)
			{
				if(row != cnt)
				{
					for(uint_x col = 0; col!= 3; ++col)
					{
						if(col != cnt)
						{
							Values[row * 3 + col] -= Values[row * 3 + cnt] * Values[cnt * 3 + col];
						}
					}
				}
			}

			for(uint_x row = 0; row != 3; ++row)
			{
				if(row != cnt)
				{
					Values[row * 3 + cnt] *= -Values[cnt * 3 + cnt];
				}
			}
		}

		//恢复
		for (int_x cnt = 2; cnt >= 0; --cnt)
		{
			if(rows[cnt] != cnt)
			{
				for(uint_x row = 0; row != 3; ++row)
				{
					swap(Values[row * 3 + cnt], Values[row * 3 + rows[cnt]]);
				}
			}
			if(cols[cnt] != cnt)
			{
				for(uint_x col = 0; col != 3; ++col)
				{
					swap(Values[cnt * 3 + col], Values[cols[cnt] * 3 + col]);
				}
			}
		}
		return true;
	}

	bool InverseTo(matrix3_tmpl & another, const float_32 & mistake = 0.001f)
	{
		matrix3_tmpl result(*this);
		if(result.InverseIn(mistake))
		{
			another = result;
			return true;
		}
		else
			return false;
	}
	matrix3_tmpl Inverse(const float_32 & mistake = 0.001f) const
	{
		matrix3_tmpl result(*this);
		if(result.InverseIn(mistake))
			return result;
		else
		{
			result.Identity();
			return result;
		}
	}
};

VENUS_END
