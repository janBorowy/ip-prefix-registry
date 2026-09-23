#ifndef RADIX_UINT32_TRIE
#define RADIX_UINT32_TRIE

#include <stdint.h>
#include <stdbool.h>

#define MAX_EDGE_COUNT 100

struct Edge {
    uint32_t value;
    uint8_t length;
    struct Node *target;
};

struct Node {
    struct Edge edges[MAX_EDGE_COUNT]; // TODO: linked list here
    int num_edges;
    bool is_terminal;
};

int add(struct Node *root, uint32_t value, uint8_t length);
int del(struct Node *root, uint32_t base, uint8_t mask);
uint8_t check(struct Node *root, uint32_t val);

#endif