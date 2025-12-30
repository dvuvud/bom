#pragma once
#include <stdbool.h>
#include <stddef.h>

#define int_elem(x) (elem_t) { .i=(x) }
#define ptr_elem(x) (elem_t) { .p=(x) }

typedef union elem elem_t;
union elem {
    int i;
    unsigned int u;
    bool b;
    float f;
    void *p;
};
typedef bool ioopm_eq_function(elem_t a, elem_t b);
typedef bool ioopm_predicate(elem_t key, elem_t value, void *extra);
typedef void ioopm_apply_function(elem_t key, elem_t value, void *extra);