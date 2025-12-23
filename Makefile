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
LIB_SRCS = $(wildcard $(SRCDIR)/*.c)
LIB_OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(LIB_SRCS))

TEST_SRCS = $(wildcard $(TESTDIR)/*.c)
TEST_BIN  = $(BINDIR)/unittests

# -------------- Standardmål ---------------
all: $(LIB_OBJS)

# --------- Bygg bibliotekets .o -----------
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c $< -o $@

# ------------ Bygg testprogram -------------
$(TEST_BIN): $(LIB_OBJS) $(TEST_SRCS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $^ $(CUNIT) $(LDFLAGS) -o $@

# --------------- Kör tester -----------------
test: $(TEST_BIN)
	./$(TEST_BIN)

# --------- Kör tester med Valgrind ----------
memtest: $(TEST_BIN)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_BIN)

# ----------- Generera coverage reports ------------
generate_coverage: clean
	$(MAKE) test EXTRA_CFLAGS="$(COVERAGE_FLAGS)"
	@mkdir -p $(COVDIR)
	gcov -b -o $(OBJDIR) $(LIB_SRCS)
	mv *.gcov $(COVDIR)

# ----------------- Städning -----------------
clean:
	rm -rf $(OBJDIR)/*.o $(OBJDIR)/*.gcno $(OBJDIR)/*.gcda $(BINDIR) $(COVDIR)
