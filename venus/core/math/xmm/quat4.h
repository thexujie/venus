#pragma once

VENUS_BEG

struct float4x4;

struct CORE_API quat4
{
	quat4() :x(0), y(0), z(0), w(0) {}
	quat4(float_32 _x, float_32 _y, float_32 _z, float_32 _w) : x(_x), y(_y), z(_z), w(_w) {}
	quat4(const quat4 & another) : x(another.x), y(another.y), z(another.z), w(another.w) {}
	quat4(const xmf & v) : x(xmv_x(v)), y(xmv_y(v)), z(xmv_z(v)), w(xmv_w(v)) {}
	quat4(float_32 arr[4]) : x(arr[0]), y(arr[1]), z(arr[2]), w(arr[3]) {}

	float_32 & operator[](int_x iIndex) { return af[iIndex]; }
	const float_32 & operator[](int_x iIndex) const { return ((const float_32 *)this)[iIndex]; }

	quat4 & operator = (const quat4 & another)
	{
		x = another.x; y = another.y; z = another.z; w = another.w;
		return *this;
	}

	quat4 & operator = (const xmf & v)
	{
		const float_32 * value = (const float_32 *)&v;
		x = xmv_x(v); y = xmv_y(v); z = xmv_z(v); w = xmv_w(v);
		return *this;
	}

	quat4 & operator = (float_32 arr[4])
	{
		x = arr[0]; y = arr[0]; z = arr[0]; w = arr[0];
		return *this;
	}

	operator const float_32 *() const { return af; }
	operator float_32 *() { return af; }
	operator xmf() const { return xm_vec_set(x, y, z, w); }

	float_32 length() const;
	float_32 length_sq() const;
	float_32 dot(const quat4 & vec) const;
	quat4 cross(const quat4 & vec1, const quat4 & vec2) const;

	quat4 & set(float_32 _x, float_32 _y, float_32 _z, float_32 _w);
	quat4 & negate();
	quat4 & normalize();

	quat4 & add(const quat4 & vec);
	quat4 & sub(const quat4 & vec);
	quat4 & mul(const quat4 & vec);
	quat4 & div(const quat4 & vec);

	quat4 & lerp(const quat4 & min, const quat4 & max, float_32 lerp);

	quat4 operator +() const;
	quat4 operator -() const;

	quat4 operator + (const quat4 & vec) const;
	quat4 & operator += (const quat4 & vec);

	quat4 operator - (const quat4 & vec) const;
	quat4 & operator -= (const quat4 & vec);

	quat4 operator * (const quat4 & vec) const;
	quat4 & operator *= (const quat4 & vec);

	quat4 operator / (const quat4 & vec) const;
	quat4 & operator /= (const quat4 & vec);

	quat4 operator * (float_32 rate) const;
	quat4 & operator *= (float_32 rate);
	quat4 operator / (float_32 rate) const;
	quat4 & operator /= (float_32 rate);

	quat4 operator * (const float4x4 & matrix) const;
	quat4 & operator *= (const float4x4 & matrix);
public:
	union
	{
		float_32 af[4];
		struct
		{
			float_32 x;
			float_32 y;
			float_32 z;
			float_32 w;
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

VENUS_END
