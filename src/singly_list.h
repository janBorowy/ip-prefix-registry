#ifndef SINGLY_LIST_H
#define SINGLY_LIST_H

#include <stdio.h>

struct SinglyList {
	void *data;
	struct SinglyList *next;
};

struct SinglyList *singly_list_create(void *data, size_t data_size);

// returns new root
struct SinglyList *singly_list_prepend(struct SinglyList *node, void *data, size_t data_size);

// returns new root
struct SinglyList *singly_list_delete_by_data(struct SinglyList *node, void *node_to_delete);

void singly_list_destroy(struct SinglyList *root);

#endif