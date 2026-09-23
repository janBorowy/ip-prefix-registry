#include "../../lib/munit/munit.h"
#include "../uint32_util.h"

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

static MunitResult test_uint32_util(const MunitParameter params[], void* data) {
    is_prefix_of_test();
    get_suffix_test();
    get_common_prefix_length_test();
    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    {
        (char *) "/ip-registry/uint32_util",
        test_uint32_util,
        NULL,
        NULL/*  */,
        MUNIT_TEST_OPTION_NONE,
        NULL
    }
};

static const MunitSuite test_suite = {
    (char *) "",
    test_suite_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char *argv[]) {
    return munit_suite_main(&test_suite, NULL, argc, argv);
}