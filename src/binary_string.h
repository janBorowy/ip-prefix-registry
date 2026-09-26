#ifndef BINARY_STRING_H
#define BINARY_STRING_H

#include <stdbool.h>
#include <stdint.h>

struct BinaryString {
    uint32_t bits;
    uint8_t length;
};

static const struct BinaryString EMPTY_BINARY_STRING = {
    .bits = 0,
    .length = 0
};

bool is_prefix_of(struct BinaryString prefix, struct BinaryString binary_string);
struct BinaryString get_prefix(struct BinaryString str, uint8_t prefix_len);
struct BinaryString get_suffix(struct BinaryString str, uint8_t suffix_length);
uint8_t get_common_prefix_length(struct BinaryString first, struct BinaryString second);
struct BinaryString append_bits(struct BinaryString str, struct BinaryString appendee);

#endif 