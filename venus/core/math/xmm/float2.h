#pragma once

VENUS_BEG

struct CORE_API float2
{
	inline float2() :x(0), y(0) {}
	inline float2(const float2 & another) : x(another.x), y(another.y) {}
	inline float2(const xmf & v) : x(xmv_x(v)), y(xmv_y(v)) {}
	inline float2(float_32 _x, float_32 _y) : x(_x), y(_y) {}
	inline float2(const float_32 (& arr)[2]) : x(arr[0]), y(arr[1]) {}
	inline float2 & operator=(const float_32 (& arr)[2])
	{
		x = arr[0]; y = arr[0]; 
		return *this;
	}

	inline float_32 & operator[](int_x iIndex) { return ((float_32 *)this)[iIndex]; }
	inline const float_32 & operator[](int_x iIndex) const { return ((const float_32 *)this)[iIndex]; }

	inline operator xmf() const
	{
		return xmf(x, y, 0.0f, 0.0f);
	}
	inline float2 & operator = (xmf v)
	{
		x = xmv_x(v); y = xmv_y(v);
		return *this;
	}

	inline float2 & operator = (const float2 & another)
	{
		x = another.x; y = another.y;
		return *this;
	}

	//----------------------------------
	float2 & set(float_32 _x, float_32 _y) { x = _x; y = _y; return *this; }
	float2 & set(const float_32(&arr)[2]) { x = arr[0]; y = arr[1]; return *this; }


	float2 operator +() const;
	float2 operator -() const;

	float2 operator + (const float2 & vec) const;
	float2 & operator += (const float2 & vec);

	float2 operator - (const float2 & vec) const;
	float2 & operator -= (const float2 & vec);

	float2 operator * (float_32 rate) const;
	float2 operator * (const float2 & vec) const;
	float2 & operator *= (float_32 rate);
	float2 & operator *= (const float2 & vec);

	float2 operator / (float_32 rate) const;
	float2 operator / (const float2 & vec) const;
	float2 & operator /= (float_32 rate);
	float2 & operator /= (const float2 & vec);

	float2 & lerp(const float2 & min, const float2 & max, float_32 lerp);
public:
	union
	{
		float_32 af[2];
		struct
		{
			float_32 x;
			float_32 y;
		};
	};
};

inline float2 operator * (float1 rate, float2 vec)
{
	return vec * rate;
}

inline float2 operator / (float1 rate, float2 vec)
{
	return vec / rate;
}

VENUS_END
