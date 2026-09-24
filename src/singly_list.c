#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "singly_list.h"

struct SinglyList *singly_list_create(void *data, size_t data_size) {
    struct SinglyList *head;
    head = calloc(1, sizeof(struct SinglyList));
    if (data != NULL) {
        head->data = calloc(1, data_size);
        memcpy(head->data, data, data_size);
    }
    return head;
}

struct SinglyList *singly_list_prepend(struct SinglyList *root, void *data,
                           size_t data_size) {
    struct SinglyList *to_insert, *temp;
    to_insert = singly_list_create(data, data_size);
    to_insert->next = root;
    return to_insert;
}

void singly_list_destroy(struct SinglyList *node) {
    struct SinglyList *next_node;

    while (node) {
        next_node = node->next;
        free(node->data);
        free(node);
        node = next_node;
    }
}
