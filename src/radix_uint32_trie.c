#include "radix_uint32_trie.h"

#include <stdlib.h>
#include <assert.h>

#include "uint32_util.h"

// uint32_t value = ((UINT32_MAX << (32 - mask)) & base) >> (32 - mask);
int add(struct Node *node, uint32_t value, uint8_t length) {
    int matched_elements_num = 0;
    uint32_t value_left = value;
    uint8_t len_left = length - matched_elements_num;

    while (node != NULL && matched_elements_num < length) {

        struct Edge *next_edge = NULL;
        len_left = length - matched_elements_num;
        value_left = get_suffix(value, len_left);
        // TODO: change this when linked list implemented
        for (int i = 0; i < node->num_edges; i++) {
            if (is_prefix_of(node->edges[i].value, node->edges[i].length, value_left, len_left)) {
                next_edge = &node->edges[i];
            }
        }
        if (next_edge) {
            matched_elements_num += next_edge->length;
            node = next_edge->target;
        } else {
            break;
        }
    }

    struct Edge *sharing_edge = NULL;
    uint8_t common_prefix_len = 0;
    for (int i = 0; i < node->num_edges; i++) {
        common_prefix_len = get_common_prefix_length(value_left, len_left, node->edges[i].value, node->edges[i].length);
        if (common_prefix_len != 0) {
            sharing_edge = &node->edges[i];
            break;
        }
    }

    if (sharing_edge != NULL) {
        // split
    } else {
        // new edge
    }
}


