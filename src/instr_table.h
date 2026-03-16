#ifndef INSTR_TABLE_H
#define INSTR_TABLE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    FMT_R,
    FMT_I,
    FMT_S,
    FMT_B,
    FMT_U,
    FMT_J,
} InstrFormat;

/**
 * Instruction metadata derived from the CS61C RV32I reference card.
 */
typedef struct {
    const char *name;
    InstrFormat format;
    uint32_t opcode;
    uint32_t funct3;
    uint32_t funct7;
} Instruction;

/**
 * Lookup an instruction by mnemonic.
 *
 * @param name Instruction mnemonic (lowercase).
 * @return Pointer to instruction metadata, or NULL if not found.
 */
const Instruction *instr_table_find(const char *name);

#ifdef __cplusplus
}
#endif

#endif // INSTR_TABLE_H
