#include <stdlib.h>
#include <stdio.h>

#include "Node.h"

void *allocMgr(ssize_t sz) {
  void *newMem = NULL;
  if (sz > 0) {
    newMem = malloc(sz);

    if (newMem == NULL) {
      raiseWarning("Run-out of memory!");
      exit(-1);
    }
  }

  return newMem;
}

inline Node *allocNode() {
  return (Node *)allocMgr(sizeof(Node));
}

Node *pop(Node *src, void *storage) {
  if (src != NULL) {
    storage = src->data;
    Node *prev = src;
    src = src->next;
    free(prev);
    prev = NULL;
  } else {
    printf("Popping from NULL src\n");
  }

  return src;
}

Node *addNode(Node *src, void *data, int metaTag) {
  Node *newN = allocNode();
  newN->data = data;
  newN->next = src;
  newN->metaTag = metaTag;
  src = newN;

  return src;
}

void freeSL(Node *src) {
  Node *tmp;
  while (src != NULL) {
    tmp = src->next;
    if (src->data != NULL) free(src->data);
    free(src);
    src = tmp;
  }

  src = NULL;
}

void printList(Node *src) {
  printf("{");
  Node *tmp = src;
  while (tmp != NULL) {
    if (tmp->data != NULL)
      printf(" %s ", (char *)(tmp->data));
    #ifdef SHOW_LENGTH
      printf(" %d ", tmp->metaTag);
    #endif
      printf("\n");
    tmp = tmp->next;
  }
  printf("}");
}


long long int reduce(Node *src) {
  long long int result = 0;
  Node *trav = src;
  while (trav != NULL) {
    if (trav->data != NULL)
      result += *(int *)trav->data;

    trav = trav->next;
  }

  return result;
}

Node *map(Node *src, void *(*func)(const void *)) {
  Node *mapped = NULL;
  Node *trav = src;
  while (trav != NULL) {
    mapped = addNode(mapped, func(trav->data), 0);
    trav = trav->next;
  }

  return mapped;
}

void *square(const void *data) {
  int *result = NULL;
  if (data != NULL) {
    result = (int *)malloc(sizeof(int));
    *result = *(int *)data;
    *result *= *result;
  }

  return result;
}

int getMax(Node *src) {
  int maxElem = -1;

  Node *n = src;
  while (n != NULL) {
    if (n->data != NULL) {
      int curData = *(int *)n->data; 
      if (curData > maxElem) maxElem = curData;
    }

    n = n->next;
  }

  return maxElem;
}

