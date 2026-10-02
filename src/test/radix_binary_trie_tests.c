#include "radix_binary_trie_tests.h"
#include "../radix_binary_trie.h"
#include "../../lib/munit/munit.h"

void assert_edge_val(struct Edge *edge, uint32_t value, uint8_t length);
struct Node *create_test_trie();

void radix_binary_trie_add_test() {
    // General test
    struct Node *root = radix_binary_trie_init();
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b100,
        .length = 3
    });

    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b001,
        .length = 3
    });

    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b011,
        .length = 3
    });
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b011,
        .length = 3
    });

    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b000,
        .length = 3
    });

    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b1101,
        .length = 4
    });

    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0,
        .length = 1
    });

    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0,
        .length = 2
    });

    munit_assert_false(root->is_terminal);

    struct Edge *left = root->edges->data;
    struct Edge *right = root->edges->next->data;
    assert_edge_val(left, 0b0, 1);
    assert_edge_val(right, 0b1, 1);

    left = root->edges->data;
    right = root->edges->next->data;
    struct Edge *left_left = left->target->edges->data;
    struct Edge *left_right = left->target->edges->next->data;

    assert_edge_val(left_left, 0b0, 1);
    assert_edge_val(left_right, 0b11, 2);
    munit_assert_null(left_right->target->edges);

    left = root->edges->data;
    right = root->edges->next->data;
    left_left = left->target->edges->data;
    left_right = left->target->edges->next->data;
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

    radix_binary_trie_destroy(root);
    // Losing edges test - internal terminal
    root = radix_binary_trie_init();

    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b1010,
        .length = 4
    });

    left = root->edges->data;
    assert_edge_val(left, 0b1010, 4);

    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b10,
        .length = 2
    });

    left = root->edges->data;
    left_left = left->target->edges->data;
    assert_edge_val(left, 0b10, 2);
    assert_edge_val(left_left, 0b10, 2);

    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b1,
        .length = 1
    });

    left = root->edges->data;
    left_left = left->target->edges->data;
    left_left_left = left_left->target->edges->data;
    assert_edge_val(left, 0b1, 1);
    assert_edge_val(left_left, 0b0, 1);
    assert_edge_val(left_left_left, 0b10, 2);
    munit_assert_true(left->target->is_terminal);
    munit_assert_true(left_left->target->is_terminal);
    munit_assert_true(left_left_left->target->is_terminal);

    radix_binary_trie_destroy(root);
    // Losing edges test - new terminal node
    root = radix_binary_trie_init();

    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b1010,
        .length = 4
    });

    left = root->edges->data;
    assert_edge_val(left, 0b1010, 4);

    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b10,
        .length = 2
    });

    left = root->edges->data;
    left_left = left->target->edges->data;
    assert_edge_val(left, 0b10, 2);
    assert_edge_val(left_left, 0b10, 2);

    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b11,
        .length = 2
    });

    left = root->edges->data;
    left_left = left->target->edges->data;
    left_right = left->target->edges->next->data;
    left_left_left = left_left->target->edges->data;
    assert_edge_val(left, 0b1, 1);
    assert_edge_val(left_left, 0b0, 1);
    assert_edge_val(left_right, 0b1, 1);
    assert_edge_val(left_left_left, 0b10, 2);
    munit_assert_false(left->target->is_terminal);
    munit_assert_true(left_left->target->is_terminal);
    munit_assert_true(left_right->target->is_terminal);
    munit_assert_true(left_left_left->target->is_terminal);

    radix_binary_trie_destroy(root);
}

void radix_binary_trie_get_longest_prefix_test() {
    struct Node *node = radix_binary_trie_init();

    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0b0), == , -1);

    radix_binary_trie_add(node, (struct BinaryString){
        .bits = 0b1010,
        .length = 4
    });
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0xA0000000), == , 4);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0x90000000), == , -1);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0x20000000), == , -1);

    radix_binary_trie_add(node, (struct BinaryString){
        .bits = 0b10,
        .length = 2
    });
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0xA0000000), == , 4);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0x90000000), == , 2);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0x80000000), == , 2);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0x20000000), == , -1);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0xC0000000), == , -1);

    radix_binary_trie_add(node, (struct BinaryString){
        .bits = 0b11,
        .length = 2
    });
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0xA0000000), == , 4);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0x90000000), == , 2);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0x80000000), == , 2);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0xC0000000), ==, 2);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0xE0000000), ==, 2);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0x20000000), == , -1);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0x00000000), == , -1);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0x00000001), == , -1);

    radix_binary_trie_add(node, (struct BinaryString){
        .bits = 0,
        .length = 0
    });
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0b0), == , 0);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0xA0000000), == , 4);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0x90000000), == , 2);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0x80000000), == , 2);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0xC0000000), ==, 2);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0xE0000000), ==, 2);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0x20000000), == , 0);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0x00000000), == , 0);
    munit_assert_int8(radix_binary_trie_get_longest_prefix(node, 0x00000001), == , 0);

    radix_binary_trie_destroy(node);
}

void radix_binary_trie_delete_test() {
    struct Node *root;
    struct Edge *right, *right_right, *right_left, *left, *left_left, *left_left_left, *left_right, *left_left_right;
    // Delete leaf and collapse
    root = create_test_trie();
    radix_binary_trie_delete(root, (struct BinaryString) {
        .bits = 0b001,
        .length = 3
    });

    right = root->edges->next->data;
    assert_edge_val(right, 0b011, 3);
    munit_assert_true(right->target->is_terminal);
    munit_assert_null(right->target->edges);

    radix_binary_trie_destroy(root);

    // Delete not exsiting node should not change trie
    root = create_test_trie();
    radix_binary_trie_delete(root, (struct BinaryString) {
        .bits = 0b111,
        .length = 3
    });

    left = root->edges->data;
    left_left = left->target->edges->data;
    left_right = left->target->edges->next->data;
    left_left_left = left_left->target->edges->data;
    right = root->edges->next->data;
    right_left = right->target->edges->data;
    right_right = right->target->edges->next->data;
    assert_edge_val(left, 0b1, 1);
    munit_assert_false(left->target->is_terminal);
    assert_edge_val(left_left, 0b0, 1);
    munit_assert_true(left_left->target->is_terminal);
    assert_edge_val(left_right, 0b1, 1);
    munit_assert_true(left_right->target->is_terminal);
    assert_edge_val(left_left_left, 0b10, 2);
    munit_assert_true(left_left_left->target->is_terminal);
    assert_edge_val(right, 0b0, 1);
    munit_assert_false(right->target->is_terminal);
    assert_edge_val(right_left, 0b11, 2);
    munit_assert_true(right_left->target->is_terminal);
    assert_edge_val(right_right, 0b01, 2);
    munit_assert_true(right_right->target->is_terminal);

    radix_binary_trie_destroy(root);

    // Delete internal terminal node
    root = create_test_trie();
    // these children should be kept after deletion
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b10100,
        .length = 5
    });
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b10101,
        .length = 5
    });
    radix_binary_trie_delete(root, (struct BinaryString) {
        .bits = 0b10,
        .length = 2
    });

    left = root->edges->data;
    left_left = left->target->edges->data;
    left_right = left->target->edges->next->data;
    left_left_left = left_left->target->edges->data;
    left_left_right = left_left->target->edges->next->data;
    assert_edge_val(left, 0b1, 1);
    munit_assert_false(left->target->is_terminal);
    assert_edge_val(left_left, 0b010, 3);
    munit_assert_true(left_left->target->is_terminal);
    assert_edge_val(left_right, 0b1, 1);
    munit_assert_true(left_right->target->is_terminal);
    assert_edge_val(left_left_left, 0b1, 1);
    assert_edge_val(left_left_right, 0b0, 1);

    radix_binary_trie_destroy(root);

    // Delete leaf but parent is terminal, so don't collapse
    root = create_test_trie();
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b0,
        .length = 1
    });
    radix_binary_trie_delete(root, (struct BinaryString) {
        .bits = 0b001,
        .length = 3
    });

    right = root->edges->next->data;
    right_right = right->target->edges->data;
    assert_edge_val(right, 0b0, 1);
    munit_assert_true(right->target->is_terminal);
    assert_edge_val(right_right, 0b11, 2);
    munit_assert_true(right_right->target->is_terminal);
    munit_assert_null(right->target->edges->next);

    radix_binary_trie_destroy(root);

    // Delete internal node, whose parent of 2 other nodes
    root = create_test_trie();

    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b1001,
        .length = 4
    });
    radix_binary_trie_delete(root, (struct BinaryString) {
        .bits = 0b10,
        .length = 2
    });

    left = root->edges->data;
    left_left = left->target->edges->data;
    left_right = left->target->edges->next->data;
    left_left_left = left_left->target->edges->data;
    left_left_right = left_left->target->edges->next->data;
    assert_edge_val(left, 0b1, 1);
    munit_assert_false(left->target->is_terminal);
    assert_edge_val(left_left, 0b0, 1);
    munit_assert_false(left_left->target->is_terminal);
    assert_edge_val(left_right, 0b1, 1);
    munit_assert_true(left_right->target->is_terminal);
    assert_edge_val(left_left_left, 0b01, 2);
    munit_assert_true(left_left_left->target->is_terminal);
    assert_edge_val(left_left_right, 0b10, 2);
    munit_assert_true(left_left_right->target->is_terminal);

    radix_binary_trie_destroy(root);

    // Delete nodes directly connected to root
    root = radix_binary_trie_init();
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b000,
        .length = 3
    });
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b100,
        .length = 3
    });

    radix_binary_trie_delete(root, (struct BinaryString) {
        .bits = 0b000,
        .length = 3
    });

    left = root->edges->data;

    assert_edge_val(left, 0b100, 3);
    munit_assert_true(left->target->is_terminal);
    munit_assert_null(left->target->edges);
    munit_assert_null(root->edges->next);

    radix_binary_trie_destroy(root);

    // Delete nodes directly connected to root and collapse
    root = radix_binary_trie_init();
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b000,
        .length = 3
    });
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b100,
        .length = 3
    });
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b0001,
        .length = 4
    });
    
    radix_binary_trie_delete(root, (struct BinaryString) {
        .bits = 0b000,
        .length = 3
    });

    left = root->edges->data;
    right = root->edges->next->data;

    assert_edge_val(left, 0b100, 3);
    munit_assert_true(left->target->is_terminal);
    munit_assert_null(left->target->edges);

    assert_edge_val(right, 0b0001, 4);
    munit_assert_true(right->target->is_terminal);
    munit_assert_null(right->target->edges);

    radix_binary_trie_destroy(root);

    // Delete root
    root = create_test_trie();
    munit_assert_false(root->is_terminal);
    radix_binary_trie_add(root, EMPTY_BINARY_STRING);
    munit_assert_true(root->is_terminal);
    radix_binary_trie_delete(root, EMPTY_BINARY_STRING);
    munit_assert_false(root->is_terminal);

    left = root->edges->data;
    left_left = left->target->edges->data;
    left_right = left->target->edges->next->data;
    left_left_left = left_left->target->edges->data;
    right = root->edges->next->data;
    right_left = right->target->edges->data;
    right_right = right->target->edges->next->data;
    assert_edge_val(left, 0b1, 1);
    munit_assert_false(left->target->is_terminal);
    assert_edge_val(left_left, 0b0, 1);
    munit_assert_true(left_left->target->is_terminal);
    assert_edge_val(left_right, 0b1, 1);
    munit_assert_true(left_right->target->is_terminal);
    assert_edge_val(left_left_left, 0b10, 2);
    munit_assert_true(left_left_left->target->is_terminal);
    assert_edge_val(right, 0b0, 1);
    munit_assert_false(right->target->is_terminal);
    assert_edge_val(right_left, 0b11, 2);
    munit_assert_true(right_left->target->is_terminal);
    assert_edge_val(right_right, 0b01, 2);
    munit_assert_true(right_right->target->is_terminal);

    radix_binary_trie_destroy(root);

    // Delete node directly connected to root with sibling
    root = radix_binary_trie_init();
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b0,
        .length = 1
    });
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b01,
        .length = 2
    });
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b00,
        .length = 2
    });

    radix_binary_trie_delete(root, (struct BinaryString) {
        .bits = 0b0,
        .length = 1
    });

    left = root->edges->data;
    left_left = left->target->edges->data;
    left_right = left->target->edges->next->data;

    assert_edge_val(left, 0b0, 1);
    munit_assert_false(left->target->is_terminal);
    munit_assert_null(root->edges->next);
    assert_edge_val(left_left, 0b0, 1);
    munit_assert_true(left_left->target->is_terminal);
    assert_edge_val(left_right, 0b1, 1);
    munit_assert_true(left_right->target->is_terminal);

    radix_binary_trie_destroy(root);
}

void radix_binary_trie_destroy_test() {
    struct Node *root = create_test_trie();
    radix_binary_trie_destroy(root);
}

struct Node *create_test_trie() {
    struct Node *root = radix_binary_trie_init();
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b011,
        .length = 3
    });
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b001,
        .length = 3
    });
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b1010,
        .length = 4
    });
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b11,
        .length = 2
    });
    radix_binary_trie_add(root, (struct BinaryString) {
        .bits = 0b10,
        .length = 2
    });
    return root;
}

void assert_edge_val(struct Edge *edge, uint32_t value, uint8_t length) {
    munit_assert_uint32(edge->str.bits, ==, value);
    munit_assert_uint32(edge->str.length, ==, length);
}
