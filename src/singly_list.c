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

struct SinglyList *singly_list_delete_by_data(struct SinglyList *root, void *data_to_delete) {
    if (root == NULL || data_to_delete == NULL) {
        return NULL;
    }

    struct SinglyList *node = root->next;
    struct SinglyList *prev = root;
    if (prev->data == data_to_delete) {
        free(prev);
        return node;
    }

    while (node) {
        if (node->data == data_to_delete) {
            prev->next = node->next;
            free(node->data);
            free(node);
            break;
        }
        prev = node;
        node = node->next;
    }
    return root;
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
