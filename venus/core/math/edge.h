#pragma once

#include "vec2.h"

VENUS_BEG

template<typename FunT>
class CORE_API edge_tmpl
{
public:
	edge_tmpl() :left(0), top(0), right(0), bottom(0) {}
	edge_tmpl(FunT _size) :left(_size), top(_size), right(_size), bottom(_size) {}
	edge_tmpl(FunT _left, FunT _top, FunT _right, FunT _bottom) :left(_left), top(_top), right(_right), bottom(_bottom) {}
	edge_tmpl(const edge_tmpl & another) : left(another.left), top(another.top), right(another.right), bottom(another.bottom) {}
	void set(FunT _size)
	{
		left = _size; top = _size; right = _size; bottom = _size;
	}

	void set(FunT _left, FunT _top, FunT _right, FunT _bottom)
	{
		left = _left; top = _top; right = _right; bottom = _bottom;
	}

	edge_tmpl & operator = (const edge_tmpl & edge)
	{
		left = edge.left;
		top = edge.top;
		right = edge.right;
		bottom = edge.bottom;
		return *this;
	}

	bool operator != (const edge_tmpl & edge) const
	{
		return left != edge.left || top != edge.top || right != edge.right || bottom != edge.bottom;
	}

	FunT width() const { return left + right; }
	FunT height() const { return top + bottom; }
	vec2<FunT> size() const { return vec2<FunT>(left + right, top + bottom); }

	edge_tmpl operator + (const edge_tmpl & edge) const
	{
		return edge_tmpl(left + edge.left, top + edge.top,
						 right + edge.right, bottom + edge.bottom);
	}

	edge_tmpl & operator += (const edge_tmpl & edge)
	{
		left += edge.left; top += edge.top;
		right += edge.right; bottom += edge.bottom;
		return *this;
	}

	edge_tmpl operator - (const edge_tmpl & edge) const
	{
		return edge_tmpl(left - edge.left, top - edge.top,
						 right - edge.right, bottom - edge.bottom);
	}

	edge_tmpl & operator -= (const edge_tmpl & edge)
	{
		left -= edge.left; top -= edge.top;
		right -= edge.right; bottom -= edge.bottom;
		return *this;
	}

	bool is_empty() const { return left <= 0 && top <= 0 && right <= 0 && bottom <= 0; }
public:
	union
	{
		struct
		{
			vec2<FunT> lefttop;
			FunT __right;
			FunT __bottom;
		};
		struct
		{
			FunT left;
			FunT top;
			FunT right;
			FunT bottom;
		};
	};
};

VENUS_END
