#include <assert.h>
#include <stdio.h>

#include "../src/symbols.h"

int main(void) {
    assert(symbols_add("start", 0) == 0);
    assert(symbols_add("loop", 4) == 0);
    assert(symbols_add("end", 100) == 0);

    assert(symbols_find("start") == 0);
    assert(symbols_find("loop") == 4);
    assert(symbols_find("end") == 100);

    assert(symbols_find("missing") == -1);

    // Duplicate label is not allowed
    assert(symbols_add("loop", 8) == -1);

    puts("[PASS] test_symbols");
    return 0;
}
