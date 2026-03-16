#include <assert.h>
#include <stdio.h>

#include "../src/instr_table.h"

int main(void) {
    const Instruction *inst = instr_table_find("add");
    assert(inst != NULL);
    assert(inst->format == FMT_R);
    assert(inst->opcode == 0x33);
    assert(inst->funct3 == 0x0);
    assert(inst->funct7 == 0x00);

    inst = instr_table_find("sub");
    assert(inst != NULL);
    assert(inst->funct7 == 0x20);

    inst = instr_table_find("lw");
    assert(inst != NULL);
    assert(inst->format == FMT_I);
    assert(inst->opcode == 0x03);
    assert(inst->funct3 == 0x2);

    inst = instr_table_find("bgeu");
    assert(inst != NULL);
    assert(inst->format == FMT_B);
    assert(inst->opcode == 0x63);

    assert(instr_table_find("not_a_real_instr") == NULL);

    puts("[PASS] test_instr_table");
    return 0;
}
