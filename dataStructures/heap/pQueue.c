// Author: Emmanuel Odeke <odeke@ualberta.ca>
#include <stdio.h>
#include "pQueue.h"
#include "parcel.h"

inline void *peek(PQueue *p) {
  return isEmpty(p) ? NULL : p->tree[0];
}

Comparison parcelCompare(const void *pA, const void *pB) {
  if (pA == NULL) return pB == NULL ? Equal : Greater;

  const Parcel *cpA = (Parcel *)pA;
  const Parcel *cpB = (Parcel *)pB;
  if (cpA->cost != cpB->cost) {
    return cpA->cost < cpB->cost ? Less : Greater;
  } else {
    return Equal;
  }
}

int main() {
  Parcel parcels[] = {
    10, printParcel, Clear, 12, setDataCost, Clear,
    9, peek, Clear, 13, main, Clear, -1, initParcel, Clear
  };

  PQueue *pQ = NULL;
  pQ = initHeap(pQ, parcelCompare, NULL);
  int i, pCount = sizeof(parcels)/sizeof(parcels[0]);

  for (i=0; i < pCount; ++i) {
    heapInsert(pQ, &parcels[i]);
  }

#ifdef DEMO_EXTRACTION
  while (! isEmpty(pQ)) {
    const void *it = NULL;
    heapExtract(pQ, &it);
    printParcel((const Parcel *)it);
  } 
#else
  void **it = pQ->tree, **end = pQ->tree + pQ->size;
  while (it != end) {
    printParcel((const Parcel *)*it++);
  }

#endif
  destroyHeap(pQ);
  return 0;
}
