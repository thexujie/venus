#pragma once

VENUS_BEG

template<typename PT>
class CORE_API TriangleTmpl
{
public:
	TriangleTmpl();

	template<typename FunT>
	TriangleTmpl(const vec2<FunT> & a, const vec2<FunT> & b, const vec2<FunT> & c)
		: A(a), B(b), C(c)
	{

	}

	template<typename FunT>
	TriangleTmpl(const FunT & ax, const FunT & ay, const FunT & bx, const FunT & by, const FunT & cx, const FunT & cy)
		: A(ax, ay), B(bx, by), C(cx, cy)
	{

	}

public:
	union
	{
		struct
		{
			vec2<PT> A;
			vec2<PT> B;
			vec2<PT> C;
		};
		struct
		{
			PT Ax;
			PT Ay;
			PT Bx;
			PT By;
			PT Cx;
			PT Cy;
		};
	};
};

VENUS_END
