#include "assembler.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source.s>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "failed to open '%s': %s\n", path, strerror(errno));
        return 1;
    }

    char **lines = NULL;
    size_t line_count = 0;
    char *line = NULL;
    size_t cap = 0;

    while (getline(&line, &cap, f) != -1) {
        // Remove trailing newline
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        char *copy = strdup(line);
        if (!copy) {
            fprintf(stderr, "out of memory\n");
            fclose(f);
            return 1;
        }
        char **tmp = realloc(lines, (line_count + 1) * sizeof(char *));
        if (!tmp) {
            fprintf(stderr, "out of memory\n");
            free(copy);
            fclose(f);
            return 1;
        }
        lines = tmp;
        lines[line_count++] = copy;
    }

    free(line);
    fclose(f);

    uint32_t *out = malloc(line_count * sizeof(uint32_t));
    size_t out_count = 0;
    char errbuf[256];

    if (assemble((const char **)lines, line_count, out, &out_count, errbuf, sizeof(errbuf)) != 0) {
        fprintf(stderr, "assembly failed: %s\n", errbuf);
        for (size_t i = 0; i < line_count; ++i) free(lines[i]);
        free(lines);
        free(out);
        return 1;
    }

    for (size_t i = 0; i < out_count; ++i) {
        printf("%08x\n", out[i]);
    }

    for (size_t i = 0; i < line_count; ++i) free(lines[i]);
    free(lines);
    free(out);

    return 0;
}
