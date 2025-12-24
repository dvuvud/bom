#pragma once

#include <stdbool.h>

void hashset_add(void *addr);
bool hashset_contains(void *addr);
void hashset_remove(void *addr);
void hashset_cleanup();
