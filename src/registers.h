#ifndef REGISTERS_H
#define REGISTERS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Parse a register name (e.g. "x5") into its register number.
 *
 * @param name Register name (must be "x0".."x31").
 * @return Register number [0..31], or -1 on invalid input.
 */
int parse_register(const char *name);

#ifdef __cplusplus
}
#endif

#endif // REGISTERS_H
