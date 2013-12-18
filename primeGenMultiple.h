 /* 
   Author: Emmanuel Odeke <odeke@ualberta.ca>
   	Prime number generation algorithms.

     Two related but slightly different algorithms for generating prime numbers:
     1. sieve algorithm: Start by filling a list with '2' and then odd numbers.
          For each odd number find and mark out its multiples in the list, these will later be overridden and taken out
     2. Iterative Check-as-you-go algorithm: For every odd number starting from '3'(after including '2'), check if it
          is a prime number and add it to the list, if it is a prime
 */
#ifndef _PRIME_SIEVE_H
#define _PRIME_SIEVE_H
    #define NONPRIME_MARKER 1    
    typedef unsigned int uint32;
    typedef unsigned long uint64;

    typedef enum {
        False=0,
        True=1
    } boolean;

    uint64 *primeGen(uint32 n);
    uint64 *sieve(uint64 limit);
    boolean isPrime(uint64 testNum);
#endif
