#include "refmem.h"
#include <stdint.h>

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


