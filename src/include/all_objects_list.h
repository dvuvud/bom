#pragma once

#include <stddef.h>

typedef struct list_node list_node_t;
typedef struct metadata metadata_t;

int all_objects_add(metadata_t *meta);

void all_objects_remove(metadata_t *meta);

metadata_t *all_objects_first();
metadata_t *all_objects_next(metadata_t *current);

void all_objects_clear();