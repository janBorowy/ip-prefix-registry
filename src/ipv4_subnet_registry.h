#ifndef IPV4_SUBNET_REGISTRY_H
#define IPV4_SUBNET_REGISTRY_H

#include <stdint.h>

#include "radix_binary_trie.h"

struct Ipv4SubnetRegistry {
    struct Node *root;
};

struct Ipv4SubnetRegistry *ipv4_subnet_registry_init();
int ipv4_subnet_registry_add(struct Ipv4SubnetRegistry *registry, uint32_t base, uint8_t mask);
int ipv4_subnet_registry_del(struct Ipv4SubnetRegistry *registry, uint32_t base, uint8_t mask);
char ipv4_subnet_registry_check(struct Ipv4SubnetRegistry *registry, uint32_t ip);
void ipv4_subnet_registry_destory(struct Ipv4SubnetRegistry *registry);

#endif