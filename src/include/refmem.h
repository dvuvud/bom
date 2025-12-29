#pragma once

#include <stddef.h>

/**
 * @brief Generic pointer to the allocated object
 *
 * This type is used to hide the objects internal metadata from the user
 * 
 * @note Because the reference counter is 8-bit, each object can hold at most
 * 255 active references. Overflowing this limit results in undefined
 * behavior.
 */
typedef void obj;

/**
 * @brief Function type for the destructor
 *
 * This function is called before an object is freed from memory 
 *
 * @param p Pointer to this object to destroy
 */
typedef void (*function1_t)(obj *);

/**
 * @brief Increments the reference counter of an object
 *
 * @param p Pointer to the object whose reference counter should be incremented
 *
 * @note If `p` is `NULL`, the call is silently ignored.
 *
 * @warning The reference counter is 8-bit (0–255). 
 * Trying to increment beyond 255 results in overflow and undefined behavior.
 *
 * @par Example:
 * @code
 * struct cell *c = allocate(sizeof(struct cell), cell_destructor);
 * retain(c); // rc(c) increments to 1
 * @endcode
 */
void retain(obj *p);

/**
 * @brief Decrements the reference counter of an object
 *
 * If the reference counter reaches 0, the object will be marked as garbage and queued for release from memory
 *
 * @param p Pointer to the object whose reference counter should be decremented
 *
 * @note If `p` is `NULL`, the call is silently ignored.
 * @par Example:
 * @code
 * struct cell *c = ...; // rc(c) = 1
 * release(c); // rc(c) is 0, the object gets prepared for destruction.
 * @endcode
 */
void release(obj *p);

/**
 * @brief Returns the reference counters value for an object
 *
 * @param p Pointer to the object
 * @return size_t The value of the objects current reference counter.
 *
 * @note If `p` is `NULL`, the return value is undefined.
 */
size_t rc(obj *p);

/**
 * @brief Allocates a block of memory (similar to `malloc`) and initiates reference counting
 *
 * @param bytes The number of bytes to allocate for the object
 * @param destructor Function to call before freeing the object, or `NULL` for default destructor
 * @return obj* Pointer to the object allocated in memory.
 *
 * @note The call the trigger collection of garbage up to the cascade limit.
 * @warning Internal memory leaks can occur if destruction is not handled correctly.
 * @par Example:
 * @code
 * struct cell *c = (struct cell*) allocate(sizeof(struct cell), cell_destructor);
 * @endcode
 */
obj *allocate(size_t bytes, function1_t destructor);

/**
 * @brief Allocates a block of memory for an array (similar to `calloc`) and initiates reference counting
 *
 * The destructor is called for each element in the array (if they aren't `NULL`).
 *
 * @param destructor Function to call for each element in the array when freeing memory, or `NULL` for a default destructor
 * @return obj* Pointer to the allocated memory.
 *
 * @note Similar to `calloc`, memory is null instantiated.
 * @par Example:
 * @code
 * // Allocates an array of 10 int pointers with a default destructor
 * obj **arr = (obj**) allocate_array(10, sizeof(obj*), NULL);
 * @endcode
 */
obj *allocate_array(size_t elements, size_t elem_size, function1_t destructor);

/**
 * @brief Frees an object immediately, if the reference counter is 0
 *
 * The objects destructor is called before the memory is freed.
 *
 * @param p Pointer to the object to be freed
 *
 * @warning Should only be called on an object `p` where `rc(p) == 0`
 */
void deallocate(obj *p);

/**
 * @brief Sets the upper limit for how many objects can be freed consecutively.
 *
 * @param limit The cascade limit
 */
void set_cascade_limit(size_t limit);

/**
 * @brief Returns the current cascade limit.
 *
 * @return size_t The current cascade limit.
 */
size_t get_cascade_limit(void);

/**
 * @brief Forces the system to free all objects whose reference counters are at 0.
 *
 * The cascade limit is ignored during this operation.
 *
 * @note Used to limit the memory load when the system is inactive.
 */
void cleanup(void);

/**
 * @brief Turns off the memory management library entirely.
 *
 * Frees all internal datastructures.
 *
 * @note Should be called at the end of the program to ensure there are not internal memory leaks coming from the library itself.
 */
void shutdown(void);
