// Author: Emmanuel Odeke <odeke@ualberta.ca>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "list.h"

inline void *getData(const Node *n) { 
  return n == NULL ? NULL : n->data; 
}

inline void *getNextNode(const Node *n) { 
  return n == NULL ? NULL : n->next; 
}

inline int getListSize(const List *l) { 
  return l == NULL ? 0 : l->size; 
}

inline Node *allocNode(void) { 
  return (Node *)malloc(sizeof(Node)); 
}

Comparison intPtrComp(const void *i1, const void *i2) {
  if (i1 == NULL && i2 == NULL) return EQUAL;
  else if (i1 != NULL && i2 != NULL) {
    const int *localI1 = (int *)i1;
    const int *localI2 = (int *)i2;
    if (*localI1 != *localI2) return *localI1 < *localI2 ? LESSTHAN : GREATERTHAN;
    else return EQUAL;
  }

  return i1 == NULL ? LESSTHAN : GREATERTHAN;
}

Node *initNode(Node *n) {
  if (n != NULL) {
    n->data = NULL;
    n->next = NULL;
  }

  return n;
}

Node *createNewNode(void) {
  Node *newNode = allocNode();
  assert(newNode != NULL);
  
  newNode = initNode(newNode);
  return newNode;
}

List *initList(List *l) {
  if (l != NULL) {
    l->head = initNode(l->head);
    l->tail = initNode(l->tail);
    l->size = 0;
  }

  return l;
}

List *createNewList(void) {
  List *newList = allocList();
  assert(newList != NULL);
  
  newList = initList(newList);
  return newList;
}


inline List *allocList(void) {
  return (List *)malloc(sizeof(List));
}


List *append(List *l, void *data) {
#ifdef DEBUG
  printf("\033[32m%s\033[00m\n", __func__);
#endif

  if (l == NULL) {
    l = createNewList();
  }

  if (l->head == NULL) {
    // First item being added to the list
    l->head = createNewNode();
    l->head->data = data;
  } else {
    // Adding to the end
    Node *newEnd = createNewNode();
    newEnd->data = data;
    newEnd->next = l->head;
    l->head = newEnd;
  }

  l->tail = l->head;
  ++l->size;

  return l;
}

void destroyList(List *l) {
  if (l != NULL) {
    Node *start = l->head, *end = l->tail, *tmp;
    while (start != NULL) {
      tmp = start->next;
      if (start == NULL) break;

      if (start->data != NULL)  free(start->data);

      free(start);
      start = tmp;
    }

    free(l);
    l = NULL;
  }
}

void printList(List *l) {;
  printf("[");
  if (l != NULL) {
    Node *it = l->head, *end = l->tail;
    do {
      if (it == NULL) break;
      if (it->data  != NULL) printf(" %d ", *(int *)it->data);
      it = it->next;
    } while (it != end);
  }
  printf("]");
}

Node *find(List *l, void *query, Comparator matchFunc) {;
  Node *result = NULL;
  if (l != NULL && matchFunc != NULL) {
    Node *it = l->head, *end = l->tail;
    do {
      if (it == NULL) break;
    
      if (matchFunc(it->data, query) == EQUAL) {
	result = it;
	break;
      }
    } while (it != NULL);
  }

  return result;
}

List *removeElem(List *l, void *query, Comparator matchFunc) {
  if (l != NULL && matchFunc != NULL) {
    Node *prev = NULL, *cur = l->head;
    while (cur != NULL) {
      if (matchFunc(cur->data, query) == EQUAL) break;

      prev = cur;
      cur = cur->next;
    }

    if (prev != NULL) {
      prev->next = NULL;
      if (cur != NULL) {
	prev->next = cur->next;
	if (cur->data != NULL) free(cur->data);
	free(cur);
	cur = NULL;
      }
    }
  }
  return l;
}

int main() {
  int i;
  List *l = NULL;
  for (i=0; i < 10000; ++i) {
    int *tp = (int *)malloc(sizeof(int));
    *tp = i;
    l = append(l, tp);
  #ifdef DEBUG
    printf("Aprespend: %p tp: %p\n", l, tp);
  #endif
  }

  printList(l);
  printf("\n");

  for (i=2; i < 10000; ++i)
    l = removeElem(l, &i, intPtrComp);

  printList(l);
  printf("\n");
  destroyList(l);
  return 0;
}