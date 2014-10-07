// Author: Emmanuel Odeke <odeke@ualberta.ca>

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

struct TupledStruct {
    uint64_t a, b, m, expected;
};

struct Cipher {
    uint64_t msgLen;
    char *key, *msg, *expected;
};


typedef struct TupledStruct ExpTestCase;
typedef struct TupledStruct MultTestCase;

typedef char * (*Cipherable) (char *, uint64_t, char *);
typedef uint64_t (*Modable) (uint64_t, uint64_t, uint64_t m);


void modableTestRunner(struct TupledStruct *cases, uint64_t caseLen, Modable f) {
    uint64_t i, tmp;
    struct TupledStruct tst;
    for (i = 0; i < caseLen; ++i) {
        tst = cases[i];
        tmp = f(tst.a, tst.b, tst.m);
        fprintf(stderr, "%d %d %d ", tst.a, tst.b, tst.m);
        if (tmp != tst.expected) {
            fprintf(stderr, "\033[31mExpected: %d instead got %d\033[00m\n", tst.expected, tmp);
        } else {
            fprintf(stderr, "\033[32mPassed!\033[00m\n");
        }
    }
}


void cipherableTestRunner(struct Cipher *cases, uint64_t caseLen, Cipherable f) { 
    uint64_t i;
    struct Cipher cst;
    char *result;
    for (i = 0; i < caseLen; ++i) {
        cst = cases[i];

        result = f(cst.msg, cst.msgLen, cst.key);
        fprintf(stderr, "%s %s ", cst.msg, cst.key);
        if (result == NULL) {
            if (cst.expected != NULL)
                fprintf(stderr, "\033[31mExpected: %s instead got NULL\033[00m\n", cst.expected);
            else
                fprintf(stderr, "\033[32mPassed!\033[00m\n");
        } else {
            if (strcmp(result, cst.expected) == 0)
                fprintf(stderr, "\033[32mPassed!\033[00m\n");
            else
                fprintf(
                   stderr, "\033[31mExpected: %s instead got %s\033[00m\n", cst.expected, result);

            free(result);
        }

    }
}


int main() {
    MultTestCase MTestCases[] = {
        {2, 3, 5, 1},
        {10, 40, 80, 0},
        {32, 1,  33, 32},
        {1111983375, 123458129,  (uint64_t)(~0) >> 1, 137283386956605375}
    };

    ExpTestCase ETestCases[] = {
        {10, 2, 2345, 100}, // 100 % 2345 == 100
        {11, 8, 91, 9}, // 214358881 % 91 == 9
        {1, 0, 10, 1},
        {20, 0, 1000, 1},
        {1111983375, 0,  2457273, 1}
    };

    uint64_t mcSz = sizeof(MTestCases)/sizeof(MTestCases[0]),
             ecSz = sizeof(ETestCases)/sizeof(ETestCases[0]);

    modableTestRunner(MTestCases, mcSz, fastMult);
    printf("\n\033[93mExponent test\033[00m\n");
    modableTestRunner(ETestCases, ecSz, fastExp);

    return 0;
}
