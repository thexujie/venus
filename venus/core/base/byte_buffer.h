#pragma once

VENUS_BEG

/**
 *  ����һ����С���ֽ����ݣ��ɸ��ƣ�ʹ�ø��ƹ��캯���� = ��������ת����������Ȩ��
 */
class CORE_API byte_buffer_t
{
public:
	byte_buffer_t(int_x _size = 0);
	byte_buffer_t(byte_buffer_t & another);
	~byte_buffer_t();

	byte_buffer_t & operator = (byte_buffer_t & another);

	void reallocate(int_x _capability);
	void reallocate(int_x _capability, int_x _size);
	void reallocate_increase(int_x _capability);

	void resize(int_x _size);

	operator byte_t * () { return buffer;}
	operator const byte_t * () const { return buffer;}

public:
	byte_t * buffer;
	int_x size;
	int_x capability;
};

VENUS_END
