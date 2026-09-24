#include "radix_uint32_trie.h"

#include <stdlib.h>
#include <assert.h>

#include "uint32_util.h"

void add_edge_and_leaf(struct Node *node, uint32_t bits, uint8_t len);

struct Node *radix_uint32_trie_init() {
    struct Node *node = calloc(1, sizeof(struct Node));
    node->is_leaf = false;
    return node;
}

// uint32_t value = ((UINT32_MAX << (32 - mask)) & base) >> (32 - mask);
int radix_uint32_trie_add(struct Node *node, struct BinaryValue value) {
    int matched_elements_num = 0;
    uint32_t value_left;
    uint8_t len_left;
    struct SinglyList *edge_node;
    struct Edge *edge;

    while (node != NULL && matched_elements_num < value.length) {

        struct Edge *next_edge = NULL;
        len_left = value.length - matched_elements_num;
        value_left = get_suffix(value.bits, len_left);

        edge_node = node->edges;
        while (edge_node) {
            edge = (struct Edge *)edge_node->data;
            if (is_prefix_of(edge->value.bits, edge->value.length, value_left, len_left)) {
                next_edge = edge;
                break;
            }
            edge_node = edge_node->next;
        }

        if (next_edge) {
            matched_elements_num += next_edge->value.length;
            node = next_edge->target;
        } else {
            break;
        }
    }

    struct Edge *sharing_edge = NULL;
    uint8_t common_prefix_len = 0;
    edge_node = node->edges;
    while (edge_node) {
        edge = (struct Edge *) edge_node->data;

        common_prefix_len = get_common_prefix_length(value_left, len_left, edge->value.bits, edge->value.length);
        if (common_prefix_len != 0) {
            sharing_edge = edge;
            break;
        }

        edge_node = edge_node->next;
    }

    if (sharing_edge != NULL) {
        uint8_t new_edge_len = len_left - common_prefix_len;
        uint32_t new_edge_val = get_suffix(value_left, new_edge_len);
        uint32_t temp_edge_len = sharing_edge->value.length - common_prefix_len;
        uint32_t temp_edge_val = get_suffix(sharing_edge->value.bits, temp_edge_len);

        sharing_edge->value.length = sharing_edge->value.length - common_prefix_len;
        sharing_edge->value.bits = get_prefix(sharing_edge->value.bits, sharing_edge->value.length, sharing_edge->value.length - common_prefix_len);

        if (new_edge_len == 0) {
            sharing_edge->target->is_leaf = true;
            add_edge_and_leaf(sharing_edge->target, temp_edge_val, temp_edge_len);
        } else {
            add_edge_and_leaf(sharing_edge->target, new_edge_val, new_edge_len);
            add_edge_and_leaf(sharing_edge->target, temp_edge_val, temp_edge_len);
        }
    } else {
        add_edge_and_leaf(node, value_left, len_left);
    }
}

void add_edge_and_leaf(struct Node *node, uint32_t bits, uint8_t len) {
    struct Edge *edge = calloc(1, sizeof(struct Edge));
    edge->value.bits = bits;
    edge->value.length = len;
    edge->target = radix_uint32_trie_init();
    edge->target->is_leaf = true;

    node->edges = singly_list_prepend(node->edges, edge, sizeof(struct Edge));
}


