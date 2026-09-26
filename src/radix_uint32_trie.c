#include "radix_uint32_trie.h"

#include <stdlib.h>
#include <assert.h>

#include "uint32_util.h"

struct Node *step(struct Node *root, struct BinaryValue *value, struct BinaryValue *value_left, int *matched_elements_num);
void add_edge_and_node(struct Node *node, uint32_t bits, uint8_t len, struct SinglyList *edges_list);

struct Node *radix_uint32_trie_init() {
    struct Node *node = calloc(1, sizeof(struct Node));
    node->is_terminal = false;
    return node;
}

// uint32_t value = ((UINT32_MAX << (32 - mask)) & base) >> (32 - mask);
int radix_uint32_trie_add(struct Node *node, struct BinaryValue value) {
    int matched_elements_num = 0;
    struct BinaryValue value_left;

    struct Node *next_node = node;
    do {
        node = next_node;
        next_node = step(node, &value, &value_left, &matched_elements_num);
    } while (next_node);

    if (matched_elements_num == value.length) {
        node->is_terminal = true;
        return 0;
    }

    struct Edge *sharing_edge = NULL;
    struct Edge *edge;
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
        struct SinglyList *edges_to_move;
        uint8_t new_edge_len = value_left.length - common_prefix_len;
        uint32_t new_edge_val = get_suffix(value_left.bits, new_edge_len);
        uint32_t temp_edge_len = sharing_edge->value.length - common_prefix_len;
        uint32_t temp_edge_val = get_suffix(sharing_edge->value.bits, temp_edge_len);

        sharing_edge->value.bits = get_prefix(sharing_edge->value.bits, sharing_edge->value.length, common_prefix_len);
        sharing_edge->value.length = common_prefix_len;

        if (new_edge_len == 0) {
            sharing_edge->target->is_terminal = true;
            edges_to_move = sharing_edge->target->edges;
            sharing_edge->target->edges = NULL;
            add_edge_and_node(sharing_edge->target, temp_edge_val, temp_edge_len, edges_to_move);
        } else {
            sharing_edge->target->is_terminal = false;
            edges_to_move = sharing_edge->target->edges;
            sharing_edge->target->edges = NULL;
            add_edge_and_node(sharing_edge->target, new_edge_val, new_edge_len, NULL);
            add_edge_and_node(sharing_edge->target, temp_edge_val, temp_edge_len, edges_to_move);
        }
    } else {
        add_edge_and_node(node, value_left.bits, value_left.length, NULL);
    }

    return 0;
}

int8_t radix_uint32_get_longest_prefix(struct Node *node, uint32_t value) {
    uint8_t longest_prefix_length = -1;
    int matched_elements_num = 0;
    struct BinaryValue value_left;
    struct BinaryValue bits_str = {
        .bits = value,
        .length = 32
    };

    struct Node *next_node = node;
    do {
        node = next_node;
        if (node->is_terminal) {
            longest_prefix_length = matched_elements_num;
        }
        next_node = step(node, &bits_str, &value_left, &matched_elements_num);
    } while (next_node);
    
    return longest_prefix_length;
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

void add_edge_and_node(struct Node *node, uint32_t bits, uint8_t len, struct SinglyList *edges_list) {
    struct Edge *edge = calloc(1, sizeof(struct Edge));
    edge->value.bits = bits;
    edge->value.length = len;
    edge->target = radix_uint32_trie_init();
    edge->target->is_terminal = true;
    edge->target->edges = edges_list;

    node->edges = singly_list_prepend(node->edges, edge, sizeof(struct Edge));
}


