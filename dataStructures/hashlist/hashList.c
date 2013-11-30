// Author: Emmanuel Odeke <odeke@ualberta.ca>
// HashList implementation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "hashList.h"

#ifdef DEBUG
  #undef DEBUG
#endif

#define HANDLE_COLLISIONS

inline Bool hasNext(Element *e) { return e->next != NULL; }

inline Element *getNext(Element *e) { return e == NULL ? NULL : e->next; }

Element *addToTail(Element *sl, void *data, const Bool overWriteOnDup) {
  if (sl == NULL) {
    sl = initElement(sl);
    sl->value = data;
  } else if (sl->value != data) {
    sl->next = addToTail(sl->next, data, overWriteOnDup);
  } else {
    if (overWriteOnDup) {
      sl->value = data;
    } else {
      //Do something interesting eg store number of visits
    }
  }

  return sl;
}

Element *initElement(Element *elem) {
  if (elem == NULL) {
    elem = (Element *)malloc(sizeof(Element));
  } 

  elem->next = NULL;
  elem->value = NULL;

  return elem;
}

HashList *initHashList(HashList *hl) {
  if (hl == NULL) {
    hl = (HashList *)malloc(sizeof(HashList));
    raiseExceptionIfNull(hl);
  }

  // All attributes are set to NULL
  hl->size = INIT_HASH_LIST_SIZE;
#ifdef DEBUG
  printf("%s:: %d\n", __func__, hl->size);
#endif
  if ((hl->list = (Element **)malloc(sizeof(Element *) * hl->size)) == NULL) {
    free(hl);
    raiseError(
     "Run out of memory, trying to create space for a hashlist's list attribute"    );
  }

  return hl;
}

void insertElem(HashList *hl, void *data, const hashValue hashCode) {
  if (hl == NULL) {
    raiseError("Cannot add elements to a NULL hashList");
  } 

  if (hl->size == 0) {
    printf("HashList size is zero, initializing it now\n");
    initHashList(hl);
    assert(hl->size != 0);
  }

  int elemIndex = hashCode % hl->size;
#ifdef DEBUG
  printf("HashCode: %d Element at Index: %p\n", hashCode, hl->list[elemIndex]);
#endif
  if (hl->list[elemIndex] == NULL) { // We've found the first entry matching that hash
    hl->list[elemIndex] = initElement(hl->list[elemIndex]);
    hl->list[elemIndex]->value = data;
  } else {
    #ifdef HANDLE_COLLISIONS
      // Always update to the latest value
      hl->list[elemIndex] = addToTail(hl->list[elemIndex], data, True);
    #else 
      hl->list[elemIndex]->value = data;
    #endif
  }
}

Element *get(HashList *hl, hashValue hashCode) {
  return (hl == NULL || ! hl->size || hl->list == NULL) \
	  ? NULL : hl->list[hashCode % hl->size];
}

void destroySList(Element *sl) {
  if (sl != NULL) { 
    // printf("Sl == NULL: %d\n", sl != NULL);
    Element *tmp;
    while (sl != NULL) { 
      tmp = sl->next;
    #ifdef DEBUG
      printf("Freeing:: curHead: %p Next: %p\n", sl, tmp);
    #endif
      if (sl->value != NULL) {
	free(sl->value);
      }

      free(sl);

      sl = tmp;
    }
    sl = NULL;
  }

}

void destroyHashList(HashList *hl) {
  if (hl != NULL) {
    int i;
    if (hl->list != NULL) {
      for (i=0; i < hl->size; ++i) {
	destroySList(hl->list[i]);
      }

      free(hl->list);
      hl->list = NULL;
    }

    free(hl);
    hl = NULL;
  }
}

int main() {
  HashList *hl = NULL;
  char *tmp = (char *)malloc(4);
  hl = initHashList(hl);

  int i;
  for (i=0; i < 1000000; i++) {
    int *x = (int *)malloc(sizeof(int));
    *x = i;
    insertElem(hl, x, i);
  }

  printf("hl %p\n", hl);

  Element *found = get(hl, 101);  

  while (hasNext(found)) {
    found = getNext(found);
    printf("%d\n", *((int *)found->value));
  }

  insertElem(hl, tmp, 2);
  destroyHashList(hl);
  return 0;
}
