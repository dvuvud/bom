#include "include/refmem.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct queue_node {
	obj *p;
	struct queue_node *next;
} queue_node_t;

typedef struct {
	queue_node_t *head;
	queue_node_t *tail;
	size_t count;
} queue_t;

static queue_t pending_frees = { NULL, NULL, 0 };

// Push an object to the end of the queue
static int queue_push(queue_t *q, obj *p) {
	queue_node_t *new_node = malloc(sizeof(queue_node_t));
	if (!new_node) return -1;

	new_node->p = p;
	new_node->next = NULL;

	if (q->tail == NULL) {
		q->head = q->tail = new_node;
	} else {
		q->tail->next = new_node;
		q->tail = new_node;
	}
	q->count++;
	return 0;
}

// Dequeue the first object in the queue
static void *queue_pop(queue_t *q) {
	if (q->head == NULL) return NULL;

	queue_node_t *temp = q->head;
	obj *p = temp->p;

	q->head = q->head->next;

	// If we just popped the last object, the tail must be NULL too
	if (q->head == NULL) {
		q->tail = NULL;
	}

	free(temp);
	q->count--;
	return p;
}

// Clear the entire queue (used in `cleanup()`)
static void queue_clear(queue_t *q) {
	while (q->head != NULL) {
		queue_pop(q);
	}
}

// Memory layout - [metadata][user object]
typedef struct metadata {
	uint8_t refcount;         // objects can have a maximum of 255 references
	size_t size;              // size of user object
	function1_t destructor;   // destructor callback (may be NULL)
} metadata_t;

// Helper function to get metadata from user object
static inline metadata_t *meta_from_obj(obj *p) {
	return ((metadata_t *)p) - 1;
}

// Helper function to get user obj from metadata
static inline obj *obj_from_meta(metadata_t *m) {
	return (obj *)(m + 1);
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

	return obj_from_meta(metadata);
}
