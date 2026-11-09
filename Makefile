CC=g++
CFLAGS=-std=c++23 -Wall -Wextra
TEST_CFLAGS=-g
LDFLAGS=
TEST_LDFLAGS=-lgtest -lgtest_main -lpthread

emulator: object/main.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

test_emulator: object/test_memory.o object/test_z80.o object/test_main.o
	$(CC) $(CFLAGS) $(TEST_CFLAGS) $^ $(LDFLAGS)$(TEST_LDFLAGS) -o $@

object/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS) -o $@

object/test_main.o: src/test/main.cpp
	$(CC) $(CFLAGS) $(TEST_CFLAGS) -c $^ $(LDFLAGS)$(TEST_LDFLAGS) -o $@

object/test_memory.o: src/test/memory.cpp
	$(CC) $(CFLAGS) $(TEST_CFLAGS) -c $^ $(LDFLAGS)$(TEST_LDFLAGS) -o $@

object/test_z80.o: src/test/z80.cpp
	$(CC) $(CFLAGS) $(TEST_CFLAGS) -c $^ $(LDFLAGS)$(TEST_LDFLAGS) -o $@

.PHONY: clean
clean:
	@rm object/*
