CC=g++
CFLAGS=-std=c++23 -Wall -Wextra
TEST_CFLAGS=-g
LDFLAGS=
TEST_LDFLAGS=-lgtest -lgtest_main -lpthread

emulator: object/main.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

test_emulator: object/test_main.o
	$(CC) $(CFLAGS) $(TEST_CFLAGS) $^ $(LDFLAGS)$(TEST_LDFLAGS) -o $@

object/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c $^ $(LDFLAGS) -o $@

object/test_main.o: src/test/main.cpp
	$(CC) $(CFLAGS) $(TEST_CFLAGS) -c $^ $(LDFLAGS)$(TEST_LDFLAGS) -o $@

.PHONY: clean
clean:
	@rm object/*
