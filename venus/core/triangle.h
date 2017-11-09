#pragma once

VENUS_BEG

template<typename PT>
class CORE_API triangle_tmpl
{
public:
	triangle_tmpl();

	template<typename ValT>
	triangle_tmpl(const vec2<ValT> & a, const vec2<ValT> & b, const vec2<ValT> & c)
		: A(a), B(b), C(c)
	{

	}

	template<typename ValT>
	triangle_tmpl(const ValT & ax, const ValT & ay, const ValT & bx, const ValT & by, const ValT & cx, const ValT & cy)
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
