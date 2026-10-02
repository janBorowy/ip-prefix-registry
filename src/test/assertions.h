#ifndef ASSERTIONS_H
#define ASSERTIONS_H

#include <stdbool.h>
#include <stdint.h>

void assert_true(bool b);
void assert_false(bool b);
void assert_equal(long a, long b);
void assert_null(void *p);

#endif