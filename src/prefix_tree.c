#include "prefix_tree.h"

#include "radix_binary_trie.h"

struct BinaryString get_mask_binary_str(uint32_t base, uint8_t mask);

struct Node trie = {0};

int add(unsigned int base, char mask) {
    if (mask > 32) {
        return -1;
    }
    radix_binary_trie_add(&trie, get_mask_binary_str(base, mask));
    return 0;
}

int del(unsigned int base, char mask) {
    if (mask > 32) {
        return -1;
    }
    radix_binary_trie_delete(&trie, get_mask_binary_str(base, mask));
    return 0;
}

char check(unsigned int ip) {
    return radix_binary_trie_get_longest_prefix(&trie, ip);
}

struct BinaryString get_mask_binary_str(uint32_t base, uint8_t mask) {
    if (mask == 0) {
        return EMPTY_BINARY_STRING;
    }
    return (struct BinaryString) {
        .bits = ((UINT32_MAX << (32 - mask)) & base) >> (32 - mask),
        .length = mask
    };
}