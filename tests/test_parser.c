#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../src/parser.h"

static void assert_parsed(const char *in,
                          const char *expect_label,
                          const char *expect_mnemonic,
                          const char *expect_ops[],
                          size_t expect_ops_count) {
    char buf[256];
    ParsedLine out;

    strncpy(buf, in, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    int res = parse_line(buf, &out);
    assert(res == 1);

    assert(strcmp(out.label, expect_label) == 0);
    assert(strcmp(out.mnemonic, expect_mnemonic) == 0);
    assert(out.operand_count == expect_ops_count);
    for (size_t i = 0; i < expect_ops_count; ++i) {
        assert(strcmp(out.operands[i], expect_ops[i]) == 0);
    }
}

static void assert_blank_or_comment(const char *in) {
    char buf[256];
    ParsedLine out;

    strncpy(buf, in, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    int res = parse_line(buf, &out);
    assert(res == 0);
}

int main(void) {
    const char *ops1[] = {"x1", "x2", "x3"};
    assert_parsed("loop: add x1, x2, x3", "loop", "add", ops1, 3);

    const char *ops2[] = {"x4", "x5"};
    assert_parsed("   sub   x4,   x5  ", "", "sub", ops2, 2);

    const char *ops3[] = {"x1", "label"};
    assert_parsed("jal x1, label", "", "jal", ops3, 2);

    assert_blank_or_comment("");
    assert_blank_or_comment("   \t  ");
    assert_blank_or_comment("# this is a comment");
    assert_blank_or_comment("   # comment with leading spaces");

    puts("[PASS] test_parser");
    return 0;
}
