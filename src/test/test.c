#include "binary_string_tests.h"
#include "singly_list_tests.h"
#include "radix_binary_trie_tests.h"
#include "prefix_tree_tests.h"

#include <stdio.h>

void test_binary_string() {
    is_prefix_of_test();
    get_prefix_test();
    get_suffix_test();
    get_common_prefix_length_test();
    append_bits_test();
}

void test_singly_list() {
    singly_list_test();
    singly_list_delete_test();
}

void test_radix_binary_trie() {
    radix_binary_trie_add_test();
    radix_binary_trie_get_longest_prefix_test();
    radix_binary_trie_delete_test();
    radix_binary_trie_destroy_test();
}

void test_prefix_tree() {
    prefix_tree_test();
    prefix_tree_complex_test();
}

void run_test(void (*test_f)(), const char *label) {
    printf("----- %s: ", label);
    (*test_f)();
    printf("PASSED -----\n");
}

int main(int argc, char *argv[]) {
    run_test(test_binary_string, "binary_string");
    run_test(test_singly_list, "singly_list");
    run_test(test_radix_binary_trie, "radix_binary_trie");
    run_test(test_prefix_tree, "prefix_tree");
    printf("All tests passed!\n");
}