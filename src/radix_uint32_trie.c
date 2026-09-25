#include "radix_uint32_trie.h"

#include <stdlib.h>
#include <assert.h>

#include "uint32_util.h"

struct Node *step(struct Node *root, struct BinaryValue *value, struct BinaryValue *value_left, int *matched_elements_num);
void add_edge_and_leaf(struct Node *node, uint32_t bits, uint8_t len);

struct Node *radix_uint32_trie_init() {
    struct Node *node = calloc(1, sizeof(struct Node));
    node->is_leaf = false;
    return node;
}

// uint32_t value = ((UINT32_MAX << (32 - mask)) & base) >> (32 - mask);
int radix_uint32_trie_add(struct Node *node, struct BinaryValue value) {
    int matched_elements_num = 0;
    struct BinaryValue value_left;
    struct Edge *edge;

    struct Node *next_node = node;
    do {
        node = next_node;
        next_node = step(node, &value, &value_left, &matched_elements_num);
    } while (next_node);

    if (matched_elements_num == value.length) {
        node->is_leaf = true;
        return 0;
    }

    struct Edge *sharing_edge = NULL;
    struct SinglyList *edge_list;
    uint8_t common_prefix_len = 0;
    edge_list = node->edges;
    while (edge_list) {
        edge = (struct Edge *) edge_list->data;

        common_prefix_len = get_common_prefix_length(value_left.bits, value_left.length, edge->value.bits, edge->value.length);
        if (common_prefix_len != 0) {
            sharing_edge = edge;
            break;
        }

        edge_list = edge_list->next;
    }

    if (sharing_edge != NULL) {
        uint8_t new_edge_len = value_left.length - common_prefix_len;
        uint32_t new_edge_val = get_suffix(value_left.bits, new_edge_len);
        uint32_t temp_edge_len = sharing_edge->value.length - common_prefix_len;
        uint32_t temp_edge_val = get_suffix(sharing_edge->value.bits, temp_edge_len);

        sharing_edge->value.bits = get_prefix(sharing_edge->value.bits, sharing_edge->value.length, common_prefix_len);
        sharing_edge->value.length = common_prefix_len;

        if (new_edge_len == 0) {
            sharing_edge->target->is_leaf = true;
            add_edge_and_leaf(sharing_edge->target, temp_edge_val, temp_edge_len);
        } else {
            sharing_edge->target->is_leaf = false;
            add_edge_and_leaf(sharing_edge->target, new_edge_val, new_edge_len);
            add_edge_and_leaf(sharing_edge->target, temp_edge_val, temp_edge_len);
        }
    } else {
        add_edge_and_leaf(node, value_left.bits, value_left.length);
    }

    return 0;
}

uint8_t radix_int32_get_longest_prefix(struct Node *root, uint32_t val) {

}

struct Node *step(struct Node *node,
                      struct BinaryValue *value,
                      struct BinaryValue *value_left,
                      int *matched_elements_num) {
    struct SinglyList *edge_list;
    struct Edge *edge;

    if (node != NULL && *matched_elements_num < value->length) {
        struct Edge *next_edge = NULL;
        value_left->length = value->length - *matched_elements_num;
        value_left->bits = get_suffix(value->bits, value_left->length);

        edge_list = node->edges;
        while (edge_list) {
            edge = (struct Edge *)edge_list->data;
            if (is_prefix_of(edge->value.bits, edge->value.length, value_left->bits, value_left->length)) {
                next_edge = edge;
                break;
            }
            edge_list = edge_list->next;
        }

        if (next_edge) {
            *matched_elements_num += next_edge->value.length;
            node = next_edge->target;
            return node;
        }
    }
    return NULL;
}

void add_edge_and_leaf(struct Node *node, uint32_t bits, uint8_t len) {
    struct Edge *edge = calloc(1, sizeof(struct Edge));
    edge->value.bits = bits;
    edge->value.length = len;
    edge->target = radix_uint32_trie_init();
    edge->target->is_leaf = true;

    node->edges = singly_list_prepend(node->edges, edge, sizeof(struct Edge));
}


