# Design Report

## 1. Introduction

This project implements a reference-counted memory management system in the C.

The system tracks allocated objects, manages reference counts, supports custom destructors, and safely deallocates memory using a controlled cascading free mechanism. It is designed to reduce common memory errors such as double-free, use-after-free, and memory leaks.

## 2. Design Goals

The main design goals of the project are:

* **Safety** – Prevent invalid memory operations by tracking all managed objects
* **Deterministic Memory Management** – Use reference counting instead of tracing garbage collection
* **Extensibility** – Allow user-defined destructors for complex data structures
* **Performance Awareness** – Keep overhead low and operations predictable
* **Modularity** – Separate functionality into clearly defined components

## 3. System Overview

The system consists of three core modules:

1. **Hash Set** – Tracks all active allocations
2. **Queue** – Manages deferred deallocation (cascading frees)
3. **Reference Memory Manager (refmem)** – Core allocation and reference-count logic

### Memory Layout

Each allocated object is stored in memory using the following layout:

```
[ metadata ][ user object ]
```

The metadata is hidden from the user and contains bookkeeping information such as the reference count, object size, and destructor callback.

## 4. Module Design

### 4.1 Hash Set Module

**Purpose**
The hash set keeps track of all currently allocated objects. This allows the system to verify whether a pointer is managed by the reference-counted allocator before performing operations such as `retain` or `release`.

**Design**

* Hashing is based on pointer addresses
* Collisions are handled using separate chaining (linked lists)
* The hash table is lazily initialized

**Design Rationale**
Using a hash set provides:

* O(1) average-time lookup
* Protection against invalid pointers
* Prevention of double-free and undefined behavior

**Key Operations**

* `hashset_add` – Register a new allocation
* `hashset_contains` – Check if a pointer is managed
* `hashset_remove` – Remove an object during deallocation
* `hashset_cleanup` – Free all internal hash set structures

---

### 4.2 Queue Module

**Purpose**
The queue module manages objects that are ready to be deallocated but must be freed in a controlled manner.

**Design Rationale**
Immediate recursive deallocation may cause:

* Deep recursion
* Stack overflows
* Unbounded destructor chains

By using an explicit FIFO queue, the system ensures predictable memory reclamation.

**Key Operations**

* `queue_push` – Add an object to the free queue
* `queue_pop` – Remove the next object to be freed
* `queue_clear` – Empty the queue during shutdown

---

### 4.3 Reference Memory Manager (refmem)

**Purpose**
This module provides the public API for memory allocation, reference counting, deallocation, and system shutdown.

#### 4.3.1 Metadata Structure

Each allocation includes a metadata structure containing:

* `refcount` (`uint8_t`) – Number of active references (maximum 255)
* `size` (`size_t`) – Size of the user object in bytes (56 bits)
* `destructor` – Optional user-provided cleanup function

#### 4.3.2 Allocation

* `allocate` – Allocates a single zero-initialized object
* `allocate_array` – Allocates a zero-initialized array

Both functions:

* Allocate metadata and user memory in a single block
* Register the object in the hash set
* Initialize the reference count to zero

#### 4.3.3 Reference Management

* `retain` – Increments the reference count
* `release` – Decrements the reference count

When the reference count reaches zero, the object is added to the pending free queue instead of being freed immediately.

#### 4.3.4 Deallocation and Cascading Free

Objects queued for deletion are processed iteratively by `process_pending_frees` and `process_pending_frees_byte_limit`. A global cascade limit controls how many objects may be freed in a single pass, preventing long deallocation chains from blocking program execution.

## 5. Destructor Design

### 5.1 Custom Destructors

Users may provide a destructor function when allocating an object. This allows:

* Releasing internal references
* Cleaning up owned resources
* Defining object-specific cleanup behavior

### 5.2 Default Destructor

If no destructor is provided, a default destructor is used. It:

* Scans the object memory in pointer-sized steps
* Identifies potential managed pointers
* Automatically calls `release` on referenced objects

**Trade-off**
This approach assumes pointer alignment and may conservatively release values that resemble valid pointers, but it provides automatic memory cleanup with minimal user effort.

## 6. Cascade Limit

The cascade limit restricts how many objects may be freed during a single deallocation pass.

* `set_cascade_limit` – Sets the global limit
* `get_cascade_limit` – Retrieves the current limit

This mechanism ensures predictable performance and prevents long-running destructor chains.

## 7. Cleanup and Shutdown

* `cleanup` – Frees all pending objects in the queue
* `shutdown` – Fully shuts down the memory manager by:

  * Cleaning up pending frees
  * Clearing the queue
  * Destroying the hash set

## 8. Limitations and Future Work

**Current Limitations**

* Not thread safe
* Reference count is limited to 255
* Conservative pointer scanning in default destructor can result in false positives

**Possible Improvements**

* Thread-safe reference counting
* Larger or dynamic reference counters
* Improved pointer detection or optional type metadata

## 9. Logical Execution Flow

This section describes the lifecycle of a managed object and the overall logical flow of the reference-counted memory management system.

### 1. Allocation Phase

1. The user calls `allocate` or `allocate_array`.
2. A single contiguous memory block is allocated with the layout: `[ metadata ][ user object ]`
3. Metadata is initialized:
- `refcount` is set to `0`
- `size` is recorded
- A destructor function is stored (or the default destructor is assigned)
4. The allocation is registered in the global hash set.
5. A pointer to the user object is returned.

At this point, the object exists but is not yet owned by any references.

### 2. Ownership and Retention Phase

1. When a component takes ownership of an object, it calls `retain`.
2. The memory manager:
- Verifies that the pointer exists in the hash set
- Increments the reference count
3. The object may now be safely shared between multiple owners.

### 3. Release Phase

1. When an owner no longer needs the object, it calls `release`.
2. The memory manager:
- Verifies that the pointer is managed
- Decrements the reference count
3. If the reference count remains greater than zero, no further action is taken.
4. If the reference count reaches zero, the object is enqueued into the pending free queue.

Memory is not freed immediately.

### 4. Deferred Deallocation Phase

1. Objects whose reference count reaches zero are added to the pending frees queue.
2. Objects in this queue are processed by one of two functions:
   - `process_pending_frees`  
     Invoked by `release` and `deallocate` after an object has been added to the queue.
   - `process_pending_frees_byte_limit`  
     Invoked during allocation through `allocate` and `allocate_array`.
3. Objects are processed iteratively in FIFO order.
4. A global cascade limit restricts how many objects may be freed in a single pass.
5. `process_pending_frees_byte_limit` allows freeing more objects than the global cascade limit would normally allow, as long as the total amount of memory freed is less than requested by the allocation. This helps prevent running out of memory prematurely when there is lots of garbage.

This design avoids recursive deallocation and prevents long or unbounded destructor chains.

### 5. Destructor Execution Phase

For each object dequeued from the free queue:

1. The registered destructor is executed.
- Custom destructor performs user-defined cleanup.
- Default destructor:
  - Scans object memory in pointer-sized steps
  - Identifies potential managed pointers
  - Calls `release` on referenced objects
2. Any newly released objects whose reference count reaches zero are added to the free queue.

This enables controlled cascading deallocation.

### 6. Final Deallocation Phase

After the destructor completes:

1. The object is removed from the hash set.
2. The memory block containing metadata and user data is freed.
3. The cascade counter is incremented.
4. Processing continues until the free queue is empty or the cascade limit is reached.

### 7. Cleanup and Shutdown Flow

`cleanup`
- Processes all pending frees
- Leaves the system initialized

`shutdown`
- Clears the pending free queue
- Destroys the hash set
- Resets global state

# 10. Deviations from the Full Specification

This section describes the intentional deviations we made from the full project specification, along with the reasoning behind them and how we see potential future improvements.

## 10.1 Use of Zero-Initialized Allocation (`calloc`)

**Deviation**  
The function `allocate` uses zero-initialized memory through `calloc` instead of uninitialized `malloc`.

**Reasoning**  
During development, we noticed that Valgrind complained about reading uninitialized memory. This happened when the user allocated an object with `allocate`, and by extension `malloc`, and didn't initialize it to anything before it was freed. When the default destructor scans the memory for pointers, it therefore reads uninitialized memory. Setting the memory to zero with `calloc` stops Valgrind from complaining.
This had other beneficial effects as well. It uncovered a bug to do with misaligned reads that Valgrind had warned about, but that we didn't notice due to assuming the warnings had to do with uninitialized memory.

With non-zeroed memory, the default destructor could accidentally interpret uninitialized garbage values as valid pointers during conservative pointer scanning. The chances of this happening are very low, but with reused memory previously containing pointers, it is a concern.

**Future Integration**  
If performance becomes a concern, this behavior could be made configurable so that uninitialized allocations can be used when appropriate.

## 10.2 Reference Counter Size and Overflow Handling

**Deviation**  
The reference counter is an 8‑bit unsigned integer, limiting the maximum number of references to 255.

**Behavior**  
If the counter reaches its maximum value, additional `retain` calls are silently ignored.

**Reasoning**  
We chose this design to keep metadata overhead low and the implementation simple. In our expected usage, hitting the 255 limit is extremely unlikely, so the trade-off felt reasonable.

**Future Integration**  
If needed, the counter could be expanded to a larger integer type or made dynamically sized.

## 10.3 Metadata `size` Field Limit

**Deviation**
The `size` field in the metadata struct is limited to 56 bits rather than 64.

**Behavior**
If a user attempts to allocate an object whose size exceeds `2^56 − 1`, the allocation request is ignored and the function returns `NULL`.

**Reasoning**
This design choice improves memory layout by allowing tighter alignment of the metadata structure and preventing the compiler from introducing unused padding. Since metadata is allocated for every object, eliminating dead space reduces overall memory overhead and improves cache efficiency.

**Future Integration**
Allocations of this magnitude are highly unlikely in practice. However, if support for larger objects becomes necessary, an alternative metadata structure could be introduced to accommodate sizes beyond the 56-bit limit without overflow.
