#pragma once

VENUS_BEG

template<typename FunT>
class CORE_API vec4
{
public:
	vec4() :x(0), y(0), z(0), w(0) {}
	vec4(FunT _x, FunT _y, FunT _z, FunT _w) :x(_x), y(_y), z(_z), w(_w) {}
	vec4(const vec4 & vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}

	void set(FunT _x, FunT _y, FunT _z, FunT _w)
	{
		x = _x; y = _y; z = _z; w = _w;
	}

	vec4 & operator = (const vec4 & vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
		return *this;
	}

	//------------------------------------------------------- 四则运算
	vec4 operator + (const vec4 & vec) const
	{
		return vec4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
	}

	vec4 operator - (const vec4 & vec) const
	{
		return vec4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
	}

	vec4 operator * (const vec4 & vec) const
	{
		return vec4(x * vec.x, y * vec.y, z * vec.z, w * vec.w);
	}

	vec4 operator / (const vec4 & vec) const
	{
		return vec4(x / vec.x, y / vec.y, z / vec.z, w / vec.w);
	}

	vec4 & operator += (const vec4 & vec)
	{
		x += vec.x; y += vec.y; z += vec.z; w += vec.w; return *this;
	}

	vec4 & operator -= (const vec4 & vec)
	{
		x -= vec.x; y -= vec.y; z -= vec.z; w -= vec.w; return *this;
	}

	vec4 & operator *= (const vec4 & vec)
	{
		x *= vec.x; y *= vec.y; z *= vec.z; w *= vec.w; return *this;
	}

	vec4 & operator /= (const vec4 & vec)
	{
		x /= vec.x; y /= vec.y; z /= vec.z; w /= vec.w; return *this;
	}

	vec4 operator * (const FunT & rate) const
	{
		return vec4(x * rate, y * rate, z * rate, w * rate);
	}

	vec4 operator / (const FunT & rate)
	{
		return vec4(x / rate, y / rate, z / rate, w / rate);
	}

	vec4 & operator *= (const FunT & rate)
	{
		x *= rate; y *= rate; z *= rate; w *= rate; return *this;
	}

	vec4 & operator /= (const FunT & rate)
	{
		x /= rate; y /= rate; z /= rate; w /= rate; return *this;
	}

	vec4 operator - () const
	{
		return vec4(-x, -y, -z, -w);
	}

	vec4 operator + () const
	{
		return *this;
	}

	//--------------------------------------------------比较运算
	bool operator == (const vec4 & vec) const
	{
		return x == vec.x && y == vec.y && z == vec.z && w == vec.w;
	}

	bool operator != (const vec4 & vec) const
	{
		return x != vec.x || y != vec.y || z != vec.z || w != vec.w;
	}

	bool operator < (const vec4 & vec) const
	{
		return x < vec.x && y < vec.y && z < vec.z && w < vec.w;
	}

	bool operator >(const vec4 & vec) const
	{
		return x > vec.x && y > vec.y && z > vec.z && w > vec.w;
	}

	bool operator <= (const vec4 & vec) const
	{
		return x <= vec.x && y <= vec.y && z <= vec.z && w <= vec.w;
	}

	bool operator >=(const vec4 & vec) const
	{
		return x >= vec.x && y >= vec.y && z >= vec.z && w >= vec.w;
	}

public:
	union
	{
		struct
		{
			vec2<FunT> xy;
			vec2<FunT> zw;
		};
		struct
		{
			FunT x;
			FunT y;
			FunT z;
			FunT w;
		};
	};
};

VENUS_END
