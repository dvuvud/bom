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
Generiska datastrukturer, refmem är isolerat (egen modul)

---

# 5. Correctness and Memory Safety
# 5.1 Defensive Programming

Använder inte malloc/calloc och free utanför refmem, vi retainar data när det sätts in i datastructurer och
gör release när de tas bort från datastructurer
ex i hashtabel kan man hämta?

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


