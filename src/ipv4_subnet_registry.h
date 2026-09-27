#ifndef IPV4_SUBNET_REGISTRY_H
#define IPV4_SUBNET_REGISTRY_H

#include <stdint.h>

#include "radix_binary_trie.h"

struct Ipv4SubnetRegistry {
  struct Node *root;
};

struct Ipv4SubnetRegistry *ipv4_subnet_registry_init();

/*
    Adds given subnet to the registry. Adding existing subnet does not make any
   effect. Returns -1 for illegal arguments.
*/
int ipv4_subnet_registry_add(struct Ipv4SubnetRegistry *registry, uint32_t base,
                             uint8_t mask);

/*
    Deletes given subnet from the registry. Deleting non existing subnet does
   not make any effect. Returns -1 for illegal arguments.
*/
int ipv4_subnet_registry_del(struct Ipv4SubnetRegistry *registry, uint32_t base,
                             uint8_t mask);

/*
    Checks if ip exists in registry. Returns the most specfic subnet mask length
   if subnet is found. If subnet is not found, returns -1. If registry is NULL,
   returns -2.
*/
char ipv4_subnet_registry_check(struct Ipv4SubnetRegistry *registry,
                                uint32_t ip);
void ipv4_subnet_registry_destory(struct Ipv4SubnetRegistry *registry);

#endif