#ifndef RADIX_UINT32_TRIE
#define RADIX_UINT32_TRIE

#include <stdint.h>
#include <stdbool.h>
#include "singly_list.h"

#define MAX_EDGE_COUNT 100

struct BinaryValue {
    uint32_t bits;
    uint8_t length;
};

struct Edge {
    struct BinaryValue value;
    struct Node *target;
};

struct Node {
    struct SinglyList *edges;
    bool is_leaf;
};

struct Node *radix_uint32_trie_init();
int radix_uint32_trie_add(struct Node *root, struct BinaryValue value);
int radix_uint32_trie_delete(struct Node *root, struct BinaryValue value);
uint8_t raidx_int32_get_longest_prefix(struct Node *root, uint32_t val);
void radix_uint32_trie_destroy(struct Node *root);

#endif