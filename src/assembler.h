#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Assemble an array of assembly source lines into machine code words.
 *
 * @param lines      Array of null-terminated strings (each line).
 * @param line_count Number of lines in the array.
 * @param out        Output buffer for encoded instructions.
 * @param out_count Pointer that receives how many words were written.
 * @param errbuf     Optional buffer to receive an error message.
 * @param errlen     Length of `errbuf` in bytes.
 * @return 0 on success, -1 on failure.
 */
int assemble(const char *lines[], size_t line_count, uint32_t *out, size_t *out_count, char *errbuf, size_t errlen);

#ifdef __cplusplus
}
#endif

#endif // ASSEMBLER_H
