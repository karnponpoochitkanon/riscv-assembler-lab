#include "encode.h"

uint32_t encode_r(uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t rd, uint32_t opcode) {
    return ((funct7 & 0x7F) << 25) |
           ((rs2 & 0x1F) << 20) |
           ((rs1 & 0x1F) << 15) |
           ((funct3 & 0x7) << 12) |
           ((rd & 0x1F) << 7) |
           (opcode & 0x7F);
}

uint32_t encode_i(uint32_t imm12, uint32_t rs1, uint32_t funct3, uint32_t rd, uint32_t opcode) {
    return ((imm12 & 0xFFF) << 20) |
           ((rs1 & 0x1F) << 15) |
           ((funct3 & 0x7) << 12) |
           ((rd & 0x1F) << 7) |
           (opcode & 0x7F);
}

uint32_t encode_s(uint32_t imm12, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t opcode) {
    uint32_t imm_lo = imm12 & 0x1F;
    uint32_t imm_hi = (imm12 >> 5) & 0x7F;
    return (imm_hi << 25) |
           ((rs2 & 0x1F) << 20) |
           ((rs1 & 0x1F) << 15) |
           ((funct3 & 0x7) << 12) |
           (imm_lo << 7) |
           (opcode & 0x7F);
}

uint32_t encode_b(uint32_t imm13, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t opcode) {
    uint32_t imm12 = (imm13 >> 12) & 0x1;
    uint32_t imm11 = (imm13 >> 11) & 0x1;
    uint32_t imm10_5 = (imm13 >> 5) & 0x3F;
    uint32_t imm4_1 = (imm13 >> 1) & 0xF;

    return (imm12 << 31) |
           (imm10_5 << 25) |
           ((rs2 & 0x1F) << 20) |
           ((rs1 & 0x1F) << 15) |
           ((funct3 & 0x7) << 12) |
           (imm4_1 << 8) |
           (imm11 << 7) |
           (opcode & 0x7F);
}

uint32_t encode_j(uint32_t imm21, uint32_t rd, uint32_t opcode) {
    uint32_t imm20 = (imm21 >> 20) & 0x1;
    uint32_t imm10_1 = (imm21 >> 1) & 0x3FF;
    uint32_t imm11 = (imm21 >> 11) & 0x1;
    uint32_t imm19_12 = (imm21 >> 12) & 0xFF;

    return (imm20 << 31) |
           (imm10_1 << 21) |
           (imm11 << 20) |
           (imm19_12 << 12) |
           ((rd & 0x1F) << 7) |
           (opcode & 0x7F);
}
