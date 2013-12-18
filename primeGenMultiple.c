#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "primeGenMultiple.h"

boolean isPrime(uint64 testNum) {
   if (testNum <= 1) { 
     return False;
   } else if (! (testNum & 1)) {//For an even number the least significant bit is cleared ie '0'
            return testNum == 2;
   } else if (testNum == 3 || testNum == 5 || testNum == 7) {
     return True;
   }

   uint64 sqrtNum = sqrt(testNum), nIncr=2; 

   /*
    Loop through all odd numbers less than or equal to the square root and 
    return False if any multiple is found in that range 
   */
   for (nIncr=3; nIncr <= sqrtNum; nIncr+=2) {
      if (! (testNum % nIncr))
         return False;
   }

   return True;
}


uint64 *primeGen(uint32 n) {
  /*
   Return the first n primes by: 
   + First adding '2' to the primeList.
   +  From then, test the next n-1 odd numbers
   and if prime, add them to the list.
  */
  printf("Running algorithm in: %s\n", __func__);
  uint64 *primeList = (uint64 *)malloc(sizeof(uint64)*n), curOddNum = 3;

  uint32 primeIndex;   
  primeList[0] = 2;

  for (primeIndex=1; primeIndex < n;) {
    if (isPrime(curOddNum)) {
       primeList[primeIndex++] = curOddNum;
    }

    curOddNum +=2;
  }

  printf(
    "Done generating: %d primes. Returning array of primes now\n", n 
  );
  
  return primeList;
}

uint64 *sieve(uint64 limit) {
    /*
     * Add '2' to the list, then add odd numbers until the 'limit'
     * Take out all multiples of each untampered number, by marking
       them off with a distinct marker such as a negative number. 
     * Keeping track of verified and added primes is done by maintaining 
       a counter that will only be incremented iff a prime has been added.
     * Once all primes till the 'limit' have been sieved through, truncate
       the list at the last nth added index.
     */
    printf("Running algorithm: %s\n", __func__);
    uint64 primeOffset = 2,
	baseNum, targNum,
	*numList = (uint64*)malloc(sizeof(uint64)*limit);

    uint32 cIndex, nIndex = 0,idx1, idx2, cOddNumber = 1;

    boolean divOverFlow;  
    cIndex = 0;
    numList[cIndex] = primeOffset; //The first prime is '2'. 
    cIndex++;

    /*From then on, prime numbers are odd numbers, hence only odd 
    numbers will now be added to 'intlist'*/
    
    for (; cIndex < limit; cIndex++){ 
      cOddNumber += primeOffset;
      numList[cIndex] = cOddNumber;
    }

    for (idx1 = 0; idx1 < cIndex; idx1++){
      baseNum = numList[idx1];
      if (baseNum == NONPRIME_MARKER)
        continue;

      for (idx2 = idx1+1; idx2<cIndex; idx2++){
        targNum = numList[idx2];
        if (targNum == NONPRIME_MARKER)
          continue;

        divOverFlow= targNum%baseNum; 

        /*Mark all multiples of 'baseNum' with 
        the non-prime identifier 'NONPRIME_MARKER*/
        if (! divOverFlow) 
          numList[idx2] = NONPRIME_MARKER;
      }

      numList[nIndex] = numList[idx1];
      nIndex++;
    }
    /* Truncate the list of primes by the index of the
       the most recent registered prime number */
    numList = (uint64*)realloc(numList, sizeof(uint64)*nIndex);

    /* Print each prime number if visual verification needed */
    #ifdef DEBUG
    for (cIndex = 0; cIndex < nIndex ; cIndex++)
      fprintf(stderr, "%ld\n", numList[cIndex]);
    #endif

    printf(
       "Done generating: %d primes. Returning array of primes now\n",nIndex
    );

    return numList;
}

int main(int argc, char *argv[]) {
  uint32 n = 1000;
  FILE *ofp = stdout;

  if (argc >= 2) {
    if (sscanf(argv[1], "%d", &n) != 1) {
       n = 1000;
    }

    if (argc >= 3) {
	ofp = fopen(argv[2], "w");
    }

    if (ofp == NULL) // Reverting back to our standard out
       ofp = stdout;
  }

  printf("\033[94mGenerating the first: %d primes\033[00m\n", n);

  uint64 *primeList = primeGen(n);

  int i=0;
  fprintf(ofp, "%-20s %30s\n", "Index:", "Value");

  for (i = 0; i < n; ++i) {
     fprintf(ofp, "%-20d %30ld\n", i+1, primeList[i]);
  }

  if (primeList != NULL) free(primeList);

  fclose(ofp);
  return 0;
}
