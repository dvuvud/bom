/**
 * @file all_objects_list.h
 * @brief internal linked_list for all metadata.
 *
 * @note this implementation is specificlly made to handle all metadata
 * allocated in refmem library.
 */

#pragma once

#include <stddef.h>

typedef struct list_node list_node_t;
typedef struct metadata metadata_t;

/**
 * @brief adds metadata to the front at the list.
 *
 * @param meta data to be added to the list.
 * @return int -1 if allocation failed, otherwise 0.
 */
int all_objects_add(metadata_t *meta);

/**
 * @brief removes a given metadata from the list.
 *
 * @param meta the data to be removed.
 * @note does nothing if the data is not in the list.
 */
void all_objects_remove(metadata_t *meta);

/**
 * @brief returns the first element in the list.
 *
 * @return metadata_t* the first element in the list, and NULL if the list is
 * empty.
 */
metadata_t *all_objects_first();

/**
 * @brief finds the next data in the list from a given metadata.
 *
 * @param current the current metadata.
 * @return metadata_t* the next node in the list from given metadata, can
 * be NULL.
 */
metadata_t *all_objects_next(metadata_t *current);

/**
 * @brief clears all objects in the list
 *
 * @note does not free the data inside the list, only the nodes of the list
 * itself.
 */
void all_objects_clear();