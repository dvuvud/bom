# Kompilator
CC = gcc

# Kompilatorflaggor
CFLAGS = -Wall -g -fprofile-arcs -ftest-coverage -I../../src/include

# Länkningsflaggor för tester
LDLIBS = -lcunit

#Sökvägar
DATA_STRUCTURES = Data-structures
UTILS = Utils
BACKEND = Webstore-backend
FRONTEND = Webstore-frontend
TESTS = Tests

# Källfiler
MAIN_SRC = $(BACKEND)/Webstore_backend.c \
           $(DATA_STRUCTURES)/hash_table.c \
           $(DATA_STRUCTURES)/linked_list.c \
           $(DATA_STRUCTURES)/iterator.c \
           $(UTILS)/utils.c

TEST_SRC = $(TESTS)/backend_tests.c \

FRONTEND_SRC = $(FRONTEND)/Webstore_frontend.c \
               $(BACKEND)/Webstore_backend.c \
               $(DATA_STRUCTURES)/hash_table.c \
               $(DATA_STRUCTURES)/linked_list.c \
               $(DATA_STRUCTURES)/iterator.c \
               $(UTILS)/utils.c

DATA_TEST_SRC = $(DATA_STRUCTURES)/unit_tests.c \
                $(DATA_STRUCTURES)/hash_table.c \
                $(DATA_STRUCTURES)/linked_list.c \
                $(DATA_STRUCTURES)/iterator.c \
                $(UTILS)/utils.c \

#Objektfiler

MAIN_OBJ = $(MAIN_SRC:.c=.o) ../../obj/refmem.o ../../obj/queue.o ../../obj/hashset.o
TEST_OBJ = $(TEST_SRC:.c=.o) ../../obj/refmem.o ../../obj/queue.o ../../obj/hashset.o
FRONTEND_OBJ = $(FRONTEND_SRC:.c=.o) ../../obj/refmem.o ../../obj/queue.o ../../obj/hashset.o
DATA_TEST_OBJ = $(DATA_TEST_SRC:.c=.o) ../../obj/refmem.o ../../obj/queue.o ../../obj/hashset.o

#Körbara filer

TARGET = webstore_backend
TEST_TARGET = backend_tests
FRONTEND_TARGET = webstore_frontend
DATA_TEST_TARGET = data_structure_tests

#Standardmål

all: $(TEST_TARGET)

# Backend
$(TARGET): $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Testprogram
$(TEST_TARGET): $(MAIN_OBJ) $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(DATA_TEST_TARGET): $(DATA_TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Frontend
$(FRONTEND_TARGET): $(FRONTEND_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# =======================================
#       Test- och debug-mål
# =======================================

test: $(TEST_TARGET)
	./$(TEST_TARGET)

data-test: $(DATA_TEST_TARGET)
	./$(DATA_TEST_TARGET)

debug: $(TEST_TARGET)
	gdb ./$(TEST_TARGET)

data-valgrind: $(DATA_TEST_TARGET)
	valgrind --leak-check=full ./$(DATA_TEST_TARGET)

valgrind: $(TEST_TARGET)
	valgrind --leak-check=full ./$(TEST_TARGET)

valgrind-frontend: $(FRONTEND_TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(FRONTEND_TARGET)

coverage: $(TEST_TARGET)
	./$(TEST_TARGET)
	gcov -b -c $(MAIN_OBJ) $(TEST_OBJ)

# Kör frontend-programmet
run: $(FRONTEND_TARGET)
	./$(FRONTEND_TARGET)

#Rensning
clean:
	find . -type f \( -name "*.o" -o -name "*.gcda" -o -name "*.gcno" -o -name "*.gcov" \) -delete
	rm -f $(TARGET) $(TEST_TARGET) $(FRONTEND_TARGET) $(DATA_TEST_TARGET)
