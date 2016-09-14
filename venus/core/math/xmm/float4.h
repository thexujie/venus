#pragma once

VENUS_BEG

struct float4x4;

struct CORE_API float4
{
	float4() :x(0), y(0), z(0), w(0) {}
	float4(float_32 _x, float_32 _y, float_32 _z, float_32 _w) : x(_x), y(_y), z(_z), w(_w) {}
	float4(const float4 & another) : x(another.x), y(another.y), z(another.z), w(another.w) {}
	float4(const float3 & another) : x(another.x), y(another.y), z(another.z), w(0) {}
	float4(const float3 & another, float_32 _w) : x(another.x), y(another.y), z(another.z), w(_w) {}
	float4(const xmf & v) : x(xmv_x(v)), y(xmv_y(v)), z(xmv_z(v)), w(xmv_w(v)) {}
	float4(const float_32 (& arr)[4]) : x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3]) {}

	float_32 & operator[](int_x iIndex) { return af[iIndex]; }
	const float_32 & operator[](int_x iIndex) const { return ((const float_32 *)this)[iIndex]; }

	float4 & operator = (const float4 & another)
	{
		x = another.x; y = another.y; z = another.z; w = another.w;
		return *this;
	}

	float4 & operator = (const xmf & v)
	{
		const float_32 * value = (const float_32 *)&v;
		x = xmv_x(v); y = xmv_y(v); z = xmv_z(v); w = xmv_w(v);
		return *this;
	}

	float4 & operator = (const float_32 (& arr)[4])
	{
		x = arr[0]; y = arr[0]; z = arr[0]; w = arr[0];
		return *this;
	}

	operator const float_32 *() const { return af; }
	operator float_32 *() { return af; }
	operator xmf() const { return xm_vec_set(x, y, z, w); }
	operator float3() const { return xyz; }

	float_32 length() const;
	float_32 length_sq() const;
	float_32 dot(const float4 & vec) const;
	float4 cross(const float4 & vec1, const float4 & vec2) const;

	float4 & set(float_32 _x, float_32 _y, float_32 _z, float_32 _w)
	{
		x = _x; y = _y; z = _z; w = _w; return *this;
	}
	float4 & set(const float_32(&arr)[4])
	{
		x = arr[0]; y = arr[0]; z = arr[0]; w = arr[0];
		return *this;
	}
	float4 & negate();
	float4 & normalize();

	float4 & add(const float4 & vec);
	float4 & sub(const float4 & vec);
	float4 & mul(const float4 & vec);
	float4 & div(const float4 & vec);

	float4 & lerp(const float4 & min, const float4 & max, float_32 lerp);
	float4 & transform(const float4x4 & matrix);

	float4 operator +() const;
	float4 operator -() const;

	float4 operator + (const float4 & vec) const;
	float4 & operator += (const float4 & vec);

	float4 operator - (const float4 & vec) const;
	float4 & operator -= (const float4 & vec);

	float4 operator * (const float4 & vec) const;
	float4 & operator *= (const float4 & vec);

	float4 operator / (const float4 & vec) const;
	float4 & operator /= (const float4 & vec);

	float4 operator * (float_32 rate) const;
	float4 & operator *= (float_32 rate);
	float4 operator / (float_32 rate) const;
	float4 & operator /= (float_32 rate);

	float4 operator * (const float4x4 & matrix) const;
	float4 & operator *= (const float4x4 & matrix);
public:
	union
	{
		float_32 af[4];
		union
		{
			struct
			{
				float3 xyz;
				float_32 __w;
			};
			struct
			{
				float_32 x;
				float_32 y;
				float_32 z;
				float_32 w;
			};
		};
		int_32 ai[4];
		struct
		{
			int_32 ix;
			int_32 iy;
			int_32 iz;
			int_32 iw;
		};
		uint_32 aui[4];
		struct
		{
			uint_32 uix;
			uint_32 uiy;
			uint_32 uiz;
			uint_32 uiw;
		};
	};
};

inline float4 operator * (float1 rate, float4 vec)
{
	return vec * rate;
}

inline float4 operator / (float1 rate, float4 vec)
{
	return vec / rate;
}

VENUS_END
