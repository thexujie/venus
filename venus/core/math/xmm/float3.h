#pragma once

VENUS_BEG

struct float2;
struct float4;
struct float4x4;

struct CORE_API float3
{
	inline float3() :x(0), y(0), z(0) {}
	inline float3(const float3 & another) : x(another.x), y(another.y), z(another.z) {}
	inline float3(const xmf & m128) : x(xmv_x(m128)), y(xmv_y(m128)), z(xmv_z(m128)) {}
	inline float3(float_32 _x, float_32 _y, float_32 _z) : x(_x), y(_y), z(_z) {}
	inline float3(const float_32 (& arr)[3]) : x(arr[0]), y(arr[1]), z(arr[2]) {}
	inline float3 & operator=(const float_32 (& arr)[3])
	{
		x = arr[0]; y = arr[0]; z = arr[0];
		return *this;
	}

	inline float_32 & operator[](int_x iIndex) { return ((float_32 *)this)[iIndex]; }
	inline const float_32 & operator[](int_x iIndex) const { return ((const float_32 *)this)[iIndex]; }

	inline operator xmf() const { return xm_vec_set(x, y, z, 0);}
	inline float3 & operator = (xmf v) { x = xmv_x(v); y = xmv_y(v); z = xmv_z(v); return *this;}
	inline float3 & operator = (const float3 & another) { x = another.x; y = another.y; z = another.z; return *this;}
	// -----------------------------------------------------------
	inline float3 & set(float_32 _x, float_32 _y, float_32 _z)
	{
		x = _x; y = _y; z = _z;
		return *this;
	}

	inline float3 & set(const float_32 (&arr)[3])
	{
		x = arr[0]; y = arr[0]; z = arr[0];
		return *this;
	}

	float_32 length() const;
	float_32 length_sq() const;
	float_32 dot(const float3 & vec) const;
	float3 cross(const float3 & vec) const;
	float3 abs() const;

	float3 & negate();
	float3 & normalize();
	float3 & add(const float3 & vec);
	float3 & sub(const float3 & vec);
	float3 & mul(const float3 & vec);
	float3 & div(const float3 & vec);

	float3 operator +() const;
	float3 operator -() const;

	float3 operator + (const float3 & vec) const;
	float3 & operator += (const float3 & vec);

	float3 operator - (const float3 & vec) const;
	float3 & operator -= (const float3 & vec);

	float3 operator * (float_32 rate) const;
	float3 operator * (const float3 & vec) const;
	float3 & operator *= (float_32 rate);
	float3 & operator *= (const float3 & vec);

	float3 operator / (float_32 rate) const;
	float3 operator / (const float3 & vec) const;
	float3 & operator /= (float_32 rate);
	float3 & operator /= (const float3 & vec);

	inline bool operator == (const float3 & vec) const
	{
		xmf diff = xm_vec_abs(xm_vec_sub(*this, vec));
		return !!xmi_x(xm_vec_less(diff, xmf_epsilon));
	}
	inline bool operator != (const float3 & vec) const
	{
		return !operator==(vec);
	}

	float3 & lerp(const float3 & min, const float3 & max, float_32 lerp);
	float3 & transform(const float4x4 & matrix);

	float3 operator * (const float4x4 & matrix) const;
	float3 operator *= (const float4x4 & matrix);

public:
	union
	{
		float_32 af[3];
		struct
		{
			float_32 x;
			float_32 y;
			float_32 z;
		};
		struct
		{
			float2 xy;
			float1 __z;
		};
		struct
		{
			float2 __x;
			float1 yz;
		};
	};
};

inline float3 operator * (float1 rate, float3 vec)
{
	return vec * rate;
}

inline float3 operator / (float1 rate, float3 vec)
{
	return vec / rate;
}

VENUS_END
