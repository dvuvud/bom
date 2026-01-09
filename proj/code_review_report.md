# Code Review Report

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

## 1. Overview

This review examines the reference-counted memory management system as a whole. 
The primary focus is on structure, readability, correctness, and whether the code 
fulfills project requirements. Rather than going into every individual function, 
this review discusses the main design choices and how effectively they work.

---

## 2. General Structure

The code is organized logically. Each component manages its own responsibility: 
- refmem handles reference-counted memory, 
- queue takes care of pending deallocations, 
- and hashset keeps track of allocated objects. 

This separation makes things clear. It can be easily found where to look, 
whenever we need to test or update something.

---

## 3. Readability

### 3.1 Naming and Style

Most function and variable names are descriptive. When you see a function name, 
it’s usually clear what it does. The coding style is consistent throughout the 
files—indentation, use of braces, and control flow all match up, which really 
helps when reading the code.

---

### 3.2 Comments

Comments appear where they’re needed, mostly explaining why something is done rather
than restating the code. They’re not excessive, and they help clarify the reasoning 
behind the implementation.

---

## 4. Correctness and Memory Safety

### 4.1 Safe Usage

Public functions do a good job of checking for errors—NULL pointers are handled, 
unmanaged objects are ignored, and the code only frees objects when their reference 
count reaches zero. These checks keep things reliable and show that memory safety 
was a priority during development.

---

### 4.2 One Notable Issue

One area stands out: 
- the rc function checks for NULL, but doesn’t verify that the pointer is managed 
by the system. If someone calls it with an unmanaged pointer, the behavior is 
undefined. This isn’t a major problem, but it’s inconsistent with the other functions, 
and could be fixed easily by adding a check.

---

## 5. Edge Cases

### 5.1 Allocation Failures

Some sections of the code assume malloc or calloc will succeed. If these ever fail, 
the outcome isn’t always clear. This is rare, but handling these cases would make 
the system more robust.

---

### 5.2 Large Allocations

When allocating large arrays, there’s a chance for integer overflow before size checks
 are performed. This is an uncommon scenario, but addressing it would make the code safer.

---

## 6. Cascade Free Mechanism

The cascade free feature uses a queue to avoid deep recursion during deallocation. 
This keeps things manageable and prevents stack overflows. There’s a cascade limit 
to stop long deallocation chains from blocking everything else. The logic is solid, 
though a brief comment explaining how the limits interact would help avoid confusion.

---

## 7. Testing

Testing is a significant strength here. Unit tests cover every function, and 
integration tests ensure the modules work together. Test coverage is high—most 
untested areas involve memory allocation failures, which are hard to simulate. 
The missing coverage is clearly documented in the test report.

---

## 8. Overall Evaluation

Overall, the code is well structured, readable, thoroughly tested, and generally 
safe and correct. The main issues are edge cases that are unlikely to come up in 
regular use.

---

## 9. Conclusion

This code accomplishes its goal—it implements a reference-counted memory management 
system that meets the requirements. The design is clear, and the system behaves as 
expected. With a bit more input validation and attention to rare edge cases, 
it could be even better, but as it is, it’s a solid and well-done project in general.

---