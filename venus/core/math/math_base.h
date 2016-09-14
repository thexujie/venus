#pragma once

VENUS_BEG


/**
* @brief �������룬С�� 0.5f ����ȥ�����ڻ���� 0.5f �Ĳ��� 1.0f��
* @return ����ȡ����� int_x ����ֵ��
*/
CORE_API int_x round(float_32 val);

/**
* @brief �������룬С�� 0.5f ����ȥ�����ڻ���� 0.5f �Ĳ��� 1.0f��
* @return ����ȡ����� float_32 ����ֵ��
*/
CORE_API float_32 roundf(float_32 val);

/**
* @brief �������룬���� 1.0f �Ĳ��ֲ��� 1.0f��
* @return ����ȡ����� int_x ����ֵ��
*/
CORE_API int_x ceil(float_32 val);

/**
* @brief �������룬���� 1.0f �Ĳ��ֲ��� 1.0f��
* @return ����ȡ����� float_32 ����ֵ��
*/
CORE_API float_32 ceilf(float_32 val);

/**
* @brief �������룬���� 1.0f �Ĳ�����ȥ��
* @return ����ȡ����� int_x ����ֵ��
*/
CORE_API int_x floor(float_32 val);
/**
* @brief �������룬���� 1.0f �Ĳ�����ȥ��
* @return ����ȡ����� float_32 ����ֵ��
*/
CORE_API float_32 floorf(float_32 val);

/**
* @brief �ж����� float_32 ֵ�Ƿ���ȡ�
* @return ��ȷ��� true�����򷵻� false��
*/
CORE_API bool f32_equal(float_32 val0, float_32 val1);

/**
* @brief �ж�һ�� float_32 ֵ�Ƿ� С�ڻ���� ����һ�� float_32 ֵ����
* @return ���fa <= fb������ true�����򷵻� false��
*/
CORE_API bool f32_less_or_equal(float_32 val0, float_32 val1);

/**
* @brief �ж�һ�� float_32 ֵ�Ƿ� ���ڻ���� ����һ�� float_32 ֵ����
* @return ���fa >= fb������ true�����򷵻� false��
*/
CORE_API bool f32_greater_or_equal(float_32 val0, float_32 val1);

CORE_API float_32 abs(float_32 val);
CORE_API float_32 cos(float_32 val);
CORE_API float_32 sin(float_32 val);
CORE_API float_32 tan(float_32 val);
CORE_API float_32 atan(float_32 val);
CORE_API float_32 cot(float_32 val);
CORE_API float_32 sqrt(float_32 val);
CORE_API float_32 pow(float_32 val, int_x exp);
CORE_API float_32 pow(float_32 val, float_32 exp);
CORE_API int_32 randi32();
CORE_API int_64 randi64();
CORE_API int_x randix();
CORE_API float_32 lerp(float_32 min, float_32 max, float_32 determinant);

VENUS_END
