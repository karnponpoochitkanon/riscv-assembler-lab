#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "../src/encode.h"

int main(void) {
    // R-type: add x1, x2, x3
    assert(encode_r(0x00, 3, 2, 0x0, 1, 0x33) == 0x003100b3);

    // I-type: addi x1, x2, 10
    assert(encode_i(10, 2, 0x0, 1, 0x13) == 0x00a10093);

    // S-type: sw x2, 8(x3)
    assert(encode_s(8, 2, 3, 0x2, 0x23) == 0x0021a423);

    // B-type: beq x1, x2, 16
    assert(encode_b(16, 2, 1, 0x0, 0x63) == 0x0208863);

    // J-type: jal x1, 2048 (0x800)
    assert(encode_j(2048, 1, 0x6F) == 0x001000ef);

    puts("[PASS] test_encode");
    return 0;
}
