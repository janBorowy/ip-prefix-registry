#include "singly_list_tests.h"
#include "../singly_list.h"
#include "assertions.h"

void singly_list_test() {
    char data[] = "Hello";
    struct SinglyList *root = singly_list_create(&data[4], sizeof(char));
    for (int i = 3; i >= 0; i--) {
        root = singly_list_prepend(root, &data[i], sizeof(char));
    }

    int i = 0;
    struct SinglyList *node = root;
    while(node) {
        assert_equal(*((char *)node->data), data[i]);
        node = node->next;
        i++;
    }

    singly_list_destroy(root);
}

void singly_list_delete_test() {
    char data[] = "Hello";
    struct SinglyList *root = singly_list_create(&data[4], sizeof(char));
    for (int i = 3; i >= 0; i--) {
        root = singly_list_prepend(root, &data[i], sizeof(char));
    }

    root = singly_list_delete_by_data(root, root->data);
    root = singly_list_delete_by_data(root, root->next->data);
    root = singly_list_delete_by_data(root, root->next->next->data);

    assert_equal(*(char *)(root->data), 'e');
    assert_equal(*(char *)(root->next->data), 'l');

    singly_list_destroy(root);
}