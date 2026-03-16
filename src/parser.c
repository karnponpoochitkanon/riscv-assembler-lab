#include "parser.h"

#include <ctype.h>
#include <string.h>

static char *trim(char *s) {
    if (!s) return s;

    // Trim leading whitespace
    while (isspace((unsigned char)*s)) {
        s++;
    }

    // Trim trailing whitespace
    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)*(end - 1))) {
        end--;
    }
    *end = '\0';

    return s;
}

static int is_label_char(char c, int first) {
    if (first) {
        return (c == '_') || isalpha((unsigned char)c);
    }
    return (c == '_') || isalnum((unsigned char)c);
}

int parse_line(char *line, ParsedLine *out) {
    if (!line || !out) {
        return -1;
    }

    // Prepare output defaults
    out->label[0] = '\0';
    out->mnemonic[0] = '\0';
    out->operand_count = 0;

    // Remove leading/trailing whitespace and early-out on blank lines
    char *cur = trim(line);
    if (*cur == '\0') {
        return 0;
    }

    // Strip comments starting with '#'
    char *comment = strchr(cur, '#');
    if (comment) {
        *comment = '\0';
        cur = trim(cur);
        if (*cur == '\0') {
            return 0;
        }
    }

    // Check for label (ident:)
    char *colon = strchr(cur, ':');
    if (colon) {
        // Ensure label is at the start (no spaces before colon)
        char *label_end = colon;
        char *label_start = cur;
        // Validate label name
        if (!is_label_char(*label_start, 1)) {
            return -1;
        }
        for (char *p = label_start + 1; p < label_end; ++p) {
            if (!is_label_char(*p, 0)) {
                return -1;
            }
        }
        size_t label_len = (size_t)(label_end - label_start);
        if (label_len >= PARSER_MAX_TOKEN) {
            return -1;
        }
        memcpy(out->label, label_start, label_len);
        out->label[label_len] = '\0';

        cur = trim(colon + 1);
        if (*cur == '\0') {
            // Line contained only a label
            return 0;
        }
    }

    // Parse mnemonic
    char *tok_end = cur;
    while (*tok_end && !isspace((unsigned char)*tok_end)) {
        tok_end++;
    }
    size_t mnemonic_len = (size_t)(tok_end - cur);
    if (mnemonic_len == 0 || mnemonic_len >= PARSER_MAX_TOKEN) {
        return -1;
    }
    memcpy(out->mnemonic, cur, mnemonic_len);
    out->mnemonic[mnemonic_len] = '\0';

    // Parse operands if any
    cur = trim(tok_end);
    if (*cur == '\0') {
        return 1;
    }

    // Split operands by comma
    while (*cur && out->operand_count < PARSER_MAX_OPERANDS) {
        char *next = cur;
        while (*next && *next != ',') {
            next++;
        }

        // Temporarily terminate operand
        char saved = *next;
        *next = '\0';

        char *operand = trim(cur);
        if (*operand != '\0') {
            size_t len = strlen(operand);
            if (len >= PARSER_MAX_TOKEN) {
                return -1;
            }
            memcpy(out->operands[out->operand_count], operand, len + 1);
            out->operand_count++;
        }

        // Restore and move on
        *next = saved;
        cur = (saved == ',') ? trim(next + 1) : next;
    }

    return 1;
}
