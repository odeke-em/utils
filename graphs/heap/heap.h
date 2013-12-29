// Author: Emmanuel Odeke <odeke@ualberta.ca> 
#ifndef _HEAP_H
#define _HEAP_H
  // #include "../utils.h"

  typedef struct Heap_ {
    int size;
    void **tree;
    Comparator compare; 
    Destructor destroy;
  } Heap;

  #define getParent(i) (i - 1)/2
  #define leftChild(i) (2 * i + 1)
  #define rightChild(i) (2 * i + 2)
  #define isEmpty(h) (h == NULL || h->tree == NULL || h->size == 0)

  Heap *newHeap();
  Heap *initHeap(Heap *h, Comparator comp, Destructor destroy);

  int heapInsert(Heap *h, void *data);
  int heapExtract(Heap *h, const void **storage);
  Heap *heapify(Heap *h, const int targetIndex);

 
  inline void *qpeek(Heap *p); 
  inline int getSize(Heap *h);

  Heap *destroyHeap(Heap *h);
  Heap *heapSort(Heap *h);
  int heapAppend(Heap *h, void *data);
#endif
