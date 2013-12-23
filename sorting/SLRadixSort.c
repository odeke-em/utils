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
    // Popping to buckets phase
    while (src != NULL) {
      int *tmpElem = (int *)src->data;
      radIndex = (*tmpElem / movBase) % radix;
    #ifdef DEBUG
      printf("Inserting %d into bucket %d\n", *tmpElem, radIndex);
    #endif
      nodeBlock[radIndex] = addNode(nodeBlock[radIndex], tmpElem, *tmpElem);

      // We'll just be passing around the data and not freeing it
      // until it's original owner is freed later on
      Node *next = src->next;
      free(src);
      src = next;
    }

    // Re-Insertion phase
    for (i=0; i < radix; ++i) {
      while (nodeBlock[i] != NULL) {
	int *popRes = (nodeBlock[i])->data;
        src = addNode(src, popRes, *popRes);

	Node *next = (nodeBlock[i])->next;
	free(nodeBlock[i]);
	nodeBlock[i] = next;
      #ifdef DEBUG
        printf("Popped from bucket: %d value %d\n", i, *popRes);
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

void printIntList(Node *sl) {
  Node *trav = sl;
  printf("[ ");
  while (trav != NULL) {
    if (trav->data != NULL) {
      printf("%d ", *(int *)trav->data);;
    }

    trav = trav->next;
  }
  printf("]");
}

int main() {
  int i;
  Node *n = NULL;

  for (i=20; i <= 37; ++i) {
    int *tmp = (int *)malloc(sizeof(int));
    *tmp = i;
    n = addNode(n, tmp, i);
  }

  for (i=200; i < 400; ++i) {
    int *tp = (int *)malloc(sizeof(int));
    *tp = (i % 71);
    n = addNode(n, tp, *tp);
  }

  Node *mpd = map(n, square);
  mpd = radixSort(mpd, 10);

  long long int reducedValue = reduce(n);
  printf("Original list\n\033[95m");
  printIntList(n);
  printf("\n\033[00mSorted list\n\033[94m");
  printIntList(mpd);
  printf("\033[00m\n");
  freeSL(n);
  freeSL(mpd);
  printf("Reduced value : %lld\n", reducedValue);
  return 0;
}
