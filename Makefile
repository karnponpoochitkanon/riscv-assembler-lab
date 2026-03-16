CC=gcc
CFLAGS=-Wall -Wextra -Isrc

SRC=src/main.c src/assembler.c src/parser.c \
    src/registers.c src/instr_table.c \
    src/symbols.c src/encode.c

.PHONY: all assembler clean test_registers test_parser test_instr_table test_symbols test_encode test_assembler

all: assembler

assembler:
	$(CC) $(CFLAGS) $(SRC) -o assembler

clean:
	rm -f assembler

# unit tests

test_registers:
	$(CC) $(CFLAGS) tests/test_registers.c src/registers.c -o test_registers

test_parser:
	$(CC) $(CFLAGS) tests/test_parser.c src/parser.c -o test_parser

test_instr_table:
	$(CC) $(CFLAGS) tests/test_instr_table.c src/instr_table.c -o test_instr_table

test_symbols:
	$(CC) $(CFLAGS) tests/test_symbols.c src/symbols.c -o test_symbols

test_encode:
	$(CC) $(CFLAGS) tests/test_encode.c src/encode.c -o test_encode

test_assembler:
	$(CC) $(CFLAGS) tests/test_assembler.c src/assembler.c src/encode.c src/parser.c src/instr_table.c src/registers.c src/symbols.c -o test_assembler
