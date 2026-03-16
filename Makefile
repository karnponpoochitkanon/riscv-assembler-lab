CC=gcc
CFLAGS=-Wall -Wextra -Isrc

.PHONY: all test_registers test_parser

all: test_registers test_parser

test_registers:
	$(CC) $(CFLAGS) tests/test_registers.c src/registers.c -o test_registers

test_parser:
	$(CC) $(CFLAGS) tests/test_parser.c src/parser.c -o test_parser
