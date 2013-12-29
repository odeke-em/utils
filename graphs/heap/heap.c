// Author: Emmanuel Odeke <odeke@ualberta.ca> 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "heap.h"

inline Heap *newHeap() {
  return (Heap *)malloc(sizeof(Heap));
}

inline void *qpeek(Heap *p) {
  return isEmpty(p) ? NULL : p->tree[0];
}

inline int getSize(Heap *h) {
  return h == NULL || h->tree == NULL ? 0 : h->size;
}

Heap *initHeap(Heap *h, Comparator comp, Destructor destroy) {
  if (h == NULL) {
    h = newHeap();
  }

  h->size = 0;
  h->tree = NULL;
  h->compare = comp;
  h->destroy = destroy;

  return h;
}

Heap *heapify(Heap *h, const int targetIndex) {
  if (targetIndex >= 0 && targetIndex <= h->size) {
    Comparator comp = h->compare;
    int endIndex = targetIndex, parentIndex = getParent(endIndex);
    while (1) {
      if (comp(h->tree[endIndex], h->tree[parentIndex]) == Greater) {
      #ifdef DEBUG
	printf("swapping\n");
      #endif
	swap(h->tree + endIndex, h->tree + parentIndex);
      } else {
	break;
      }

      endIndex = parentIndex;
      parentIndex = getParent(endIndex);
    }
  }

  return h;
}

int heapAppend(Heap *h, void *data) {
  if (h != NULL) {
    if (h->tree == NULL) {
      h->size = 0;
      h->tree = (void **)malloc(sizeof(void *));
    } else {
      h->tree = (void **)realloc(h->tree, sizeof(void *) * (h->size + 1));
    }

    if (h->tree != NULL) {
      h->tree[h->size] = data;
      ++h->size;
      return 0;
    } else {
      return -1;
    }
  }
}

int heapInsert(Heap *h, void *data) {
  if (h != NULL) {
    int extraSize = h->size + 1;
    if (h->tree == NULL) {
      h->tree = (void **)malloc(sizeof(void *) * extraSize);
    } else {
      h->tree = (void **)realloc(h->tree, sizeof(void *) * extraSize);
    }

    if (h->tree == NULL) return -1;
    h->tree[h->size] = data;
    h = heapify(h, h->size);
    h->size = extraSize;
    return 0;
  } else {
    fprintf(stderr, "Can't insert into null heap\n");
    return -1;
  }
}

int heapExtract(Heap *h, const void **storage) {
  if (h == NULL || h->tree == NULL) {
    return -1;
  } else {
    void *headNode = h->tree[0];
    *storage = headNode; 
    int decrSz = h->size - 1;
    if (decrSz > 0) { // When there is more than one node present
      void *lastNode = h->tree[decrSz];
      h->tree = (void **)realloc(h->tree, sizeof(void *) * decrSz);
      h->tree[0] = lastNode;
      --h->size;

      // Push the contents of the new top downward
      int lPos, rPos, markedPos,
	  curPos = 0;
      Comparator comp = h->compare; 
      while (1 && curPos < decrSz) {
	lPos = leftChild(curPos);
	rPos = rightChild(curPos);

	if (lPos < decrSz && comp(h->tree[lPos], h->tree[curPos]) == Greater) {
	  markedPos = lPos;
	} else {
	  markedPos = curPos;
	}

	if  (rPos < decrSz
	  && comp(h->tree[rPos], h->tree[markedPos]) == Greater
	) {
	  markedPos = rPos;
	}

	// When the marked position is the current positon,
	// heap property has been restored
	if (markedPos == curPos) {
	  break;
	} else {
	  swap(h->tree + markedPos, h->tree + curPos);
	}

	// Continue heapifying by moving another level down
	curPos = markedPos;
      }
    } else { // Last element
      h->size = 0;
      free(h->tree);
      h->tree = NULL;
    }

    return 0;
  }
}

Heap *destroyHeap(Heap *h) {
  if (h != NULL) {
    if (h->tree != NULL) {
      if (h->destroy != NULL) {
	void **it = h->tree, 
	   **end = it + h->size;
	while (it != end) {
	  if (*it != NULL)
	    h->destroy(*it);
	  ++it;
	}
      }

      free(h->tree);
    }

    h->size = 0;
    free(h);
  }

  return h;
}

void printHeap(Heap *h) {
  printf("[ ");
  if (h != NULL && h->tree != NULL) {
    void **it = h->tree,
	 **end = it + h->size;
    while (it != end) {
      printf("%d ", *(int *)*it++);
    }
  }
  printf("]\n");
}

#ifdef SAMPLE_RUN
int main() {
  Heap *h = NULL;
  h = initHeap(h, intPtrComp, free);
  int i, *apt;

  for (i=0; i < 10000; ++i) {
    apt = (int *)malloc(sizeof(int));
    *apt = i;
    heapInsert(h, apt);
  }

  printHeap(h);
#ifdef DEMO_EXTRACTION
  while (! isEmpty(h)) {
    int *tp = NULL;
    heapExtract(h, (const void **)&tp);
    printf("tp: %d\n", *tp);
    free(tp);
  }
#endif

  destroyHeap(h);
  return 0;
}
#endif
