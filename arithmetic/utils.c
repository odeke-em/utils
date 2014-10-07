// Author: Emmanuel Odeke <odeke@ualberta.ca>

#include <stdio.h>
#include <stdlib.h> /* uint*_t */

#include "utils.h"

uint64_t fastMult(uint64_t a, uint64_t b, uint64_t m) {
    // Avoid overflow
    // Complexity: O(log2(a))
    // Idea is iterate over all the bits of a
    //  ie  (an * b + an-1 * b + .... + a1 * b + a0 * b) % m
    //  ==  ((an * b) % m + (an-1 * b) %m  + .... + (a1 * b) % m + ((a0 * b) % m))
    
    uint64_t result = 0;
    uint64_t pow2Sav = 1;

    b %= m;

    while (a) {
        if (a & 1) {
            result = (result + ((pow2Sav * b) % m)) % m;
        }

        a >>= 1;
        pow2Sav = (pow2Sav * 2) % m;
    }

    return result;
}


uint64_t fastExp(uint64_t a, uint64_t b, uint64_t m) {
    // Avoid overflow
    // Complexity: O(log2(b) * log2(a))
    
    uint64_t result = 1, v = a % m;
    while (b) {
        if (b & 1) {
            result = (result * v) % m;
        }

        v = fastMult(v, v, m);
        b >>= 1;
    }

    return result;
}


#ifdef UTILS_MAIN
int main() {
    return 0;
}
#endif
