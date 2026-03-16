#include <assert.h>
#include <stdio.h>

#include "../src/registers.h"

int main(void) {
    // Valid registers
    assert(parse_register("x0") == 0);
    assert(parse_register("x1") == 1);
    assert(parse_register("x5") == 5);
    assert(parse_register("x10") == 10);
    assert(parse_register("x31") == 31);

    // Invalid registers
    assert(parse_register(NULL) == -1);
    assert(parse_register("") == -1);
    assert(parse_register("x") == -1);
    assert(parse_register("x32") == -1);
    assert(parse_register("x-1") == -1);
    assert(parse_register("y0") == -1);
    assert(parse_register("x1a") == -1);
    assert(parse_register("x 1") == -1);

    puts("[PASS] test_registers");
    return 0;
}
