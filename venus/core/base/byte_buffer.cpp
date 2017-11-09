#include "stdafx.h"
#include "byte_buffer.h"

VENUS_BEG

byte_buffer_t::byte_buffer_t(int_x _size) : buffer(nullptr), size(0), capability(0)
{
	if(_size > 0)
	{
		buffer = new byte_t[_size];
		capability = size = _size;
	}
}

byte_buffer_t::byte_buffer_t(byte_buffer_t & another)
{
	buffer = another.buffer;
	size = another.size;
	capability = another.capability;

	another.buffer = nullptr;
	another.size = 0;
	another.capability = 0;
}

byte_buffer_t::~byte_buffer_t()
{
	SafeDelete(buffer);
}

byte_buffer_t & byte_buffer_t::operator = (byte_buffer_t & another)
{
	buffer = another.buffer;
	size = another.size;
	capability = another.capability;

	another.buffer = nullptr;
	another.size = 0;
	another.capability = 0;

	return *this;
}

void byte_buffer_t::reallocate(int_x _capability)
{
	if(_capability < 0)
		_capability = 0;

	if(_capability <= 0)
	{
		SafeDelete(buffer);
		buffer = nullptr;
		capability = 0;
	}
	else
	{
		byte_t * _buffer = new byte_t[_capability];
		buffcpy8(_buffer, _capability, buffer, size);
		SafeDelete(buffer);
		buffer = _buffer;
		capability = _capability;
	}
	if(size > _capability)
		size = _capability;
}

void byte_buffer_t::reallocate(int_x _capability, int_x _size)
{
	if(_capability < 0)
		_capability = 0;

	if(_capability <= 0)
	{
		SafeDelete(buffer);
		buffer = nullptr;
		capability = 0;
	}
	else
	{
		byte_t * _buffer = new byte_t[_capability];
		buffcpy8(_buffer, _capability, buffer, size);
		SafeDelete(buffer);
		buffer = _buffer;
		capability = _capability;
	}

	if(_size < 0)
		_size = 0;
	else if(_size > _capability)
		_size = _capability;
	else {}

	size = _size;
}

void byte_buffer_t::reallocate_increase(int_x _capability)
{
	int_x temp = capability;
	if(_capability > capability)
	{
		if(!temp)
			temp = 1;

		while(temp < _capability)
			temp <<= 1;

		byte_t * _buffer = new byte_t[temp];
		buffcpy8(_buffer, temp, buffer, size);
		SafeDelete(buffer);
		buffer = _buffer;
		capability = temp;
	}
}

void byte_buffer_t::resize(int_x _size)
{
	if(_size < 0 || _size > capability)
		throw exp_illegal_argument();
	size = _size;
}

VENUS_END
