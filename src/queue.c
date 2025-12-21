#include "include/queue.h"
#include <stdlib.h>

struct queue_node {
    void *data;
    struct queue_node *next;
};

// Push an object to the end of the queue
int queue_push(queue_t *q, void *data)
{
    queue_node_t *new_node = malloc(sizeof(queue_node_t));
    if (!new_node) {
        return -1;
    }

    new_node->data = data;
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
void *queue_pop(queue_t *q)
{
    if (q->head == NULL) {
        return NULL;
    }

    queue_node_t *temp = q->head;
    void *data = temp->data;

    q->head = q->head->next;

    // If we just popped the last object, the tail must be NULL too
    if (q->head == NULL) {
        q->tail = NULL;
    }

    free(temp);
    q->count--;
    return data;
}

// Clear the entire queue (used in `cleanup()`)
void queue_clear(queue_t *q)
{
    while (q->head != NULL) {
        queue_pop(q);
    }
}
