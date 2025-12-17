#include "include/refmem.h"
#include <stdint.h>
#include <stdlib.h>

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
