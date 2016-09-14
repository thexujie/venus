#pragma once

VENUS_BEG

template<typename FunT>
class CORE_API vec3
{
public:
	vec3() :x(0), y(0), z(0){}
	vec3(FunT _x, FunT _y, FunT _z) :x(_x), y(_y), z(_z) {}
	vec3(const vec3 & vec) : x(vec.x), y(vec.y), z(vec.z) {}

	void set(FunT _x, FunT _y, FunT _z)
	{
		x = _x; y = _y; z = _z;
	}

	vec3 & operator = (const vec3 & vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	//------------------------------------------------------- 四则运算
	vec3 operator + (const vec3 & vec) const
	{
		return vec3(x + vec.x, y + vec.y, z + vec.z);
	}

	vec3 operator - (const vec3 & vec) const
	{
		return vec3(x - vec.x, y - vec.y, z - vec.z);
	}

	vec3 operator * (const vec3 & vec) const
	{
		return vec3(x * vec.x, y * vec.y, z * vec.z);
	}

	vec3 operator / (const vec3 & vec) const
	{
		return vec3(x / vec.x, y / vec.y, z / vec.z);
	}

	vec3 & operator += (const vec3 & vec)
	{
		x += vec.x; y += vec.y; z += vec.z; return *this;
	}

	vec3 & operator -= (const vec3 & vec)
	{
		x -= vec.x; y -= vec.y; z -= vec.z; return *this;
	}

	vec3 & operator *= (const vec3 & vec)
	{
		x *= vec.x; y *= vec.y; z *= vec.z; return *this;
	}

	vec3 & operator /= (const vec3 & vec)
	{
		x /= vec.x; y /= vec.y; z /= vec.z;return *this;
	}

	vec3 operator * (const FunT & rate) const
	{
		return vec3(x * rate, y * rate, z * rate);
	}

	vec3 operator / (const FunT & rate)
	{
		return vec3(x / rate, y / rate, z / rate);
	}

	vec3 & operator *= (const FunT & rate)
	{
		x *= rate; y *= rate; z *= rate; return *this;
	}

	vec3 & operator /= (const FunT & rate)
	{
		x /= rate; y /= rate; z /= rate; return *this;
	}

	vec3 operator - () const
	{
		return vec3(-x, -y, -z);
	}

	vec3 operator + () const
	{
		return *this;
	}

	//--------------------------------------------------比较运算
	bool operator == (const vec3 & vec) const
	{
		return x == vec.x && y == vec.y && z == vec.z;
	}

	bool operator != (const vec3 & vec) const
	{
		return x != vec.x || y != vec.y || z != vec.z;
	}

	bool operator < (const vec3 & vec) const
	{
		return x < vec.x && y < vec.y && z < vec.z;
	}

	bool operator >(const vec3 & vec) const
	{
		return x > vec.x && y > vec.y && z > vec.z;
	}

	bool operator <= (const vec3 & vec) const
	{
		return x <= vec.x && y <= vec.y && z <= vec.z;
	}

	bool operator >=(const vec3 & vec) const
	{
		return x >= vec.x && y >= vec.y && z >= vec.z;
	}

public:
	union
	{
		struct
		{
			vec2<FunT> xy;
			FunT __z;
		};
		struct
		{
			FunT __x;
			vec2<FunT> yz;
		};
		struct
		{
			FunT x;
			FunT y;
			FunT z;
		};
	};
};

typedef vec3<int_x> intx4;
//typedef vec3x1<float_32> float2x1;

VENUS_END
