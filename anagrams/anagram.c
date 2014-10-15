// Author: Emmanuel Odeke <odeke@ualberta.ca>
// Anagram checker

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"

#define ALPHA_OFFSET ('z' - 'a')

static int SPACE = -1;
static int ALPHABET_SIZE = ALPHA_OFFSET;

int alphabet_index(const char c) {
    if (isalpha(c))
        return tolower(c) - 'a';

    return -1;
}

unsigned int is_anagram(
        const char *base, const size_t b_len,
        const char *query, const size_t q_len) {

    register int i;
    unsigned int max_index = ALPHABET_SIZE;
    unsigned long int count_map[max_index];
    char *iter, *end;

    // Initialize all counts to zero
    for (i = 0; i < max_index; ++i)
        count_map[i] = 0;

    // Create the map
    for (iter = (char *)base, end = iter + b_len; iter < end; ++iter) {
        i = alphabet_index(*iter);
        if (i < 0)
            continue;

        count_map[i] += 1;
    }

    for (iter = (char *)query, end = iter + q_len; iter < end; ++iter) {
        i = alphabet_index(*iter);
        if (i < 0)
            continue;

        if (count_map[i] < 1)
            return 0;
        else
            count_map[i] -= 1;
    }

    for (i = 0; i < max_index; ++i) {
        if (count_map[i] != 0)
            return 0;
    }

    return 1;
}

typedef struct TestCase_t {
    char *b, *q;
    unsigned int expected:1;
} AnagramTestCase;

int main() {
    AnagramTestCase cases[] = {
        {.b = "", .q = "", .expected = 1},
        {.b = "goto", .q = "togo", .expected = 1},
        {.b = "Doormat", .q = "matdoor", .expected = 1},
        {.b = "", .q = "mat monad go first", .expected = 0},
        {.b = "industry", .q = "ind\0ustry", .expected = 0},
        {.b = "shakespeare", .q = "cheaspeake", .expected = 0},
        {.b = "Metathesis", .q = "It's the same", .expected = 1},
        {.b = "facebook list", .q = "cook fabelist", .expected = 1},
        {.b = "Martin Scorsese", .q= "Screen is a storm", .expected = 1},
        {.b = "Just because some of us can read and write and do a little math, "
              "that doesn't mean we deserve to conquer the universe.",
         .q = "A masquerade can cover a sense of what is real to deceive us; to be "
              "unjaded and not lost, we must, then, determine truth.",
         .expected = 1
        },
        {.b = "Internet Anagram Server", .q = "I, rearrangement servant", .expected = 1},
    }, t_case;

    unsigned int result;
    size_t t_len = sizeof(cases)/sizeof(cases[0]), i;
    for (i = 0; i < t_len; ++i) {
        t_case = cases[i];
        result = is_anagram(t_case.b, strlen(t_case.b), t_case.q, strlen(t_case.q));
        printf("'%s' is %s an anagram of '%s'\n", t_case.q, result ? "" : "not", t_case.b);
        assert(result == t_case.expected);
    }

    return 0;
}
