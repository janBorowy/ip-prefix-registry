#include "binary_string.h"
#include <assert.h>

bool get_nth_rightmost_bit(struct BinaryString str, uint8_t n);
void assert_unused_bits_are_zero(struct BinaryString str);

bool is_prefix_of(struct BinaryString prefix, struct BinaryString str) {
    assert_unused_bits_are_zero(prefix);

    if (prefix.length == 0) {
        return true;
    }

    if (prefix.length > str.length) {
        return false;
    }

    return str.bits >> str.length - prefix.length == prefix.bits;
}

struct BinaryString get_prefix(struct BinaryString str, uint8_t prefix_len) {
    if (prefix_len == 0) {
        return EMPTY_BINARY_STRING; 
    }

    if (prefix_len > str.length) {
        return str;
    }

    return (struct BinaryString) {
        .bits = str.bits >> str.length - prefix_len,
        .length = prefix_len
    };
}

struct BinaryString get_suffix(struct BinaryString str, uint8_t suffix_length) {
    if (suffix_length == 0) {
        return EMPTY_BINARY_STRING;
    }

    if (suffix_length >= str.length) {
        return str;
    }

    return (struct BinaryString) {
        .bits = (UINT32_MAX >> 32 - suffix_length) & str.bits,
        .length = suffix_length
    };
}

uint8_t get_common_prefix_length(struct BinaryString first, struct BinaryString second) {
    assert_unused_bits_are_zero(first);
    assert_unused_bits_are_zero(second);

    int i = first.length, j = second.length;
    int common_len = 0;
    while (i > 0 && j > 0) {
        if (get_nth_rightmost_bit(first, i) == get_nth_rightmost_bit(second, j)) {
            common_len++;
            --i;
            --j;
        } else {
            break;
        }
    }
    return common_len;
}

struct BinaryString append_bits(struct BinaryString str, struct BinaryString appendee) {
    assert_unused_bits_are_zero(str);
    assert_unused_bits_are_zero(appendee);
    assert(str.length + appendee.length <= 32);

    return (struct BinaryString) {
        .bits = (str.bits << appendee.length) | appendee.bits,
        .length = str.length + appendee.length
    };
}

bool get_nth_rightmost_bit(struct BinaryString str, uint8_t n) {
    return (str.bits >> (n - 1)) & 1u;
}

void assert_unused_bits_are_zero(struct BinaryString str) {
    assert(str.length == 32 || (((UINT32_MAX >> str.length) << str.length) & str.bits) == 0);
}