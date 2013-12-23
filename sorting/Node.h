// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _NODE_H
#define _NODE_H

  #define raiseWarning(msg) {\
    fprintf(stderr, "%s [%s::%d]\033[31m%s\033[00m\n", \
        __FILE__, __func__, __LINE__, msg);\
  }

  typedef struct Node_ {
    int data;
    struct Node_ *next;
  } Node;

  void *allocMgr(ssize_t sz);

  void freeSL(Node *src);
  inline Node *allocNode();

  void printList(Node *src);

  Node *pop(Node *src, int *storage);
  Node *addNode(Node *src, const int data);

  int getMax(Node *src);

  long long int reduce(Node *src);
  int squareAsInts(const void *data);
  Node *map(Node *src, int (*func)(const void *));
  
#endif
