#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "linked_list.h"
#include "refmem.h"

/**
 * @file linked_list.c
 * @author Hiba Ahmad
 * @date 10 Oct 2025
 * @brief A complete linked list implementation for Step 10.
 *
 * @note This implementation is for lists of integers only.
 * Failure is handled using assert() ex, for NULL pointers or invalid indexes.
 * All index based functions assume that the first element is at index 0.
 * Append and size are implemented in o(1) using a tail pointer and size counter.
 */

///---------------------------------------------
///            Helpfunctions
///---------------------------------------------
bool int_eq(elem_t a, elem_t b) { return a.i == b.i; }

bool str_eq(elem_t a, elem_t b)
{
    const char *sa = (const char *)a.p;
    const char *sb = (const char *)b.p;

    if (sa == NULL || sb == NULL){
    return sa == sb;
    }
    else{
    return strcmp(sa, sb) == 0;
    }
}

static ioopm_link_t *link_create(elem_t value, ioopm_link_t *next)
{
    ioopm_link_t *link = allocate(sizeof(*link), NULL);
    assert(link);

    link->element = value;
    link->next    = next;
    return link;
}

static ioopm_link_t *list_inner_find_previous(ioopm_link_t *link, size_t index)
{
    ioopm_link_t *cursor = link;

    for (size_t i = 0; i < index; ++i)
    {
        assert(cursor != NULL);
        cursor = cursor->next;
    }

  return cursor;
}

// elper that ensures index is within valid range [0, upper_bound)
static int list_inner_adjust_index(int index, int upper_bound)
{
    if (index < 0 || index >= upper_bound)
    {
        assert(false && "Index out of bounds");
    }

    return index;
}

///---------------------------------------------------
///                Public functions
///---------------------------------------------------


/// @brief Creates a new empty list
/// @return an empty linked list
ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function *eq_fun)
{
    ioopm_list_t *list = allocate(sizeof(*list), NULL);
    assert(list);

    list->head = link_create(int_elem(0), NULL);
    list->tail = list->head;   // if empty list: tail == head
    list->size = 0;

    if (eq_fun != NULL)
    {
        list->eq_fun = eq_fun;
    }
    else
    {
        list->eq_fun = int_eq;
    }

    return list;
}

/// @brief Tear down the linked list and return all its memory (but not the memory of the elements)
/// @param list the list to be destroyed
void ioopm_linked_list_destroy(ioopm_list_t *list)
{
    if (!list)
    return;

   ioopm_linked_list_clear(list);

    // Free list structur
    deallocate(list->head);
    release(list);
}


/// @brief Insert at the end of a linked list in O(1) time
/// @param list the linked list that will be appended
/// @param value the value to be appended
/// @note Supposes that list is not NULL, faials with assert(list) if it is NULL
/// @note Uses a tail pointer to achieve O(1) time complexity
/// @note Supports generic data using elem_t
void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
{
    // make sure list is not NULL
    assert(list);

    if (value.p != NULL) {
        retain(value.p);
    }

    // create a new link,tha placed at the end
    ioopm_link_t *new_link = link_create(value, NULL);

    // Connect the new node after the current tail
    list->tail->next = new_link;
    // let pointer poit to the nexts tail
    list->tail = new_link;

    list->size++;
}

/// @brief Insert at the front of a linked list in O(1) time
/// @param list the linked list that will be prepended to
/// @param value the value to be prepended
/// @note Supposes that list is not NULL, faials with assert(list) if it is NULL
void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
    assert(list);

    if (value.p != NULL) {
        retain(value.p);
    }
    // Create a new node and point to the current head
    ioopm_link_t *new_link = link_create(value, list->head->next);

    list->head->next = new_link;
    // empty list -> tail is sentinel
    if( list->tail == list->head)
    {
        list->tail = new_link;
    }
    // Update the list size
    list->size++;
}

/// @brief Insert an element into a linked list in O(n) time.
/// The valid values of index are [0,n] for a list of n elements,
/// where 0 means before the first element and n means after
/// the last element.
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @param value the value to be inserted
/// @note Supposes that list is not NULL, faials with assert(list) if it is NULL
/// @note Supports generic data using elem_t
/// @note Uses a tail pointer to achieve O(1) time complexity for insert at end
void ioopm_linked_list_insert(ioopm_list_t *list, size_t index, elem_t value)
{
    assert(list);
    // to check if idex in the range
    int valid_index = list_inner_adjust_index(index, (int)list->size + 1);

    ioopm_link_t *prev = list_inner_find_previous(list->head, (size_t)valid_index);

    if (value.p != NULL) {
        retain(value.p);
    }
    // create a new link
    ioopm_link_t *new_link = link_create(value, prev->next);
    prev->next = new_link;

    // if we inserted at the end, update the tail
    if (new_link->next == NULL)
    {
         list->tail = new_link;
    }
    list->size++;
}

/// @brief Remove an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list the linked list
/// @param index the position in the list
/// @return the value removed
elem_t ioopm_linked_list_remove(ioopm_list_t *list, size_t index)
{
    assert(list);
    // keep index in range
    int valid_index = list_inner_adjust_index(index, (int)list->size);

    // find node before target
    ioopm_link_t *prev = list_inner_find_previous(list->head, (size_t)valid_index);
    // node to delete
    ioopm_link_t *to_remove = prev->next;
    assert(to_remove); // must exist

    prev->next = to_remove->next;  // save value before freeing
    elem_t value = to_remove->element; // unlink it
    if (value.p != NULL) {
        release(value.p);
    }
    deallocate(to_remove); // free memory

    // removed the last real node
    if (prev->next == NULL)
    {
        list->tail = prev;  // tail falls back
    }
    // update size and return removed value
    list->size--;
    return value;
}
/// @brief Retrieve an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @return the value at the given position
elem_t ioopm_linked_list_get(const ioopm_list_t *list, size_t index)
{
    assert(list);
    int valid_index = list_inner_adjust_index(index, (int)list->size);
    ioopm_link_t *prev = list_inner_find_previous(list->head, (size_t)valid_index);
    assert(prev->next);
    // return the value at that position
    return prev->next->element;
}

/// @brief Test if an element is in the list
/// @param list the linked list
/// @param element the element sought
/// @return true if element is in the list, else false
bool ioopm_linked_list_contains(const ioopm_list_t *list, elem_t element)
{
    assert(list); // list must exist
    ioopm_link_t *cursor = list->head->next;

    while(cursor)
    {
        if (list->eq_fun(cursor->element, element))
        {
            return true; // stop if we found
        }
        cursor = cursor->next;
    }

    return false; // not found
}

/// @brief Lookup the number of elements in the linked list in O(1) time
/// @param list the linked list
/// @return the number of elements in the list
size_t ioopm_linked_list_size(const ioopm_list_t *list)
{
    assert(list);
    return list->size;
}

/// @brief Test whether a list is empty or not
/// @param list the linked list
/// @return true if the number of elements int the list is 0, else false
/// @note Assumes that list is not NULL, fails with assert(list) if it is NULL
/// @note This function runs in O(1) time by delegating to ioopm_linked_list_size
bool ioopm_linked_list_is_empty(const ioopm_list_t *list)
{
    assert(list);// make sure list is not NULL
    return list->size == 0; // empty when size is zero
}

/// @brief Remove all elements from a linked list
/// @param list the linked list
/// @note Assumes that list is not NULL, fails with assert(list) if it is NULL
/// @note Frees all links but not the list structure itself
/// @note Supports generic data using elem_t
void ioopm_linked_list_clear(ioopm_list_t *list)
{
    assert(list);

    ioopm_link_t *cursor = list->head->next; // skip sentinel
    while (cursor) {
        ioopm_link_t *tmp = cursor;      // Remember current node
        cursor = cursor->next;    // Move forward

        if (tmp->element.p != NULL) {
            release(tmp->element.p);
        }

        deallocate(tmp); // Free node
    }
    // reset sentinel pointers and size
    list->head->next = NULL;
    list->tail = list->head;
    list->size = 0;
}

/// @brief Test if a supplied property holds for all elements in a list.
/// The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested (function pointer)
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for all elements in the list, else false
/// @note A Assumes that list and fun are not NULL. Fails with assert(list) if is NULL
/// @note Its return false as soon as one element fails the predicate.
/// @note Supports generic data using elem_t
/// @note Runs in O(n) time by traversing the list
bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_predicate *prop, void *extra)
{
    assert(list);
    assert(prop);  // predicate function must exist

    ioopm_link_t *cursor = list->head->next;
    while (cursor)
    {
        if (!prop(int_elem(0), cursor->element, extra))  // if prop returns false for any element
        {
            return false;
        }
        cursor = cursor->next;// move to the next link
    }
    return true;  // All passed
}
/// @brief Test if a supplied property holds for any element in a list.
/// The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for any elements in the list, else false
/// @note Assumes that list and prop are not NULL, fails with assert(list)
/// @note  It will return as soon as one elemnent satisfied the predicate
/// @note Supports generic data using elem_t
bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_predicate *prop, void *extra)
{
    assert(list);
    assert(prop);

    ioopm_link_t *cursor = list->head->next;                      // start at the head
    while (cursor)
    {
        if (prop(int_elem(0), cursor->element, extra))   // if prop returns true for any element
        {
            return true;
        }
        cursor = cursor->next;// move to the next link
    }
    return false; // nothing matched
}

/// @brief Apply a supplied function to all elements in a list.
/// @param list the linked list
/// @param fun the function to be applied
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of fun
/// @note Assumes that list and fun are not NULL. Fails with assert(list) if violated.
/// @note The function pointer operates on elem_t values
/// @note Supports generic data using elem_t
void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_function *fun, void *extra)
{
    assert(list);
    assert(fun);

    ioopm_link_t *cursor = list->head->next;// start at the head
    while (cursor)
    {
        fun(int_elem(0), cursor->element, extra);  // apply the function to the current element
        cursor = cursor->next;// move to the next link
    }
}