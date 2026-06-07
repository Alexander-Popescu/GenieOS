#include "test_util.h"
#include "../util/util.h"

/* strlength tests */
static bool test_strlength_empty() { return strlength("") == 0; }
static bool test_strlength_single() { return strlength("a") == 1; }
static bool test_strlength_multi() { return strlength("hello") == 5; }

/* compareString tests */
static bool test_compare_equal() { return compareString("abc", "abc"); }
static bool test_compare_diff_content() { return !compareString("abc", "xyz"); }
static bool test_compare_diff_length() { return !compareString("ab", "abc"); }
static bool test_compare_both_empty() { return compareString("", ""); }

/* intToAscii tests */
static bool test_itoa_zero() {
    char buf[12];
    intToAscii(0, buf);
    return compareString(buf, "0");
}

static bool test_itoa_single() {
    char buf[12];
    intToAscii(7, buf);
    return compareString(buf, "7");
}

static bool test_itoa_multi() {
    char buf[12];
    intToAscii(1234, buf);
    return compareString(buf, "1234");
}

/* memoryCopy tests */
static bool test_memcopy_basic() {
    char src[] = "hello";
    char dst[6] = {0};
    memoryCopy(src, dst, 6);
    return compareString(dst, "hello");
}

/* chrToLower / chrToUpper tests */
static bool test_tolower_upper() {
    char c = 'A';
    chrToLower(&c);
    return c == 'a';
}

static bool test_tolower_already() {
    char c = 'z';
    chrToLower(&c);
    return c == 'z';
}

static bool test_tolower_nonalpha() {
    char c = '5';
    chrToLower(&c);
    return c == '5';
}

static bool test_toupper_lower() {
    char c = 'a';
    chrToUpper(&c);
    return c == 'A';
}

static bool test_toupper_already() {
    char c = 'Z';
    chrToUpper(&c);
    return c == 'Z';
}

static bool test_toupper_nonalpha() {
    char c = '!';
    chrToUpper(&c);
    return c == '!';
}

/* isNumeric tests */
static bool test_isnumeric_digit() { return isNumeric('5'); }
static bool test_isnumeric_letter() { return !isNumeric('a'); }
static bool test_isnumeric_special() { return !isNumeric('#'); }

/* isAlpha tests */
static bool test_isalpha_upper() { return isAlpha('G'); }
static bool test_isalpha_lower() { return isAlpha('g'); }
static bool test_isalpha_digit() { return !isAlpha('3'); }
static bool test_isalpha_special() { return !isAlpha('@'); }

void testUtilSuite(TEST_CONTEXT *ctx) {
    TEST_CASE tests[] = {
        {"strlength: empty string",         test_strlength_empty},
        {"strlength: single char",          test_strlength_single},
        {"strlength: multi char",           test_strlength_multi},
        {"compareString: equal",            test_compare_equal},
        {"compareString: diff content",     test_compare_diff_content},
        {"compareString: diff length",      test_compare_diff_length},
        {"compareString: both empty",       test_compare_both_empty},
        {"intToAscii: zero",                test_itoa_zero},
        {"intToAscii: single digit",        test_itoa_single},
        {"intToAscii: multi digit",         test_itoa_multi},
        {"memoryCopy: basic",               test_memcopy_basic},
        {"chrToLower: uppercase",           test_tolower_upper},
        {"chrToLower: already lowercase",   test_tolower_already},
        {"chrToLower: non-alpha",           test_tolower_nonalpha},
        {"chrToUpper: lowercase",           test_toupper_lower},
        {"chrToUpper: already uppercase",   test_toupper_already},
        {"chrToUpper: non-alpha",           test_toupper_nonalpha},
        {"isNumeric: digit",                test_isnumeric_digit},
        {"isNumeric: letter",               test_isnumeric_letter},
        {"isNumeric: special",              test_isnumeric_special},
        {"isAlpha: uppercase",              test_isalpha_upper},
        {"isAlpha: lowercase",              test_isalpha_lower},
        {"isAlpha: digit",                  test_isalpha_digit},
        {"isAlpha: special",                test_isalpha_special},
    };
    runSuite(ctx, "util", tests, sizeof(tests) / sizeof(tests[0]));
}
