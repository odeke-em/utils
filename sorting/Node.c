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

