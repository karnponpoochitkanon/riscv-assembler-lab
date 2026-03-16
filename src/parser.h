#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Parsed line representation.
 */
#define PARSER_MAX_OPERANDS 8
#define PARSER_MAX_TOKEN    64

typedef struct {
    char label[PARSER_MAX_TOKEN];      /* Optional label (without colon) */
    char mnemonic[PARSER_MAX_TOKEN];   /* Instruction mnemonic */
    char operands[PARSER_MAX_OPERANDS][PARSER_MAX_TOKEN];
    size_t operand_count;
} ParsedLine;

/**
 * Parse a single line of assembly.
 *
 * Accepts lines of the form:
 *   [label:] mnemonic [operand1[, operand2 ...]]
 *
 * @param line Input line (may be modified).
 * @param out Parsed result (written on success).
 * @return 1 if a valid instruction line was parsed, 0 if the line is blank/comment,
 *         or -1 on parse error.
 */
int parse_line(char *line, ParsedLine *out);

#ifdef __cplusplus
}
#endif

#endif // PARSER_H
