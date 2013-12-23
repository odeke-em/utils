// Author: Emmanuel Odeke <odeke@ualberta.ca>
// Radix sort implementation

#include <stdio.h>
#include <stdlib.h>

#include "Node.h"

Node *radixSort(Node *src, const int radix) {
  Node **nodeBlock = (Node **)allocMgr(sizeof(Node *) * radix);
  int i;
  for (i=0; i < radix; ++i) {
    nodeBlock[i] = NULL;
  }

  int maxElem = getMax(src);
  int radIndex, movBase = 1;
  while (maxElem >= movBase) {
  #ifdef DEBUG
    printf("MovBase %d\n", movBase);
  #endif
    int tmpElem = 0;

    // Popping to buckets phase
    while (src != NULL) {
      src = pop(src, &tmpElem);
      radIndex = (tmpElem / movBase) % radix;
    #ifdef DEBUG
      printf("Inserting %d into bucket %d\n", tmpElem, radIndex);
    #endif
      nodeBlock[radIndex] = addNode(nodeBlock[radIndex], tmpElem);
    }

    // Re-Insertion phase
    for (i=0; i < radix; ++i) {
      int popRes = 0;
      while (nodeBlock[i] != NULL) {
        nodeBlock[i] = pop(nodeBlock[i], &popRes);
        src = addNode(src, popRes);
      #ifdef DEBUG
        printf("Popped from bucket: %d value %d\n", i, popRes);
      #endif
      }
      freeSL(nodeBlock[i]);
    }

    movBase *= radix;
  }

#ifdef DEBUG
  printf("MaxElem %d\n", maxElem);
#endif

  free(nodeBlock);
  nodeBlock = NULL;

  return src;
}

int main() {
  int i;
  Node *n = NULL;
  n = addNode(n, 10);
  n = addNode(n, 50);
  n = addNode(n, 1);
  n = addNode(n, 3);
  n = addNode(n, 8);

  for (i=20; i <= 37; ++i)
    n = addNode(n, i);

  n = addNode(n, 1);
  n = addNode(n, 123);
  n = addNode(n, 99);
  n = addNode(n, 8898);

  Node *mpd = map(n, squareAsInts);
  mpd = radixSort(mpd, 10);

  long long int reducedValue = reduce(n);
  printList(n);
  printf("\n");
  printList(mpd);
  printf("\n");
  freeSL(n);
  freeSL(mpd);
  printf("Reduced value : %lld\n", reducedValue);
  return 0;
}
