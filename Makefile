CC=gcc
CFLAGS=-Wall -Wextra -Isrc

.PHONY: all test_registers test_parser test_instr_table test_symbols

all: test_registers test_parser test_instr_table test_symbols

test_registers:
	$(CC) $(CFLAGS) tests/test_registers.c src/registers.c -o test_registers

test_parser:
	$(CC) $(CFLAGS) tests/test_parser.c src/parser.c -o test_parser

test_instr_table:
	$(CC) $(CFLAGS) tests/test_instr_table.c src/instr_table.c -o test_instr_table

test_symbols:
	$(CC) $(CFLAGS) tests/test_symbols.c src/symbols.c -o test_symbols
