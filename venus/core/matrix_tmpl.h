#pragma once


VENUS_BEG

// 3 * 3 矩阵
// 用于支持2D平面内的图形转换

template<typename ValueT, int_x ROW, int_x COL>
class matrix_tmpl
{
public :
	static const int_x COUNT = ROW * COL;
	matrix_tmpl()
	{
		Identity();
	}
	matrix_tmpl(const matrix_tmpl & another)
	{
		buffcpy<ValueT>((ValueT *)Values, COUNT, (ValueT *)another.Values, COUNT);
	}
	matrix_tmpl(const ValueT & val)
	{
		Set(val);
	}
	//MatrixTmpl(const ValueT (& _values)[3][3])
	//{
	//	Set(_values);
	//}

	void Set(const ValueT _values[ROW][COL])
	{
		for(int_x row = 0; row != ROW; ++row)
			for(int_x col = 0; col != COL; ++col)
				Values[row][col] = _values[row][col];
	}

	void Set(const ValueT & val)
	{
		arrayfill((ValueT *)Values, COUNT, val);
	}

	matrix_tmpl & Identity()
	{
		for(int_x row = 0; row != ROW; ++row)
		{
			for(int_x col = 0; col != COL; ++col)
			{
				if(row == col)
					Values[row][col] = 1;
				else
					Values[row][col] = 0;
			}
		}
		return *this;
	}

	//MatrixTmpl & operator *= (const MatrixTmpl & other)
	//{
	//	ValueT & (_values[ROW][COL]) = other.values;
	//	ValueT result[ROW][COL];
	//	ValueT (*pThisRow)[COL] = nullptr;
	//	for(uint_x row = 0, col = 0; row != 3; ++row)
	//	{
	//		pThisRow = values[row];
	//		for(col = 0; col != 3; ++col)
	//		{
	//			return[row * ROW + col] = 
	//				pThisRow[0] * _values[col] + pThisRow[1] * _values[ROW + col] + pThisRow[2] * _values[ROW + ROW + col];
	//		}
	//	}
	//	Set(result);
	//	return *this;
	//}

	//MatrixTmpl operator * (const MatrixTmpl other) const
	//{
	//	MatrixTmpl matrRet;
	//	ValueT & (_values[ROW][COL]) = other.values;
	//	ValueT & (result[ROW][COL]) = matrRet.values;
	//	ValueT (*pThisRow)[COL] = nullptr;
	//	for(uint_x row = 0, col = 0; row != 3; ++row)
	//	{
	//		pThisRow = values[row];
	//		for(col = 0; col != 3; ++col)
	//		{
	//			return[row * ROW + col] = 
	//				pThisRow[0] * _values[col] + pThisRow[1] * _values[ROW + col] + pThisRow[2] * _values[ROW + ROW + col];
	//		}
	//	}
	//	return matrRet;
	//}

	matrix_tmpl & operator = (const matrix_tmpl & another)
	{
		buffcpy<ValueT>((ValueT *)Values, COUNT, (ValueT *)another.Values, COUNT);
		return *this;
	}

	//ValueT & operator[](int_x index)
	//{
	//	return values[index];
	//}
	//const ValueT & operator[](int_x index) const
	//{
	//	return values[index];
	//}
	ValueT & operator()(int_x row, int_x col)
	{
		return Values[row][col];
	}
	const ValueT & operator()(int_x row, int_x col) const
	{
		return Values[row][col];
	}

	ValueT & operator[](int_x index)
	{
		ValueT * pValues = &(Values[0][0]);
		return pValues[index];
	}

	const ValueT & operator[](int_x index) const
	{
		const ValueT * pValues = &(Values[0][0]);
		return pValues[index];
	}

public :
	ValueT Values[ROW][COL];
};

VENUS_END
