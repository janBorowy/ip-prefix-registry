#include "radix_uint32_trie_tests.h"
#include "../radix_uint32_trie.h"
#include "../../lib/munit/munit.h"

void assert_edge_val(struct Edge *edge, uint32_t value, uint8_t length);

void radix_uint32_trie_add_test() {
    struct Node *root = radix_uint32_trie_init();
    radix_uint32_trie_add(root, (struct BinaryValue) {
        .bits = 0b100,
        .length = 3
    });

    radix_uint32_trie_add(root, (struct BinaryValue) {
        .bits = 0b001,
        .length = 3
    });

    radix_uint32_trie_add(root, (struct BinaryValue) {
        .bits = 0b011,
        .length = 3
    });
    radix_uint32_trie_add(root, (struct BinaryValue) {
        .bits = 0b011,
        .length = 3
    });

    radix_uint32_trie_add(root, (struct BinaryValue) {
        .bits = 0b000,
        .length = 3
    });

    radix_uint32_trie_add(root, (struct BinaryValue) {
        .bits = 0b1101,
        .length = 4
    });

    radix_uint32_trie_add(root, (struct BinaryValue) {
        .bits = 0,
        .length = 1
    });

    radix_uint32_trie_add(root, (struct BinaryValue) {
        .bits = 0,
        .length = 2
    });

    munit_assert_false(root->is_leaf);

    struct Edge *left = root->edges->data;
    struct Edge *right = root->edges->next->data;
    assert_edge_val(left, 0b0, 1);
    assert_edge_val(right, 0b1, 1);

    struct Edge *left_left = left->target->edges->data;
    struct Edge *left_right = left->target->edges->next->data;

    assert_edge_val(left_left, 0b0, 1);
    assert_edge_val(left_right, 0b11, 2);
    munit_assert_null(left_right->target->edges);

    struct Edge *left_left_left = left_left->target->edges->data;
    struct Edge *left_left_right = left_left->target->edges->next->data;

    assert_edge_val(left_left_left, 0b1, 1);
    assert_edge_val(left_left_right, 0b0, 1);
    munit_assert_null(left_left_left->target->edges);
    munit_assert_null(left_left_right->target->edges);

    struct Edge *right_left = right->target->edges->data;
    struct Edge *right_right = right->target->edges->next->data;

    assert_edge_val(right_left, 0b00, 2);
    assert_edge_val(right_right, 0b101, 3);
    munit_assert_null(right_left->target->edges);
    munit_assert_null(right_right->target->edges);
}

void radix_uint32_trie_check_test() {
}

void assert_edge_val(struct Edge *edge, uint32_t value, uint8_t length) {
    munit_assert_uint32(edge->value.bits, ==, value);
    munit_assert_uint32(edge->value.length, ==, length);
}
