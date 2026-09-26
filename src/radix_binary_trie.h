#ifndef RADIX_BINARY_TRIE
#define RADIX_BINARY_TRIE

#include <stdint.h>
#include <stdbool.h>
#include "singly_list.h"
#include "binary_string.h"

#define MAX_EDGE_COUNT 100

struct Edge {
    struct BinaryString str;
    struct Node *target;
};

struct Node {
    struct SinglyList *edges;
    bool is_terminal;
};

struct Node *radix_binary_trie_init();
int radix_binary_trie_add(struct Node *root, struct BinaryString value);
int8_t radix_binary_trie_get_longest_prefix(struct Node *root, uint32_t val);
int radix_binary_trie_delete(struct Node *root, struct BinaryString value);
void radix_binary_trie_destroy(struct Node *root);

#endif