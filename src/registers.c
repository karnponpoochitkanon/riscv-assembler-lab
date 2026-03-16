#include "registers.h"

int parse_register(const char *name) {
    if (!name || name[0] != 'x') {
        return -1;
    }

    int reg = 0;
    int idx = 1;

    if (name[idx] == '\0') {
        return -1;
    }

    for (; name[idx] != '\0'; idx++) {
        char c = name[idx];
        if (c < '0' || c > '9') {
            return -1;
        }
        reg = reg * 10 + (c - '0');
        if (reg > 31) {
            return -1;
        }
    }

    return reg;
}
