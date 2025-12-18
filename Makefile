# ------- Kompilator och flaggor ---------
CC      = gcc
CFLAGS  = -Wall -pedantic -g -Isrc/include
CUNIT   = -lcunit
COVERAGE_FLAGS = --coverage -o0

# -------------- Kataloger ----------------
SRCDIR  = src
TESTDIR = test
OBJDIR  = obj
BINDIR  = bin
COVDIR = docs/coverage

# ----------------- Filer -----------------
LIB_SRC  = $(SRCDIR)/refmem.c
LIB_OBJ  = $(OBJDIR)/refmem.o

TEST_SRC = $(TESTDIR)/test_allocate_array.c
TEST_BIN = $(BINDIR)/unittests

# -------------- Standardmål ---------------
all:

# --------- Bygg bibliotekets .o -----------
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c $< -o $@

# ------------ Bygg testprogram -------------
$(TEST_BIN): $(LIB_OBJ) $(TEST_SRC)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $^ $(CUNIT) $(LDFLAGS) -o $@

# --------------- Kör tester -----------------
test: $(TEST_BIN)
	./$(TEST_BIN)

# ----------- Generera coverage reports ------------
generate_coverage: clean
	$(MAKE) test EXTRA_CFLAGS="$(COVERAGE_FLAGS)"
	@mkdir -p $(COVDIR)
	gcov -b -o $(OBJDIR) $(LIB_SRC)
	mv *.gcov $(COVDIR)

# ----------------- Städning -----------------
clean:
	rm -rf $(OBJDIR)/*.o $(OBJDIR)/*.gcno $(OBJDIR)/*.gcda $(BINDIR) $(COVDIR)
