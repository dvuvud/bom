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
LIB_SRCS = $(wildcard $(SRCDIR)/*.c)
LIB_OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(LIB_SRCS))

TEST_SRCS = $(wildcard $(TESTDIR)/*.c)
TEST_BIN  = $(BINDIR)/unittests

# -------------- Standardmål ---------------
all: $(LIB_OBJS)

# --------- Bygg bibliotekets .o -----------
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ------------ Bygg testprogram -------------
$(TEST_BIN): $(LIB_OBJS) $(TEST_SRCS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ $(CUNIT) -o $@

# --------------- Kör tester -----------------
test: $(TEST_BIN)
	./$(TEST_BIN)

# ----------------- Städning -----------------
clean:
	rm -rf $(OBJDIR) $(BINDIR)
