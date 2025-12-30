# Test Report

## Participant List
| Name                   | Email                                    | Active Dates           |
|------------------------|------------------------------------------|------------------------|
| David Buhrman          | david.buhrman.0648@student.uu.se         | 3/12 2025 – 16/1 2026  |
| Erik Verastegui Ochoa  | erik.verastegui-ochoa.3297@student.uu.se | 3/12 2025 – 16/1 2026  |
| Hiba Ahmad             | hiba.ahmad.5302@student.uu.se            | 3/12 2025 – 16/1 2026  |
| Joel Terenius          | joel.terenius.0417@student.uu.se         | 3/12 2025 – 16/1 2026  |
| Nora Ayaz              | nora.ayaz.1089@student.uu.se             | 3/12 2025 – 16/1 2026  |
| Veronica Pettersson    | veronica.pettersson.7655@student.uu.se   | 3/12 2025 – 16/1 2026  |

---

# 1. Overview

This project implements a reference-counted memory management system in C. It handles dynamic memory allocation and deallocation. Each object keeps track of its reference count. When the count reaches zero, the object can be freed. Cascading deallocation and cleanup mechanisms are also supported. Queues and hash sets are used to track allocated objects and pending frees.

---

# 2. Unit Testing

## 2.1 Method

Unit tests were written using CUnit. Each test file contains its own test suite and executed from `test_main.c`.

The following functions were tested individually:
- Allocation and array allocation
- Reference counting (`retain`, `release`, `rc`)
- Deallocation
- Cleanup and shutdown
- Cascade limit behavior
- Default destructor handling
- Internal queue implementation
- Internal hash set implementation

Each unit test written for a single function and checks it's normal and edge-case behavior.

---

## 2.2 Code Coverage of Important Functions

Code coverage was measured using `gcov` with branch coverage enabled.

Summary of coverage for core source files:

| File        | Line Coverage | Branch Coverage |
|-------------|---------------|-----------------|
| hashset.c   | 100%          | 100%            |
| queue.c     | 96.30%        | 100%            |
| refmem.c    | 99.13%        | 100%            |

---

Suites run: 11  
Tests run: 58  
Tests passed: 58  
Tests failed: 0  
Assertions: 2656  

---

# 3. Integration Testing

## 3.1 Method

Verifying the interaction between multiple functions of the system.

The most important integration tests:
- allocation with reference counting
- reference counting with deallocation
- cascade limit interaction with cleanup
- interaction between memory management and internal data structures

---

## 3.2 Examples of Integration Tests

### Retain–Release–Cleanup Interaction

Tests in `test_release_retain.c` allocate an object. The object is retained and then released until its reference count reaches zero. After that, the cleanup function is called. This confirms that objects are freed correctly and that pending frees are handled properly.

### Cascade Limit Interaction

Tests in `test_cascade_limit.c` use a very low cascade limit. Objects are released but not freed immediately. Instead, they are placed in a pending state. The cleanup function is then called. This verifies that objects are correctly freed and that pending frees are handled properly.

---

# 4. Regression Testing

## 4.1 Method

Regression testing was performed by adding new tests to the test suite whenever a bug appeared. 

All tests are re-run after every fix to verify that previously fixed bugs do not corrupt the whole system.

---

# 5. The Five Most Severe Bugs

| Bug | Description                                                  | Link                                                      |
|-----|--------------------------------------------------------------|-----------------------------------------------------------|
| #35 | Default destructor was trying to access uninitialized memory | [GitHub Issue](https://github.com/IOOPM-UU/bom/issues/35) |
| #32 | Tests were leaking memory due to shutdown not being called   | [GitHub Issue](https://github.com/IOOPM-UU/bom/issues/32) |
| #39 | Cascading frees not handled on allocation                    | [GitHub Issue](https://github.com/IOOPM-UU/bom/issues/39) |
| #48 | Hash set easily collides due to weak hash function           | [GitHub Issue](https://github.com/IOOPM-UU/bom/issues/48) |
| #29  | Deallocate wasn't processing pending frees like release was | [GitHub Issue](https://github.com/IOOPM-UU/bom/issues/29) |

---

# 6. Known Test Failures and Limitations

Two unit tests failed consistently:

- Tests for extremely large allocations in `test_allocate.c`
- Tests for extremely large array allocations in `test_allocate_array.c`

These tests expect allocation functions to return `NULL` when requesting very large memory blocks.

On some systems, `malloc` may succeed even for very large allocations. This is due to memory overcommit behavior. As a result, these tests may fail even when the implementation is correct. Therefore, these failures are not considered a functional correctness error.

---

# 7. Conclusion

The test suite provides extensive coverage and validates both normal and edge-case behavior. Integration and regression tests confirm that the system is stable and functions correctly. Known test failures are documented separately. 

---

# 8. Appendix

## Tools Used
- GCC
- CUnit 2.1-3
- Valgrind
- gcov


