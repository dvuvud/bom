#include "include/hashset.h"
#include <stdlib.h>
#include <stdint.h>

#define HASH_TABLE_SIZE 1024

typedef struct hash_node {
    void *addr;
    struct hash_node *next;
} hash_node_t;

typedef struct {
    hash_node_t **buckets;
    size_t bucket_count;
    size_t item_count;
} hash_set_t;

static hash_set_t addr_set = {NULL, 0, 0};

// Simple hash function for addresses
static size_t hash_addr(void *addr)
{
    uintptr_t val = (uintptr_t)addr;
    // Shift right by 3 to ignore alignment bits, then mod by bucket count
    return (val >> 3) % addr_set.bucket_count;
}

static void hashset_init(void)
{
    addr_set.bucket_count = HASH_TABLE_SIZE;
    addr_set.buckets = calloc(addr_set.bucket_count, sizeof(hash_node_t *));
    addr_set.item_count = 0;
}

void hashset_add(void *addr)
{
    if (addr == NULL) {
        return;
    }

    if (addr_set.buckets == NULL) {
        hashset_init();
    }

    size_t bucket = hash_addr(addr);

    hash_node_t *current = addr_set.buckets[bucket];
    while (current != NULL) {
        if (current->addr == addr) {
            return;  // Already exists
        }
        current = current->next;
    }

    // Add new node at front of list
    hash_node_t *node = malloc(sizeof(hash_node_t));
    node->addr = addr;
    node->next = addr_set.buckets[bucket];
    addr_set.buckets[bucket] = node;
    addr_set.item_count++;
}

bool hashset_contains(void *addr)
{
    if (addr_set.buckets == NULL) {
        return false;
    }

    size_t bucket = hash_addr(addr);
    hash_node_t *current = addr_set.buckets[bucket];

    while (current != NULL) {
        if (current->addr == addr) {
            return true;
        }
        current = current->next;
    }

    return false;
}

void hashset_remove(void *addr)
{
    if (addr_set.buckets == NULL) {
        return;
    }

    size_t bucket = hash_addr(addr);
    hash_node_t **current = &addr_set.buckets[bucket];

    while (*current != NULL) {
        if ((*current)->addr == addr) {
            hash_node_t *to_free = *current;
            *current = (*current)->next;
            free(to_free);
            addr_set.item_count--;
            return;
        }
        current = &(*current)->next;
    }
}

void hashset_cleanup()
{
    if (addr_set.buckets == NULL) {
        return;
    }

    for (size_t i = 0; i < addr_set.bucket_count; i++) {
        hash_node_t *current = addr_set.buckets[i];
        while (current != NULL) {
            hash_node_t *next = current->next;
            free(current);
            current = next;
        }
    }

    free(addr_set.buckets);
    addr_set.buckets = NULL;
    addr_set.bucket_count = 0;
    addr_set.item_count = 0;
}
