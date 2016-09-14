#pragma once

VENUS_BEG

template<typename FunT>
class CORE_API vec2
{
public:
	vec2() :x(0), y(0) {}
	vec2(FunT _x, FunT _y) :x(_x), y(_y) {}
	vec2(const vec2 & vec) : x(vec.x), y(vec.y) {}

	void set(FunT _x, FunT _y)
	{
		x = _x; y = _y;
	}

	void expand(FunT _cx, FunT _cy)
	{
		w += _cx * 2;
		h += _cy * 2;
	}

	void set_empty() { w = 0; h = 0; }
	bool is_empty() const { return w == 0 || h == 0; }

	vec2 & operator = (const vec2 & vec)
	{
		x = vec.x;
		y = vec.y;
		return *this;
	}

	//------------------------------------------------------- 四则运算
	vec2 operator + (const vec2 & vec) const
	{
		return vec2(x + vec.x, y + vec.y);
	}

	vec2 operator - (const vec2 & vec) const
	{
		return vec2(x - vec.x, y - vec.y);
	}

	vec2 operator * (const vec2 & vec) const
	{
		return vec2(x * vec.x, y * vec.y);
	}

	vec2 operator / (const vec2 & vec) const
	{
		return vec2(x / vec.x, y / vec.y);
	}

	vec2 & operator += (const vec2 & vec)
	{
		x += vec.x; y += vec.y; return *this;
	}

	vec2 & operator -= (const vec2 & vec)
	{
		x -= vec.x; y -= vec.y; return *this;
	}

	vec2 & operator *= (const vec2 & vec)
	{
		x *= vec.x; y *= vec.y; return *this;
	}

	vec2 & operator /= (const vec2 & vec)
	{
		x /= vec.x; y /= vec.y; return *this;
	}

	vec2 operator * (const FunT & rate) const
	{
		return vec2(x * rate, y * rate);
	}

	vec2 operator / (const FunT & rate)
	{
		return vec2(x / rate, y / rate);
	}

	vec2 & operator *= (const FunT & rate)
	{
		x *= rate; y *= rate; return *this;
	}

	vec2 & operator /= (const FunT & rate)
	{
		x /= rate; y /= rate; return *this;
	}

	vec2 operator - () const
	{
		return vec2(-x, -y);
	}

	vec2 operator + () const
	{
		return *this;
	}

	//--------------------------------------------------比较运算
	bool operator == (const vec2 & vec) const
	{
		return x == vec.x && y == vec.y;
	}

	bool operator != (const vec2 & vec) const
	{
		return x != vec.x || y != vec.y;
	}

	bool operator < (const vec2 & vec) const
	{
		return x < vec.x && y < vec.y;
	}

	bool operator >(const vec2 & vec) const
	{
		return x > vec.x && y > vec.y;
	}

	bool operator <= (const vec2 & vec) const
	{
		return x <= vec.x && y <= vec.y;
	}

	bool operator >=(const vec2 & vec) const
	{
		return x >= vec.x && y >= vec.y;
	}

public:
	union
	{
		struct
		{
			//! 横坐标。
			FunT x;
			//! 纵坐标。
			FunT y;
		};
		struct
		{
			//! 横坐标。
			FunT left;
			//! 纵坐标。
			FunT top;
		};
		struct
		{
			//! 宽。
			FunT cx;
			//! 高。
			FunT cy;
		};
		struct
		{
			//! 宽。
			FunT w;
			//! 高。
			FunT h;
		};
		struct
		{
			//! 列。
			FunT col;
			//! 行。
			FunT row;
		};
	};
};

VENUS_END
