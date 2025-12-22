#include "all_objects_list.h";
#include <stdlib.h>;

struct list_node {
    metadata_t *meta;
    list_node_t *next;
};

static list_node_t *head = NULL;

int linked_list_add(metadata_t *meta)
{
    list_node_t *node = malloc(sizeof(list_node_t));
    if (node == NULL) {
        return -1;
    }

    node->meta = meta;
    node->next = head;
    head = node;
    return 0;
}

void all_objects_remove(metadata_t *meta)
{
    list_node_t **current = &head;

    // (*current) is a pointer to the metadata, basiclly same as head..
    while (*current != NULL) {
        if ((*current)->meta == meta) {
            list_node_t *dead = *current;
            *current = dead->next;
            free(dead);
            return;
        }
        current = &(*current)->next;
    }
}

metadata_t *all_objects_first()
{
    return head ? head->meta : NULL;
}

metadata_t *all_objects_next(metadata_t *current)
{
    for (list_node_t *cur = head; cur; cur = cur->next) {
        if (cur->meta == current) {
            return cur->next->meta;
        }
    }
    return NULL;
}

void all_objects_clear()
{
    while (head != NULL) {
        list_node_t *current = head;
        head = current->next;
        free(current);
    }
}