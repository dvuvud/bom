#pragma once

#include <stdbool.h>

/**
 * @brief Adds an address to the hash set for tracking
 *
 * @param addr Pointer to the object to track
 *
 * @note If `addr` is `NULL`, the call is silently ignored.
 * @note If `addr` is already in the set, it will not be added again.
 * @note The hash set is initialized lazily on the first call to this function.
 */
void hashset_add(void *addr);

/**
 * @brief Checks if an address exists in the hash set
 *
 * @param addr Pointer to check
 * @return true if the address is tracked in the hash set
 * @return false if the address is not tracked or the hash set is uninitialized
 */
bool hashset_contains(void *addr);

/**
 * @brief Removes an address from the hash set
 *
 * @param addr Pointer to the object to stop tracking
 *
 * @note If `addr` is `NULL` or not in the set, the call is silently ignored.
 */
void hashset_remove(void *addr);

/**
 * @brief Frees all internal memory used by the hash set
 *
 * @note After calling this function, the hash set can be reused and will
 * be lazily reinitialized on the next call to `hashset_add`.
 */
void hashset_cleanup(void);
