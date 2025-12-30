# Code Quality Report

## Participant List
| Name                   | Email                                    | Active Dates           |
|------------------------|------------------------------------------|------------------------|
| David Buhrman          | david.buhrman.????@student.uu.se         | 3/12 2025 – 16/1 2026  |
| Erik Verastegui Ochoa  | erik.verastegui-ochoa.3297@student.uu.se | 3/12 2025 – 16/1 2026  |
| Hiba Ahmad             | hiba.ahmad.5302@student.uu.se            | 3/12 2025 – 16/1 2026  |
| Joel Terenius          | joel.terenius.0417@student.uu.se         | 3/12 2025 – 16/1 2026  |
| Nora Ayaz              | nora.ayaz.1089@student.uu.se             | 3/12 2025 – 16/1 2026  |
| Veronica Pettersson    | veronica.pettersson.7655@student.uu.se   | 3/12 2025 – 16/1 2026  |

---

# 1. Overview



---

# 2. Code Quality Goals

From the beginning of the project, we aimed to write code that is easy to read,
easy to test, and easy to maintain over time. Since the system was developed by
multiple people, clarity and consistency were more important than clever or
overly compact solutions.

We focused on keeping responsibilities well separated, making functions small
and focused, and writing code that could be understood without deep knowledge of
the entire system. This approach made it easier to modify existing functionality
and to locate and fix bugs during development.

These principles guided our decisions throughout the project and influenced how
the system was structured and implemented.

---

# 3. Readability
## 3.1 Coding Style and Naming

To improve readability and consistency, we used the Apache C Style Guide as a
reference for formatting and code structure. While it was not followed strictly
in every detail, it provided a common baseline for indentation, brace placement,
and general layout.

Function and variable names were chosen to reflect their purpose rather than
their implementation details. This made it easier to understand the role of
different components without having to inspect their full implementation.

---

## 3.2 Comments and Documentation

Comments were used primarily to explain intent and non-obvious behavior, rather
than restating what the code already expresses. Public functions were documented
using comments describing their purpose, parameters, and expected behavior.

This made it easier to use and test individual modules without needing to read
their full implementation.

---


# 4. Maintainability and Structure

Maintainability was achieved by structuring the system into clear and well-defined
modules with distinct responsibilities. The core reference-counted memory
management is separated from supporting data structures such as the queue and
hash set, each implemented in its own file.

Public interfaces are defined in header files and kept small, while
internal implementation details are hidden using static functions. This makes
it possible to change or improve internal behavior without affecting other parts
of the system, as long as the interfaces remain unchanged.

The code follows consistent patterns for error handling and control flow, which
makes future modifications easier and less error-prone. 

Overall, these design choices made the system easier to extend, debug, and
maintain over time.

---

# 5. Correctness and Memory Safety

Correctness and memory safety were central concerns throughout the implementation
of the reference-counted memory management system. The code was written to behave
predictably even in the presence of invalid inputs or unexpected usage patterns,
and to avoid undefined behavior where possible.

# 5.1 Defensive Programming

Defensive programming techniques are used consistently across the codebase.
Functions validate their inputs early, and invalid states such as NULL pointers,
untracked objects, or incorrect reference counts are handled safely by returning
without performing any action. This prevents illegal memory access and reduces
the risk of crashes.

Reference counting operations are used to avoid overflow, and
objects are only freed when their reference count reaches zero and they are
known to be tracked by the system. Destructors are used in a controlled and
well-defined manner, ensuring that cleanup logic is executed before memory is
released.

# 5.2 Adherence to Specification

The implementation follows the project specification by replacing direct use of
malloc, calloc, and free with a reference-counted memory management
interface. Objects are only freed when their reference count reaches zero, and
explicit rules for allocation, retention, and release are consistently enforced.

Support for destructors and default destructors ensures that complex data
structures are cleaned up correctly. Cascading frees are handled in a controlled
manner, and cleanup and shutdown functionality ensures that all internal data
structures are released at program termination.

---

# 6. Testability
# 6.1 Design for Testing

Det är designat för att vara testbart från början. Fanns ett krav i projektspecen.
Det olika modulerna gör att det kan testas var för sig

---

# 7. Performance


---

# 8. Consequences of Our Design Choices

Om det finns

---

# 9. Conclusion


