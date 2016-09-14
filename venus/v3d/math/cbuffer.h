#pragma once

VENUS_BEG
/**
 * float4 ¶ÔÆëµÄ   
 */
 template<int_x SIZE>
class V3D_API cbuffer
{
public:
	cbuffer() :size(0)
	{
	}
	~cbuffer()
	{
	}

	int_x push(const float1 & _val)
	{
		if(!prepare(1, 1))
			return -1;

		buffer[size++] = _val;
		return size;
	}

	int_x push(const float1 * _pval, int_x _count)
	{
		if(!prepare(16, _count))
			return -1;

		buffcpy32(buffer + size, _count, _pval, _count);
		size += _count;
		return size;
	}

	int_x push(const float2 & _val)
	{
		if(!prepare(2, 1))
			return -1;

		buffcpy32(buffer + size, 2, _val.af, 2);
		size += 2;
		return size;
	}

	int_x push(const float3 & _val)
	{
		if(!prepare(3, 1))
			return -1;

		buffcpy32(buffer + size, 3, _val.af, 3);
		size += 3;
		return size;
	}

	int_x push(const float4 & _val)
	{
		if(!prepare(4, 1))
			return -1;

		buffcpy32(buffer + size, 4, _val.af, 4);
		size += 4;
		return size;
	}

	int_x push(const float4 * _pval, int_x _count)
	{
		if(!prepare(4, _count))
			return -1;

		buffcpy32(buffer + size, 4 * _count, _pval, 4 * _count);
		size += 4 * _count;
		return size;
	}

	int_x push(const float4x4 & _val)
	{
		if(!prepare(16, 1))
			return -1;

		buffcpy32(buffer + size, 16, _val.af, 16);
		size += 16;
		return size;
	}

	int_x push(const float4x4 * _pval, int_x _count)
	{
		if(!prepare(16, _count))
			return -1;

		for(int_x cnt = 0; cnt < _count; ++cnt)
		{
			const float4x4 & val = _pval[cnt];
			buffcpy32(buffer + size, 16, val.af, 16);
			size += 16;
		}
		return size;
	}

	int_x offset(int _size)
	{
		if(size + _size < SIZE)
			size += _size;
		return size;
	}

	bool set(int_x _index, const float1 & _val)
	{
		if(0 <= _index && _index + 1 < SIZE)
			buffer[_index] = _val;
	}

	bool set(int_x _index, const float2 & _val)
	{
		if(0 <= _index && _index + 2 < SIZE)
		{
			buffer[_index] = _val.x;
			buffer[_index + 1] = _val.y;
		}
	}

	bool set(int_x _index, const float3 & _val)
	{
		if(0 <= _index && _index + 3 < SIZE)
		{
			buffer[_index] = _val.x;
			buffer[_index + 1] = _val.y;
			buffer[_index + 2] = _val.z;
		}
	}

	bool set(int_x _index, const float4 & _val)
	{
		if(0 <= _index && _index + 4 < SIZE)
		{
			buffer[_index] = _val.x;
			buffer[_index + 1] = _val.y;
			buffer[_index + 2] = _val.z;
			buffer[_index + 3] = _val.w;
		}
	}

	bool set(int_x _index, const float4x4 & _val)
	{
		if(0 <= _index && _index + 16 < SIZE)
		{
			for(int_x cnt = 0; cnt < 16; ++cnt)
				buffer[_index + cnt] = _val.af[cnt];
			return true;
		}
		else
			return false;
	}

	bool prepare(int_x _stride, int_x _count)
	{
		if(_stride <= 0)
			return false;

		int_x index = size;

		if(_stride == 1)
			;
		else if(_stride == 2)
		{
			if(index % 4 > 2)
			{
				++index;
			}
		}
		else if(_stride == 3)
		{
			if(index % 4 > 1)
			{
				index += 4 - index % 4;
			}
		}
		else if(_stride == 4 || _stride == 8 || _stride == 12 || _stride == 16)
		{
			if(index % 4 > 0)
			{
				index += 4 - index % 4;
			}
		}
		else
			return false;

		if(index + _stride * _count <= SIZE)
		{
			size = index;
			return true;
		}
		else
			return false;
	}

public:
	int_x size;
	float_32 buffer[SIZE];
};

VENUS_END
