#include "radix_binary_trie.h"

#include <stdlib.h>
#include <assert.h>

struct Edge *step(struct Node *root, struct BinaryString *value, struct BinaryString *value_left, int *matched_elements_num);
void add_edge_and_node(struct Node *node, struct BinaryString str, struct SinglyList *edges_list, bool is_terminal);
void delete_node(struct Edge *edge_to_deletee, struct Node *root, struct Edge *edge_to_parent);
void collapse_node(struct Edge *edge_to_node);

struct Node *radix_binary_trie_init() {
    struct Node *node = calloc(1, sizeof(struct Node));
    node->is_terminal = false;
    return node;
}

int radix_binary_trie_add(struct Node *node, struct BinaryString value) {
    int matched_elements_num = 0;
    struct BinaryString str_left;

    struct Edge *next_edge = step(node, &value, &str_left, &matched_elements_num);
    while (next_edge) {
        node = next_edge->target;
        next_edge = step(node, &value, &str_left, &matched_elements_num);
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

        common_prefix_len = get_common_prefix_length(str_left, edge->str);
        if (common_prefix_len != 0) {
            sharing_edge = edge;
            break;
        }

        edge_list = edge_list->next;
    }

    if (sharing_edge != NULL) {
        struct SinglyList *edges_to_move;
        struct BinaryString new_edge_str = get_suffix(str_left, str_left.length - common_prefix_len);
        struct BinaryString temp_edge_str = get_suffix(sharing_edge->str, sharing_edge->str.length - common_prefix_len);
        bool target_is_terminal = sharing_edge->target->is_terminal;

        sharing_edge->str = get_prefix(sharing_edge->str, common_prefix_len);

        if (new_edge_str.length == 0) {
            sharing_edge->target->is_terminal = true;
            edges_to_move = sharing_edge->target->edges;
            sharing_edge->target->edges = NULL;
            add_edge_and_node(sharing_edge->target, temp_edge_str, edges_to_move, target_is_terminal);
        } else {
            sharing_edge->target->is_terminal = false;
            edges_to_move = sharing_edge->target->edges;
            sharing_edge->target->edges = NULL;
            add_edge_and_node(sharing_edge->target, new_edge_str, NULL, true);
            add_edge_and_node(sharing_edge->target, temp_edge_str, edges_to_move, target_is_terminal);
        }
    } else {
        add_edge_and_node(node, str_left, NULL, true);
    }

    return 0;
}

int8_t radix_binary_trie_get_longest_prefix(struct Node *node, uint32_t value) {
    uint8_t longest_prefix_length = -1;
    int matched_elements_num = 0;
    struct BinaryString value_left;
    struct BinaryString bits_str = {
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

int radix_binary_trie_delete(struct Node *node, struct BinaryString bit_str) {
    int matched_elements_num = 0;
    struct BinaryString value_left;
    struct Edge *edge_to_parent = NULL;
    struct Edge *edge_to_deletee = NULL;
    struct Node *root = node;

    if (bit_str.length == 0) {
        node->is_terminal = false;
        return 0;
    }

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

void radix_binary_trie_destroy(struct Node *root) {
    if (root == NULL) {
        return;
    }

    struct SinglyList *edge_node = root->edges;
    while (edge_node) {
        radix_binary_trie_destroy(((struct Edge *)edge_node->data)->target);
        edge_node = edge_node->next;
    }
    singly_list_destroy(root->edges);
    free(root);

    return;
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
        edge_to_node->str = append_bits(edge_to_node->str, edge_to_collapse->str);
        edge_to_node->target = edge_to_collapse->target;

        singly_list_destroy(node_to_delete->edges);
        free(node_to_delete);
    } else {
        edge_to_node->target = NULL;
        free(node_to_delete);
    }
}

struct Edge *step(struct Node *node,
                      struct BinaryString *value,
                      struct BinaryString *value_left,
                      int *matched_elements_num) {
    struct SinglyList *edge_list;
    struct Edge *edge;

    if (node != NULL && *matched_elements_num < value->length) {
        struct Edge *next_edge = NULL;
        *value_left = get_suffix(*value, value->length - *matched_elements_num);

        edge_list = node->edges;
        while (edge_list) {
            edge = (struct Edge *)edge_list->data;
            if (is_prefix_of(edge->str, *value_left)) {
                next_edge = edge;
                break;
            }
            edge_list = edge_list->next;
        }

        if (next_edge) {
            *matched_elements_num += next_edge->str.length;
            return next_edge;
        }
    }
    return NULL;
}

void add_edge_and_node(
    struct Node *node,
    struct BinaryString str,
    struct SinglyList *edges_list,
    bool is_terminal
) {
    struct Edge *edge = calloc(1, sizeof(struct Edge));
    edge->str = str;
    edge->target = radix_binary_trie_init();
    edge->target->is_terminal = is_terminal;
    edge->target->edges = edges_list;

    node->edges = singly_list_prepend(node->edges, edge, sizeof(struct Edge));
}
