#pragma once

VENUS_BEG

CORE_API int_x tls_create();
CORE_API void tls_destroy(int_x iTls);
CORE_API void tls_set(int_x iTls, int_x iValue);
CORE_API int_x tls_get(int_x iTls);

CORE_API void thread_set_name(int_x iThreadId, const char_8 * szName);
CORE_API void thread_set_name(int_x iThreadId, const char_16 * szName);

VENUS_END
