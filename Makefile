CC=gcc
CFLAGS=-Wall -Wextra -Isrc

.PHONY: test_registers

test_registers:
	$(CC) $(CFLAGS) tests/test_registers.c src/registers.c -o test_registers
