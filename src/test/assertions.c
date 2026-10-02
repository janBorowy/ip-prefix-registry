#include "assertions.h"
#include <assert.h>
#include <stdlib.h>

void assert_true(bool b) {
    assert(b);
}

void assert_false(bool b) {
    assert(!b);
}

void assert_equal(long a, long b) {
    assert(a == b);
}


void assert_null(void *p) {
    assert(p == NULL);
}