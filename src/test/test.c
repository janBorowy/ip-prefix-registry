#include "../../lib/munit/munit.h"
#include "uint32_util_tests.h"
#include "singly_list_tests.h"
#include "radix_uint32_trie_tests.h"

static MunitResult test_uint32_util(const MunitParameter params[], void* data) {
    is_prefix_of_test();
    get_prefix_test();
    get_suffix_test();
    get_common_prefix_length_test();
    return MUNIT_OK;
}

static MunitResult test_linked_list(const MunitParameter params[], void* data) {
    singly_list_test();
    return MUNIT_OK;
}

static MunitResult test_radix_uint32_trie(const MunitParameter params[], void* data) {
    radix_uint32_trie_add_test();
    radix_uint32_trie_get_longest_prefix_test();
    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    {
        (char *) "/ip-registry/uint32_util",
        test_uint32_util,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char *) "/ip-registry/linked_list",
        test_linked_list,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char *) "/ip-registry/radix_uint32_trie",
        test_radix_uint32_trie,
        NULL,
        NULL,
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