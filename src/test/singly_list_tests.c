#include "singly_list_tests.h"
#include "../singly_list.h"
#include "../../lib/munit/munit.h"

void singly_list_test() {
    char data[] = "Hello";
    struct SinglyList *root = singly_list_create(&data[4], sizeof(char));
    for (int i = 3; i >= 0; i--) {
        root = singly_list_prepend(root, &data[i], sizeof(char));
    }

    int i = 0;
    struct SinglyList *node = root;
    while(node) {
        munit_assert_char(*((char *)node->data), ==, data[i]);
        node = node->next;
        i++;
    }

    singly_list_destroy(root);
}