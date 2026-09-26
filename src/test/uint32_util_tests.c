#include "uint32_util_tests.h"
#include "../uint32_util.h"
#include "../../lib/munit/munit.h"

void is_prefix_of_test() {
    munit_assert_true(is_prefix_of(0b01, 2, 0b0111, 4));
    munit_assert_true(is_prefix_of(0b0, 1, 0b0111, 4));
    munit_assert_false(is_prefix_of(0b1, 1, 0b0111, 4));

    munit_assert_false(is_prefix_of(0b0, 1, 0b1000, 4));
    munit_assert_false(is_prefix_of(0b111, 3, 0b1100, 4));
    munit_assert_false(is_prefix_of(0b1001, 4, 0b1000, 4));

    munit_assert_true(is_prefix_of(0, 0, 0, 0));
    munit_assert_true(is_prefix_of(0, 0, 0b1, 1));

    munit_assert_true(is_prefix_of(1, 1, UINT32_MAX, 32));
    munit_assert_true(is_prefix_of(UINT32_MAX, 32, UINT32_MAX, 32));
    munit_assert_true(is_prefix_of(0xABCD, 16, 0xABCD1234, 32));
}

void get_prefix_test() {
    munit_assert_uint32(get_prefix(0b1001, 4, 0), ==, 0);
    munit_assert_uint32(get_prefix(0b1001, 4, 1), ==, 0b1);
    munit_assert_uint32(get_prefix(0b1001, 4, 2), ==, 0b10);
    munit_assert_uint32(get_prefix(0b1001, 4, 3), ==, 0b100);
    munit_assert_uint32(get_prefix(0b1001, 4, 4), ==, 0b1001);
    munit_assert_uint32(get_prefix(0b11110000, 8, 4), ==, 0b1111);
    munit_assert_uint32(get_prefix(0xABCDEFAB, 32, 16), ==, 0xABCD);
    munit_assert_uint32(get_prefix(UINT32_MAX, 32, 24), ==, 0xFFFFFF);
    munit_assert_uint32(get_prefix(UINT32_MAX, 32, 32), ==, 0xFFFFFFFF);
}

void get_suffix_test() {
    munit_assert_uint32(get_suffix(0b1001, 0), ==, 0);
    munit_assert_uint32(get_suffix(0b1001, 1), ==, 0b1);
    munit_assert_uint32(get_suffix(0b1001, 2), ==, 0b1);
    munit_assert_uint32(get_suffix(0b1011, 2), ==, 0b11);
    munit_assert_uint32(get_suffix(0xABCDEFAB, 32), ==, 0xABCDEFAB);
    munit_assert_uint32(get_suffix(0xABCDEFAB, 8), ==, 0xAB);
}

void get_common_prefix_length_test() {
    munit_assert_uint32(get_common_prefix_length(0b0001, 4, 0b1101, 4), ==, 0);
    munit_assert_uint32(get_common_prefix_length(0b1001, 4, 0b1101, 4), ==, 1);
    munit_assert_uint32(get_common_prefix_length(0b1001, 4, 0b1011, 4), ==, 2);
    munit_assert_uint32(get_common_prefix_length(0b1001, 4, 0b1000, 4), ==, 3);
    munit_assert_uint32(get_common_prefix_length(0b1001, 4, 0b1001, 4), ==, 4);
}

void append_bits_test() {
    munit_assert_uint32(append_bits(0b101, 3, 0b01, 2), ==, 0b10101);
    munit_assert_uint32(append_bits(0b1101, 4, 0b10, 2), ==, 0b110110);
    munit_assert_uint32(append_bits(0x1234, 16, 0xABCD, 16), ==, 0x1234ABCD);
    munit_assert_uint32(append_bits(0, 0, 0b11, 2), ==, 0b11);
    munit_assert_uint32(append_bits(0xABCD, 16, 0, 0), ==, 0xABCD);
    munit_assert_uint32(append_bits(UINT32_MAX, 32, 0, 0), ==, UINT32_MAX);
}
