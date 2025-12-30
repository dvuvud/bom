# Inlupp2 description

This folder contains all files for Assignment 2 in the IOOPM course HT-2025.

The project implements a complete text-based webstore system, divided into four main modules:  
**Data-structures**, **Utils**, **Webstore-backend**, and **Webstore-frontend**, with unit tests located in **Tests**.

# Building and running the program

To build and run the backend tests, enter "make test"

To build and run the backend tests tests and check coverage, enter "make coverage"

To build and run the backend tests tests while using valgrind, enter "make valgrind"

To build and run the text-based frontend program, enter "make run"


# Line and Branch Coverage

Coverage is measured using gcov (via make coverage):

1. Webstore_backend.c: 90.00% line coverage, 91.30% branch coverage 

2. hash_table.c: 67.57% line coverage, 60.61% branch coverage 

3. linked_list.c: 82.84% line coverage, 78.95% branch coverage  

4. iterator.c: 95.65% line coverage, 100.00% branch coverage  

5. utils.c: 0.00% line coverage, 0.00% branch coverage  

6. backend_tests.c: 99.19%  line coverage, 100.00% branch coverage  

Total line coverage: 87.80%

# Testing and Memory Management

Test summary:

- 41 total tests  
- 117 assertions  
- 0 failed tests  

Valgrind summary:
- All heap blocks freed — no leaks are possible

# Authors

- Amelie Weiss  
- Hiba Ahmad

