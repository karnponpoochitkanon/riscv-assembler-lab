#include "assembler.h"

#include "encode.h"
#include "instr_table.h"
#include "parser.h"
#include "registers.h"
#include "symbols.h"

#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void errorf(char *errbuf, size_t errlen, const char *fmt, ...) {
    if (!errbuf || errlen == 0) {
        return;
    }
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(errbuf, errlen, fmt, ap);
    va_end(ap);
}

static int parse_imm(const char *s, int32_t *out) {
    if (!s || !*s) {
        return -1;
    }
    char *end;
    long v = strtol(s, &end, 0);
    if (*end != '\0') {
        return -1;
    }
    *out = (int32_t)v;
    return 0;
}

static int32_t parse_reg(const char *s) {
    int r = parse_register(s);
    return (r < 0) ? -1 : r;
}

int assemble(const char *lines[], size_t line_count, uint32_t *out, size_t *out_count, char *errbuf, size_t errlen) {
    if (!lines || !out || !out_count) {
        errorf(errbuf, errlen, "invalid arguments");
        return -1;
    }

    symbols_reset();

    // PASS 1: record labels
    size_t inst_index = 0;
    for (size_t i = 0; i < line_count; ++i) {
        char buf[256];
        strncpy(buf, lines[i], sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';

        ParsedLine pl;
        int r = parse_line(buf, &pl);
        if (r < 0) {
            errorf(errbuf, errlen, "parse error on line %zu", i + 1);
            return -1;
        }

        if (pl.label[0]) {
            int addr = (int)(inst_index * 4);
            if (symbols_add(pl.label, addr) != 0) {
                errorf(errbuf, errlen, "duplicate label '%s' on line %zu", pl.label, i + 1);
                return -1;
            }
        }

        if (r == 1) {
            inst_index++;
        }
    }

    // PASS 2: encode instructions
    inst_index = 0;
    for (size_t i = 0; i < line_count; ++i) {
        char buf[256];
        strncpy(buf, lines[i], sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';

        ParsedLine pl;
        int r = parse_line(buf, &pl);
        if (r != 1) {
            continue;
        }

        const Instruction *inst = instr_table_find(pl.mnemonic);
        if (!inst) {
            errorf(errbuf, errlen, "unknown instruction '%s' on line %zu", pl.mnemonic, i + 1);
            return -1;
        }

        uint32_t word = 0;
        uint32_t pc = (uint32_t)(inst_index * 4);

        switch (inst->format) {
            case FMT_R: {
                if (pl.operand_count != 3) {
                    errorf(errbuf, errlen, "expected 3 operands for %s on line %zu", pl.mnemonic, i + 1);
                    return -1;
                }
                int rd = parse_reg(pl.operands[0]);
                int rs1 = parse_reg(pl.operands[1]);
                int rs2 = parse_reg(pl.operands[2]);
                if (rd < 0 || rs1 < 0 || rs2 < 0) {
                    errorf(errbuf, errlen, "invalid register on line %zu", i + 1);
                    return -1;
                }
                word = encode_r(inst->funct7, (uint32_t)rs2, (uint32_t)rs1, inst->funct3, (uint32_t)rd, inst->opcode);
                break;
            }
            case FMT_I: {
                if (pl.operand_count != 3) {
                    errorf(errbuf, errlen, "expected 3 operands for %s on line %zu", pl.mnemonic, i + 1);
                    return -1;
                }
                int rd = parse_reg(pl.operands[0]);
                int rs1 = parse_reg(pl.operands[1]);
                int32_t imm;
                if (rd < 0 || rs1 < 0 || parse_imm(pl.operands[2], &imm) != 0) {
                    errorf(errbuf, errlen, "invalid operand for %s on line %zu", pl.mnemonic, i + 1);
                    return -1;
                }
                word = encode_i((uint32_t)imm, (uint32_t)rs1, inst->funct3, (uint32_t)rd, inst->opcode);
                break;
            }
            case FMT_S: {
                if (pl.operand_count != 2) {
                    errorf(errbuf, errlen, "expected 2 operands for %s on line %zu", pl.mnemonic, i + 1);
                    return -1;
                }
                int rs2 = parse_reg(pl.operands[0]);
                // operand format: imm(rs1)
                char *paren = strchr(pl.operands[1], '(');
                if (!paren) {
                    errorf(errbuf, errlen, "invalid operand format for %s on line %zu", pl.mnemonic, i + 1);
                    return -1;
                }
                *paren = '\0';
                char *endparen = strchr(paren + 1, ')');
                if (!endparen) {
                    errorf(errbuf, errlen, "invalid operand format for %s on line %zu", pl.mnemonic, i + 1);
                    return -1;
                }
                *endparen = '\0';
                int32_t imm;
                int rs1 = parse_reg(paren + 1);
                if (rs2 < 0 || rs1 < 0 || parse_imm(pl.operands[1], &imm) != 0) {
                    errorf(errbuf, errlen, "invalid operand for %s on line %zu", pl.mnemonic, i + 1);
                    return -1;
                }
                word = encode_s((uint32_t)imm, (uint32_t)rs2, (uint32_t)rs1, inst->funct3, inst->opcode);
                break;
            }
            case FMT_B: {
                if (pl.operand_count != 3) {
                    errorf(errbuf, errlen, "expected 3 operands for %s on line %zu", pl.mnemonic, i + 1);
                    return -1;
                }
                int rs1 = parse_reg(pl.operands[0]);
                int rs2 = parse_reg(pl.operands[1]);
                if (rs1 < 0 || rs2 < 0) {
                    errorf(errbuf, errlen, "invalid register on line %zu", i + 1);
                    return -1;
                }
                int32_t target = symbols_find(pl.operands[2]);
                if (target < 0) {
                    errorf(errbuf, errlen, "undefined label '%s' on line %zu", pl.operands[2], i + 1);
                    return -1;
                }
                int32_t imm = target - (int32_t)pc;
                word = encode_b((uint32_t)imm, (uint32_t)rs2, (uint32_t)rs1, inst->funct3, inst->opcode);
                break;
            }
            case FMT_J: {
                if (pl.operand_count != 2) {
                    errorf(errbuf, errlen, "expected 2 operands for %s on line %zu", pl.mnemonic, i + 1);
                    return -1;
                }
                int rd = parse_reg(pl.operands[0]);
                if (rd < 0) {
                    errorf(errbuf, errlen, "invalid register on line %zu", i + 1);
                    return -1;
                }
                int32_t target = symbols_find(pl.operands[1]);
                if (target < 0) {
                    errorf(errbuf, errlen, "undefined label '%s' on line %zu", pl.operands[1], i + 1);
                    return -1;
                }
                int32_t imm = target - (int32_t)pc;
                word = encode_j((uint32_t)imm, (uint32_t)rd, inst->opcode);
                break;
            }
            default:
                errorf(errbuf, errlen, "unsupported format for %s on line %zu", pl.mnemonic, i + 1);
                return -1;
        }

        out[inst_index++] = word;
    }

    *out_count = inst_index;
    return 0;
}
