#include <stdio.h>
#include "utils.h"

Comparison intPtrComp(const void *a, const void *b) {
  if (a == NULL) return a == b ? Equal : Greater;
  const int *aPtr = (int *)a;
  const int *bPtr = (int *)b;
  if (*aPtr != *bPtr) {
    return *aPtr < *bPtr ? Less : Greater;
  } else {
    return Equal;
  }
}

inline void swap(void **a, void **b) {
  void *tmp = *a;
  *a = *b;
  *b = tmp;
}
