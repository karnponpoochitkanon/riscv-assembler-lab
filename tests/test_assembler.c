#include <assert.h>
#include <stdio.h>

#include "../src/assembler.h"
#include "../src/encode.h"

int main(void) {
    const char *program[] = {
        "start: addi x1, x0, 1",
        "       addi x2, x0, 2",
        "loop:  beq x1, x2, end",
        "       addi x1, x1, 1",
        "       jal x0, loop",
        "end:   add x3, x1, x2",
    };

    uint32_t out[16];
    size_t out_count = 0;
    char errbuf[256];

    int ret = assemble(program, sizeof(program) / sizeof(program[0]), out, &out_count, errbuf, sizeof(errbuf));
    assert(ret == 0);
    assert(out_count == 6);

    // Calculate expected encodings manually.
    uint32_t expected[6];
    expected[0] = encode_i(1, 0, 0x0, 1, 0x13);   // addi x1, x0, 1
    expected[1] = encode_i(2, 0, 0x0, 2, 0x13);   // addi x2, x0, 2
    expected[2] = encode_b(12, 2, 1, 0x0, 0x63);  // beq x1, x2, end (pc=8 -> target=20 => offset=12)
    expected[3] = encode_i(1, 1, 0x0, 1, 0x13);   // addi x1, x1, 1
    expected[4] = encode_j(-8, 0, 0x6F);          // jal x0, loop (pc=16 -> target=8 => offset=-8)
    expected[5] = encode_r(0x00, 2, 1, 0x0, 3, 0x33); // add x3, x1, x2

    for (size_t i = 0; i < out_count; ++i) {
        assert(out[i] == expected[i]);
    }

    puts("[PASS] test_assembler");
    return 0;
}
