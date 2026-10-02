#include "binary_string_tests.h"
#include "../binary_string.h"
#include "assertions.h"

struct BinaryString bin_str_from(const char *str);

void is_prefix_of_test() {
    assert_true(is_prefix_of(bin_str_from("01"), bin_str_from("0111")));
    assert_true(is_prefix_of(bin_str_from("0"), bin_str_from("0111")));
    assert_false(is_prefix_of(bin_str_from("1"), bin_str_from("0111")));

    assert_false(is_prefix_of(bin_str_from("0"), bin_str_from("1000")));
    assert_false(is_prefix_of(bin_str_from("111"), bin_str_from("1100")));
    assert_false(is_prefix_of(bin_str_from("1001"), bin_str_from("1000")));

    assert_true(is_prefix_of(bin_str_from(""), bin_str_from("")));
    assert_true(is_prefix_of(bin_str_from(""), bin_str_from("1")));

    assert_true(is_prefix_of(bin_str_from("1"), bin_str_from("11111111111111111111111111111111")));
    assert_true(is_prefix_of(bin_str_from("11111111111111111111111111111111"),
                                   bin_str_from("11111111111111111111111111111111")));
    assert_true(is_prefix_of(bin_str_from("1010110010110101"),
                                   bin_str_from("10101100101101011011011010101010")));
}

void get_prefix_test() {
    struct BinaryString prefix = get_prefix(bin_str_from("1001"), 0);
    assert_equal(prefix.bits, 0);
    assert_equal(prefix.length, 0);

    prefix = get_prefix(bin_str_from("1001"), 1);
    assert_equal(prefix.bits, 0b1);
    assert_equal(prefix.length, 1);

    prefix = get_prefix(bin_str_from("1001"), 2);
    assert_equal(prefix.bits, 0b10);
    assert_equal(prefix.length, 2);

    prefix = get_prefix(bin_str_from("1001"), 3);
    assert_equal(prefix.bits, 0b100);
    assert_equal(prefix.length, 3);

    prefix = get_prefix(bin_str_from("1001"), 4);
    assert_equal(prefix.bits, 0b1001);
    assert_equal(prefix.length, 4);

    prefix = get_prefix(bin_str_from("11110000"), 4);
    assert_equal(prefix.bits, 0b1111);
    assert_equal(prefix.length, 4);

    prefix = get_prefix(bin_str_from("10101011110011011110111110101011"), 16);
    assert_equal(prefix.bits, 0xABCD);
    assert_equal(prefix.length, 16);

    prefix = get_prefix(bin_str_from("11111111111111111111111111111111"), 24);
    assert_equal(prefix.bits, 0xFFFFFF);
    assert_equal(prefix.length, 24);

    prefix = get_prefix(bin_str_from("11111111111111111111111111111111"), 32);
    assert_equal(prefix.bits, 0xFFFFFFFF);
    assert_equal(prefix.length, 32);
}

void get_suffix_test() {
    struct BinaryString suffix = get_suffix(bin_str_from("1001"), 0);
    assert_equal(suffix.bits, 0);
    assert_equal(suffix.length, 0);

    suffix = get_suffix(bin_str_from("1001"), 1);
    assert_equal(suffix.bits, 0b1);
    assert_equal(suffix.length, 1);

    suffix = get_suffix(bin_str_from("1001"), 2);
    assert_equal(suffix.bits, 0b01);
    assert_equal(suffix.length, 2);

    suffix = get_suffix(bin_str_from("1011"), 2);
    assert_equal(suffix.bits, 0b11);
    assert_equal(suffix.length, 2);

    suffix = get_suffix(bin_str_from("10101011110011011110111110101011"), 32);
    assert_equal(suffix.bits, 0xABCDEFAB);
    assert_equal(suffix.length, 32);

    suffix = get_suffix(bin_str_from("10101011110011011110111110101011"), 8);
    assert_equal(suffix.bits, 0xAB);
    assert_equal(suffix.length, 8);
}

void get_common_prefix_length_test() {
    struct BinaryString first = bin_str_from("0001");
    struct BinaryString second = bin_str_from("1101");
    assert_equal(get_common_prefix_length(first, second), 0);

    first = bin_str_from("1001");
    second = bin_str_from("1101");
    assert_equal(get_common_prefix_length(first, second), 1);

    first = bin_str_from("1001");
    second = bin_str_from("1011");
    assert_equal(get_common_prefix_length(first, second), 2);

    first = bin_str_from("1001");
    second = bin_str_from("1000");
    assert_equal(get_common_prefix_length(first, second), 3);

    first = bin_str_from("1001");
    second = bin_str_from("1001");
    assert_equal(get_common_prefix_length(first, second), 4);
}

void append_bits_test() {
    struct BinaryString result = append_bits(bin_str_from("101"), bin_str_from("01"));
    assert_equal(result.bits, 0b10101);
    assert_equal(result.length, 5);

    result = append_bits(bin_str_from("1101"), bin_str_from("10"));
    assert_equal(result.bits, 0b110110);
    assert_equal(result.length, 6);

    result = append_bits(bin_str_from("0001001000110100"), bin_str_from("1010101111001101"));
    assert_equal(result.bits, 0x1234ABCD);
    assert_equal(result.length, 32);

    result = append_bits(bin_str_from(""), bin_str_from("11"));
    assert_equal(result.bits, 0b11);
    assert_equal(result.length, 2);

    result = append_bits(bin_str_from("1010101111001101"), bin_str_from(""));
    assert_equal(result.bits, 0xABCD);
    assert_equal(result.length, 16);

    result = append_bits(bin_str_from("11111111111111111111111111111111"), bin_str_from(""));
    assert_equal(result.bits, UINT32_MAX);
    assert_equal(result.length, 32);
}

struct BinaryString bin_str_from(const char *str) {
    uint32_t val = 0;
    uint8_t len = 0;

    while (*str != '\0') {
        val <<= 1;
        if (*str == '1') {
            val += 1;
        }
        len += 1;
        str += 1;
    }

    return (struct BinaryString) {
        .bits = val,
        .length = len
    };
}