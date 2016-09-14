#pragma once

VENUS_BEG


/**
* @brief 四舍五入，小于 0.5f 的舍去，大于或等于 0.5f 的补齐 1.0f。
* @return 返回取整后的 int_x 类型值。
*/
CORE_API int_x round(float_32 val);

/**
* @brief 四舍五入，小于 0.5f 的舍去，大于或等于 0.5f 的补齐 1.0f。
* @return 返回取整后的 float_32 类型值。
*/
CORE_API float_32 roundf(float_32 val);

/**
* @brief 向上舍入，不足 1.0f 的部分补齐 1.0f。
* @return 返回取整后的 int_x 类型值。
*/
CORE_API int_x ceil(float_32 val);

/**
* @brief 向上舍入，不足 1.0f 的部分补齐 1.0f。
* @return 返回取整后的 float_32 类型值。
*/
CORE_API float_32 ceilf(float_32 val);

/**
* @brief 向下舍入，不足 1.0f 的部分舍去。
* @return 返回取整后的 int_x 类型值。
*/
CORE_API int_x floor(float_32 val);
/**
* @brief 向下舍入，不足 1.0f 的部分舍去。
* @return 返回取整后的 float_32 类型值。
*/
CORE_API float_32 floorf(float_32 val);

/**
* @brief 判断两个 float_32 值是否相等。
* @return 相等返回 true，否则返回 false。
*/
CORE_API bool f32_equal(float_32 val0, float_32 val1);

/**
* @brief 判断一个 float_32 值是否 小于或等于 另外一个 float_32 值。。
* @return 如果fa <= fb，返回 true，否则返回 false。
*/
CORE_API bool f32_less_or_equal(float_32 val0, float_32 val1);

/**
* @brief 判断一个 float_32 值是否 大于或等于 另外一个 float_32 值。。
* @return 如果fa >= fb，返回 true，否则返回 false。
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
