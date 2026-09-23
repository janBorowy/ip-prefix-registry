#ifndef UINT32_UTIL
#define UINT32_UTIL

#include <stdbool.h>
#include <stdint.h>

bool is_prefix_of(uint32_t prefix_val, uint8_t prefix_len, uint32_t num_val, uint8_t num_len);
uint32_t get_suffix(uint32_t value, uint8_t suffix_length);
uint8_t get_common_prefix_length(uint32_t first_value, uint8_t first_len, uint32_t second_value, uint8_t second_len);

#endif 