/**
 * @file queue.h
 * @brief Internal pending-free management queue.
 *
 * @note This queue is designed specifically to handle "Cascading Frees" within the 
 * refmem library. 
 */

#pragma once
#include <stddef.h>

/**
 * @struct queue_node
 * @brief Private node structure for the linked-list queue.
 */
typedef struct queue_node queue_node_t;

/**
 * @struct queue_t
 * @brief A container for tracking objects awaiting deallocation.
 *
 * @note This structure should be initialized to { NULL, NULL, 0 } before use.
 */
typedef struct {
	queue_node_t *head;
	queue_node_t *tail;
	size_t count;
} queue_t;

/**
 * @brief Adds an object to the back of the pending-free queue.
 * 
 * Used during the release process when an object's reference count reaches zero.
 * 
 * @param q The queue instance (the global pending_frees queue).
 * @param data A pointer to the object (obj*) to be queued for freeing.
 * @return int Returns 0 on success, or -1 if memory allocation for the node failed.
 */
int queue_push(queue_t* q, void *data);

/**
 * @brief Removes and processes the object at the front of the queue.
 *
 * @param q The queue instance.
 * @return A pointer to the dequeued object, or NULL if the queue is empty
 */
void *queue_pop(queue_t* q);

/**
 * @brief Safely clears and frees all internal nodes within the queue.
 *
 * This should be called during library shutdown or cleanup
 *
 * @param q The queue instance to be cleared.
 */
void queue_clear(queue_t* q);
