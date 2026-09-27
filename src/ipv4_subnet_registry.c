#include "ipv4_subnet_registry.h"

#include <stdlib.h>

struct BinaryString get_mask_binary_str(uint32_t base, uint8_t mask);

struct Ipv4SubnetRegistry *ipv4_subnet_registry_init() {
    struct Ipv4SubnetRegistry *reg = calloc(1, sizeof(struct Ipv4SubnetRegistry));
    reg->root = radix_binary_trie_init();
    return reg;
}

int ipv4_subnet_registry_add(struct Ipv4SubnetRegistry *reg, uint32_t base, uint8_t mask) {
    if (reg == NULL || mask > 32) {
        return -1;
    }
    radix_binary_trie_add(reg->root, get_mask_binary_str(base, mask));
    return 0;
}

int ipv4_subnet_registry_del(struct Ipv4SubnetRegistry *reg, uint32_t base, uint8_t mask) {
    if (reg == NULL || mask > 32) {
        return -1;
    }
    radix_binary_trie_delete(reg->root, get_mask_binary_str(base, mask));
    return 0;
}

char ipv4_subnet_registry_check(struct Ipv4SubnetRegistry *reg, uint32_t ip) {
    if (reg == NULL) {
        return -2;
    }
    return radix_binary_trie_get_longest_prefix(reg->root, ip);
}

void ipv4_subnet_registry_destory(struct Ipv4SubnetRegistry *reg) {
    radix_binary_trie_destroy(reg->root);
    free(reg);
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