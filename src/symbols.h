#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Simple array-backed symbol table.
 *
 * Symbols map a name to a 32-bit address.
 */

/**
 * Add a symbol to the table.
 *
 * @param name Label name (null-terminated).
 * @param addr Address associated with the label.
 * @return 0 on success, -1 on failure (duplicate or table full).
 */
int symbols_add(const char *name, int32_t addr);

/**
 * Find a symbol address by name.
 *
 * @param name Label name to search for.
 * @return Address on success, -1 if not found.
 */
int32_t symbols_find(const char *name);

#ifdef __cplusplus
}
#endif

#endif // SYMBOLS_H
