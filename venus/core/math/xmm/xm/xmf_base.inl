#pragma once

inline float_32 xm_scalar_sin(float_32 angle)
{
	// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
	float_32 quotient = xm_1_of_2pi * angle;
	if(angle >= 0.0f)
	{
		quotient = (float_32)((int_32)(quotient + 0.5f));
	}
	else
	{
		quotient = (float_32)((int_32)(quotient - 0.5f));
	}
	float_32 y = angle - xm_2pi * quotient;

	// Map y to [-pi/2,pi/2] with sin(y) = sin(angle).
	if(y > xm_1pi_of_2)
	{
		y = xm_pi - y;
	}
	else if(y < -xm_1pi_of_2)
	{
		y = -xm_pi - y;
	}
	else {}

	// 11-degree minimax approximation
	float_32 y2 = y * y;
	return (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
}

inline void xm_scalar_sin_cos(float_32 * sin, float_32 * cos, float_32 value)
{
	// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
	float_32 quotient = xm_1_of_2pi * value;
	if(value >= 0.0f)
		quotient = (float_32)((int_32)(quotient + 0.5f));
	else
		quotient = (float_32)((int_32)(quotient - 0.5f));
	float_32 y = value - xm_2pi * quotient;

	// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
	float_32 sign = 0.0f;
	if(y > xm_1pi_of_2)
	{
		y = xm_pi - y;
		sign = -1.0f;
	}
	else if(y < -xm_1pi_of_2)
	{
		y = -xm_pi - y;
		sign = -1.0f;
	}
	else
	{
		sign = +1.0f;
	}

	float_32 y2 = y * y;

	// 11-degree minimax approximation
	*sin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

	// 10-degree minimax approximation
	float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
	*cos = sign*p;
}
