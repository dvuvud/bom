.PHONY: memtest test generate_coverage clean all demo demo-original demo-data-tests demo-backend-tests demo-tests demo-original-tests
# ------- Kompilator och flaggor ---------
CC      = gcc
CFLAGS  = -Wall -pedantic -g -Isrc/include -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib
CUNIT   = -lcunit
COVERAGE_FLAGS = --coverage -o0

# -------------- Kataloger ----------------
SRCDIR  = src
TESTDIR = test
OBJDIR  = obj
BINDIR  = bin
COVDIR = docs/coverage

DEMOREF = demo/inlupp2-refmem
DEMONORM = demo/inlupp2-original

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

# ------------------ Kör demo --------------------
demo:
	$(MAKE) -C $(DEMOREF) valgrind-frontend

# --------------- Kör demo med test input ----------------
demo-tests:
	$(MAKE) -C $(DEMOREF) valgrind-frontend < $(DEMOREF)/Tests/webstoretest.txt

# ----------- Kör testerna i demo -----------
demo-backend-tests:
	$(MAKE) -C $(DEMOREF) valgrind

demo-data-tests:
	$(MAKE) -C $(DEMOREF) data-valgrind

# ------------------ Kör originalet --------------------
demo-original:
	$(MAKE) -C $(DEMONORM) valgrind-frontend

# ----------------- Kör originalet med test input ----------------
demo-original-tests:
	$(MAKE) -C $(DEMONORM) valgrind-frontend < $(DEMONORM)/Tests/webstoretest.txt

# ----------------- Städning -----------------
clean:
	rm -rf $(OBJDIR)/*.o $(OBJDIR)/*.gcno $(OBJDIR)/*.gcda $(BINDIR) $(COVDIR)
	$(MAKE) -C $(DEMOREF) clean
