# Boomers of Memory

A reference-counted memory management system for C with automatic cleanup through conservative object scanning and configurable cascade limits.

## Documentation

This project uses [Doxygen](https://www.doxygen.nl) to generate API documentation.

### Generating docs
```bash
doxygen Doxyfile
```
This will generate the documentation in the `docs/html` folder. You can open the `index.html` file in a web browser to view the documentation.

## Build & Run

### Commands

#### Using Makefile
```bash
# Clone with submodules (om vi lägger till några)
git clone --recursive https://github.com/IOOPM-UU/bom.git
cd bom

# Build library
make
```

## Running tests

#### Using Makefile
```bash
# Build and run tests
make test

# Build and run tests with Valgrind
make memtest
```

## Generate coverage reports

#### Using Makefile
```bash
# Generate reports
make generate_coverage
```
After being created, coverage reports can be found in `docs/coverage`.
