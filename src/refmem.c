#include "include/refmem.h"
#include "include/queue.h"
#include <stdint.h>
#include <stdlib.h>

#define REFCOUNT_MAX 255

static queue_t pending_frees = { NULL, NULL, 0 };

static size_t cascade_limit = 100;      // Global cascade limit (default value)
//static size_t cascade_counter = 0;    // Den läggs till senare när hela cascade-logiken kopplas ihop.


// Memory layout - [metadata][user object]
typedef struct metadata {
    uint8_t refcount;         // objects can have a maximum of 255 references
    size_t size;              // size of user object
    function1_t destructor;   // destructor callback (may be NULL)
    struct metadata *next;    // pointer to the next object's metadata struct
    struct metadata *prev;    // pointer to the previous object's metadata struct
} metadata_t;

static metadata_t *object_list_head = NULL;

// Helper function to get metadata from user object
static inline metadata_t *meta_from_obj(obj *p)
{
    return ((metadata_t *)p) - 1;
}

// Helper function to get user obj from metadata
static inline obj *obj_from_meta(metadata_t *m)
{
    return (obj *)(m + 1);
}

size_t rc(obj *p)
{
    // If pointer is NULL, refcount is 0
    if (p == NULL) {
        return 0;
    }

    // Get metadata from object pointer
    metadata_t *meta = meta_from_obj(p);

    // Return refcount
    return meta->refcount;
}

void free_object(obj *p)
{
    if (p == NULL) {
        return;
    }

    // get metadata
    metadata_t *meta = meta_from_obj(p);

    // Reassign the previos objects next meta link, or the head of the list
    if (meta->prev) {
        meta->prev->next = meta->next;
    } else {
        object_list_head = meta->next;
    }

    // Reassign the next objects prev meta link
    if (meta->next) {
        meta->next->prev = meta->prev;
    }

    // call destructor if it exists
    if (meta->destructor != NULL) {
        meta->destructor(p);
    }

    // free metadata
    free(meta);
}

void retain(obj *p)
{
    if (p == NULL) {
        return;
    }
    
    // get metadata from object pointer
    metadata_t *meta = meta_from_obj(p);
    
    if (meta->refcount == REFCOUNT_MAX) {
        // error! refcount overflows
        return;
    }
    
    // decrease refcount
    meta->refcount++;
}

void release(obj *p)
{
    if (p == NULL) {
        return;
    }

    // get metadata from object pointer
    metadata_t *meta = meta_from_obj(p);

    // if refcount is 0, do nothing
    if (meta->refcount == 0) {
        return;
    }

    // decrease refcount
    meta->refcount--;

    // if there are still refs, stop
    if (meta->refcount > 0) {
        return;
    }

    // object is garbage, add it to the queue
    queue_push(&pending_frees, p);

    // free objects
    size_t i = 0;
    while (pending_frees.count > 0 && i < cascade_limit) {
        obj *garbage = queue_pop(&pending_frees);
        free_object(garbage);
        i++;
    }
}

obj *allocate(size_t bytes, function1_t destructor)
{
    metadata_t *metadata;

    metadata = malloc(sizeof(metadata_t) + bytes);
    if (metadata == NULL) {
        return NULL;
    }

    metadata->refcount = 0;
    metadata->size = bytes;
    metadata->destructor = destructor;

    metadata->next = object_list_head;
    metadata->prev = NULL;

    if (object_list_head != NULL) {
        object_list_head->prev = metadata;
    }

    object_list_head = metadata;

    return obj_from_meta(metadata);
}

// Allocates and null-initializes an array with `elememts` number of elements of `elem_size` size
obj *allocate_array(size_t elements, size_t elem_size, function1_t destructor)
{
    metadata_t *metadata;
    size_t total_bytes;

    if (elements == 0 || elem_size == 0) {
        return NULL;
    }

    total_bytes = elements * elem_size;

    metadata = calloc(1, sizeof(metadata_t) + total_bytes);
    if (metadata == NULL) {
        return NULL;
    }

    metadata->refcount = 0;
    metadata->size = total_bytes;
    metadata->destructor = destructor;

    metadata->next = object_list_head;
    metadata->prev = NULL;

    if (object_list_head != NULL) {
        object_list_head->prev = metadata;
    }

    object_list_head = metadata;

    return obj_from_meta(metadata);
}

// Sets the global cascade limit.
void set_cascade_limit(size_t limit)
{
    cascade_limit = limit;
}

// Returns the current cascade limit.
size_t get_cascade_limit(void)
{
    return cascade_limit;
}


void deallocate(obj *p)
{
    if (p == NULL) {
        return;
    }

    metadata_t *meta;

    meta = meta_from_obj(p);

    if (meta->refcount != 0) {
        return;
    }

    free_object(p);
}

void cleanup()
{
    while (pending_frees.count > 0) {
        obj *garbage = queue_pop(&pending_frees);
        free_object(garbage);
    }

}

void shutdown()
{
    metadata_t *meta = object_list_head;
    metadata_t *next;

    while (meta != NULL) {
        next = meta->next;
        free_object(obj_from_meta(meta));
        meta = next;
    }

    // only contains garbadge pointers at this point..
    queue_clear(&pending_frees);
}
