#include "../../lib/munit/munit.h"
#include "binary_string_tests.h"
#include "singly_list_tests.h"
#include "radix_binary_trie_tests.h"
#include "prefix_tree_tests.h"

static MunitResult test_binary_string(const MunitParameter params[], void* data) {
    is_prefix_of_test();
    get_prefix_test();
    get_suffix_test();
    get_common_prefix_length_test();
    append_bits_test();
    return MUNIT_OK;
}

static MunitResult test_singly_list(const MunitParameter params[], void* data) {
    singly_list_test();
    singly_list_delete_test();
    return MUNIT_OK;
}

static MunitResult test_radix_binary_trie(const MunitParameter params[], void* data) {
    radix_binary_trie_add_test();
    radix_binary_trie_get_longest_prefix_test();
    radix_binary_trie_delete_test();
    radix_binary_trie_destroy_test();
    return MUNIT_OK;
}

static MunitResult test_prefix_tree(const MunitParameter params[], void* data) {
    prefix_tree_test();
    prefix_tree_complex_test();
    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    {
        (char *) "/ip-registry/binary_string",
        test_binary_string,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char *) "/ip-registry/singly_list",
        test_singly_list,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char *) "/ip-registry/radix_binary_trie",
        test_radix_binary_trie,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        (char *) "/ip-registry/ipv4_subnet_registry",
        test_prefix_tree,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL
    },
    {
        NULL,
        NULL,
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