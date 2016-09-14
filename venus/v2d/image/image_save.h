#pragma once

VENUS_BEG

typedef err_t(*image_save_write_fun_t)(const void * buffer, int_x size, void * userdata);
typedef err_t(*image_save_fun_t)(const image_data_t * data, image_save_write_fun_t pfn_write, void * userdata);

VENUS_END