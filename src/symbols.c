#include "symbols.h"

#include <string.h>

#define SYMBOLS_MAX 256
#define SYMBOLS_NAME_MAX 64

typedef struct {
    char name[SYMBOLS_NAME_MAX];
    int32_t addr;
} SymbolEntry;

static SymbolEntry g_symbols[SYMBOLS_MAX];
static size_t g_symbol_count = 0;

void symbols_reset(void) {
    g_symbol_count = 0;
}

int symbols_add(const char *name, int32_t addr) {
    if (!name || name[0] == '\0') {
        return -1;
    }

    // Prevent duplicates.
    for (size_t i = 0; i < g_symbol_count; ++i) {
        if (strcmp(g_symbols[i].name, name) == 0) {
            return -1;
        }
    }

    if (g_symbol_count >= SYMBOLS_MAX) {
        return -1;
    }

    strncpy(g_symbols[g_symbol_count].name, name, SYMBOLS_NAME_MAX - 1);
    g_symbols[g_symbol_count].name[SYMBOLS_NAME_MAX - 1] = '\0';
    g_symbols[g_symbol_count].addr = addr;
    g_symbol_count++;

    return 0;
}

int32_t symbols_find(const char *name) {
    if (!name) {
        return -1;
    }

    for (size_t i = 0; i < g_symbol_count; ++i) {
        if (strcmp(g_symbols[i].name, name) == 0) {
            return g_symbols[i].addr;
        }
    }

    return -1;
}
