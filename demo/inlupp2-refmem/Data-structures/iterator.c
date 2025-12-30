#include <stddef.h>
#include <stdbool.h>
#include "linked_list.h"
#include "iterator.h"
#include <refmem.h>
#define int_elem(x) (elem_t) { .i=(x) }
#define ptr_elem(x) (elem_t) { .p=(x) }

ioopm_list_iterator_t *ioopm_iterator_create(ioopm_list_t *list) {
    //creates an iterator and allocates memory for it
    ioopm_list_iterator_t *result = allocate(sizeof(struct iter), NULL);

    result->current = list->head->next;
    result->list = list;

    retain(result->current);
    retain(list);

    return result;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter){
    return iter->current != NULL && iter->current->next != NULL; // checks if next is possible
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter) {
    if (!ioopm_iterator_has_next(iter)) return ptr_elem(NULL); //if next is not possible return null
    elem_t value = iter->current->element;

    release(iter->current);

    iter->current = iter->current->next;

    retain(iter->current);

    return value; // else return the next value
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter){
    if (ioopm_linked_list_is_empty(iter->list)) {
        return;
    }
    else {
        release(iter->current);

        iter->current = iter->list->head->next;

        retain(iter->current);
    }
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter){
    if (iter->current == NULL) {
        return ptr_elem(NULL);
    } else {
        return iter->current->element;
    }

}
