#ifndef _UTILS_H
#define _UTILS_H

#ifdef ALIEN_ARCHITECTURE
    typedef signed long long int64_t;
    typedef unsigned long long int uint64_t;
#endif // ALIEN_ARCHITECTURE

    uint64_t fastExp(uint64_t a, uint64_t b, uint64_t m);

    uint64_t fastMult(uint64_t a, uint64_t b, uint64_t m);
#endif // _UTILS_H
