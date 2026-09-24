#include "uint32_util.h"
#include <assert.h>

bool get_nth_rightmost_bit(uint32_t value, uint8_t n);
void assert_unused_bits_are_zero(uint32_t value, uint8_t len);

bool is_prefix_of(uint32_t prefix_val, uint8_t prefix_len, uint32_t num_val, uint8_t num_len) {
    assert_unused_bits_are_zero(prefix_val, prefix_len);

    if (prefix_len == 0) {
        return true;
    }

    if (prefix_len > num_len) {
        return false;
    }

    return num_val >> num_len - prefix_len == prefix_val;
}

uint32_t get_prefix(uint32_t value, uint8_t value_len, uint8_t prefix_len) {
    if (prefix_len == 0) {
        return 0;
    }

    if (prefix_len > value_len) {
        return value_len;
    }

    return value >> value_len - prefix_len;
}

uint32_t get_suffix(uint32_t value, uint8_t suffix_length) {
    if (suffix_length == 0) {
        return 0;
    }

    if (suffix_length == 32) {
        return value;
    }

    return (UINT32_MAX >> 32 - suffix_length) & value;
}

uint8_t get_common_prefix_length(uint32_t first_value, uint8_t first_len, uint32_t second_value, uint8_t second_len) {
    assert_unused_bits_are_zero(first_value, first_len);
    assert_unused_bits_are_zero(second_value, second_len);

    int i = first_len, j = second_len;
    int common_len = 0;
    while (i > 0 && j > 0) {
        if (get_nth_rightmost_bit(first_value, i) == get_nth_rightmost_bit(second_value, j)) {
            common_len++;
            --i;
            --j;
        } else {
            break;
        }
    }
    return common_len;
}

bool get_nth_rightmost_bit(uint32_t value, uint8_t n) {
    return (value >> (n - 1)) & 1u;
}

void assert_unused_bits_are_zero(uint32_t value, uint8_t len) {
    assert(len == 32 || (((UINT32_MAX >> len) << len) & value) == 0);
}