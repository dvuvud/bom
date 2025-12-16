# ------- Kompilator och flaggor ---------
CC      = gcc
CFLAGS  = -Wall -pedantic -g -I./src/include
CUNIT   = -lcunit

# -------------- Kataloger ----------------
SRCDIR  = src
TESTDIR = test
OBJDIR  = obj
BINDIR  = bin

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
	$(CC) $(CFLAGS) -c $< -o $@

# ------------ Bygg testprogram -------------
$(TEST_BIN): $(LIB_OBJ) $(TEST_SRC)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ $(CUNIT) $(LDFLAGS) -o $@

# --------------- Kör tester -----------------
test: $(TEST_BIN)
	./$(TEST_BIN)

# ----------------- Städning -----------------
clean:
	rm -rf $(OBJDIR) $(BINDIR)