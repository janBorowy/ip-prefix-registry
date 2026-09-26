#include "radix_uint32_trie.h"

#include <stdlib.h>
#include <assert.h>

#include "uint32_util.h"

struct Edge *step(struct Node *root, struct BinaryValue *value, struct BinaryValue *value_left, int *matched_elements_num);
void add_edge_and_node(struct Node *node, uint32_t bits, uint8_t len, struct SinglyList *edges_list);
void delete_node(struct Edge *edge_to_deletee, struct Node *root, struct Edge *edge_to_parent);
void collapse_node(struct Edge *edge_to_node);

struct Node *radix_uint32_trie_init() {
    struct Node *node = calloc(1, sizeof(struct Node));
    node->is_terminal = false;
    return node;
}

// uint32_t value = ((UINT32_MAX << (32 - mask)) & base) >> (32 - mask);
int radix_uint32_trie_add(struct Node *node, struct BinaryValue value) {
    int matched_elements_num = 0;
    struct BinaryValue value_left;

    struct Edge *next_edge = step(node, &value, &value_left, &matched_elements_num);
    while (next_edge) {
        node = next_edge->target;
        next_edge = step(node, &value, &value_left, &matched_elements_num);
    }
    
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

    if (node->is_terminal) {
        longest_prefix_length = 0;
    }

    struct Edge *next_edge = step(node, &bits_str, &value_left, &matched_elements_num);
    while (next_edge) {
        node = next_edge->target;
        if (node->is_terminal) {
            longest_prefix_length = matched_elements_num;
        }
        next_edge = step(node, &bits_str, &value_left, &matched_elements_num);
    };

    while (next_edge) {
        if (node->is_terminal) {
            longest_prefix_length = matched_elements_num;
        }
        next_edge = step(node, &bits_str, &value_left, &matched_elements_num);
    };
    
    return longest_prefix_length;
}

int radix_uint32_trie_delete(struct Node *node, struct BinaryValue bit_str) {
    int matched_elements_num = 0;
    struct BinaryValue value_left;
    struct Edge *edge_to_parent = NULL;
    struct Edge *edge_to_deletee = NULL;
    struct Node *root = node;

    struct Edge *next_edge = step(node, &bit_str, &value_left, &matched_elements_num);
    while (next_edge) {
        node = next_edge->target;
        if (next_edge) {
            edge_to_parent = edge_to_deletee;
            edge_to_deletee = next_edge;
        }
        next_edge = step(node, &bit_str, &value_left, &matched_elements_num);
    };

    if (bit_str.length == matched_elements_num && node->is_terminal) {
        delete_node(edge_to_deletee, root, edge_to_parent);
    }
    return 0;
}

void delete_node(struct Edge *edge_to_deletee, struct Node *root, struct Edge *edge_to_parent) {
    struct Node *deletee = edge_to_deletee->target;
    struct Node *parent;
    if (edge_to_parent == NULL) {
        collapse_node(edge_to_deletee);
        if (edge_to_deletee->target == NULL) {
            root->edges = singly_list_delete_by_data(root->edges, (void *)edge_to_deletee);
        }
    } else if (deletee->edges == NULL) {
        parent = edge_to_parent->target;
        free(deletee);
        parent->edges = singly_list_delete_by_data(parent->edges, (void *)edge_to_deletee);
        if (!edge_to_parent->target->is_terminal)  {
            collapse_node(edge_to_parent);
        }
    } else if (deletee->edges->next != NULL) {
        deletee->is_terminal = false;
    } else {
        collapse_node(edge_to_deletee);
    }
}

void collapse_node(struct Edge *edge_to_node) {
    struct Node *node_to_delete = edge_to_node->target;
    if (node_to_delete->edges) {
        struct Edge *edge_to_collapse = edge_to_node->target->edges->data;
        edge_to_node->value.bits = append_bits(edge_to_node->value.bits, edge_to_node->value.length,
                                        edge_to_collapse->value.bits, edge_to_collapse->value.length);
        edge_to_node->value.length = edge_to_node->value.length + edge_to_collapse->value.length;
        edge_to_node->target = edge_to_collapse->target;

        singly_list_destroy(node_to_delete->edges);
        free(node_to_delete);
    } else {
        edge_to_node->target = NULL;
        free(node_to_delete);
    }
}

struct Edge *step(struct Node *node,
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
            return next_edge;
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

void radix_uint32_trie_destroy(struct Node *root) {
    return;
}
