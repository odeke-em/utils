// Author: Emmanuel Odeke <odeke@ualberta.ca>
// Radix sort implementation

#include <stdio.h>
#include <stdlib.h>

#define raiseWarning(msg) {\
  fprintf(stderr, "%s [%s::%d]\033[31m%s\033[00m\n", \
	__FILE__, __func__, __LINE__, msg);\
}

typedef struct Node_ {
  int data;
  struct Node_ *next;
} Node;

inline void *allocMgr(ssize_t sz) {
  void *newMem = NULL;
  if (sz > 0) {
    newMem = malloc(sz);
  }

  if (newMem == NULL) {
    raiseWarning("Run-out of memory!");
    exit(-1);
  }

  return newMem;
}

inline Node *allocNode() { 
  return (Node *)allocMgr(sizeof(Node)); 
}

Node *pop(Node *src, int *storage) {
  if (src != NULL) {
    *storage = src->data;
    Node *prev = src;
    src = src->next;
    free(prev);
    prev = NULL;
  } else {
    printf("src is NULL\n");
  }

  return src;
}

Node *addNode(Node *src, const int data) {
  if (src != NULL) {
    Node *newN = allocNode();
    newN->data = data;
    newN->next = src;
    src = newN;
  } else {
    src = allocNode();
    src->data = data;
    src->next = NULL;
  }

  return src;
}

void freeSL(Node *src) {
  Node *tmp;
  while (src != NULL) {
    tmp = src->next;
    free(src);
    src = tmp;
  }

  src = NULL;
}

void printList(Node *src) {
  printf("[");
  Node *tmp = src;
  while (tmp != NULL) {
    printf(" %d ", tmp->data);
    tmp = tmp->next;
  }
  printf("]");
}

long long int reduce(Node *src) {
  long long int result = 0;
  Node *trav = src;
  while (trav != NULL) {
    result += trav->data;
    trav = trav->next;
  }

  return result;
}

Node *map(Node *src, int (*func)(const void *)) {
  Node *mapped = NULL;
  Node *trav = src;
  while (trav != NULL) {
    mapped = addNode(mapped, func(&trav->data));
    trav = trav->next;
  }

  return mapped;
}

int squareAsInts(const void *data) {
  if (data  != NULL) {
    int result = *(int *)data;
    return result * result;
  }

  return 0;
}

int getMax(Node *src) {
  int maxElem = -1;

  Node *n = src;
  while (n != NULL) {
    if (n->data > maxElem) maxElem = n->data;
    n = n->next;
  }

  return maxElem;
}

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

  for (i=20; i <= 370; ++i)
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
