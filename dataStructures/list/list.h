// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _LIST_H
#define _LIST_H
  typedef enum {
    Less=-1, Equal=0, Greater=1
  } Comparison;

  typedef struct Node_ {
    void *data;
    struct Node_ *next;
    unsigned int tag:1;
    int (*freeData)(void *data);
  } Node;

  typedef struct List_ {
    unsigned int size;
    Node *head, *tail;
  } List;

  typedef Comparison (*Comparator)(const void *, const void *);

  Node *initNode(Node *);
  Node *createNewNode(void);
  inline Node *allocNode(void);

  List *initList(List *);
  List *createNewList(void);
  inline List *allocList(void);

  inline void *getData(const Node *n);
  inline void *getNextNode(const Node *n);
  inline unsigned int getListSize(const List *l);
  void *pop(Node **n);

  inline void *peek(const List *l);
  List *append(List *l, void *data);
  List *prepend(List *l, void *data);
  Node *find(List *l, void *query, Comparator comp);
  List *removeElem(List *l, void *query, Comparator comp);

  void destroyList(List *l);

  // Miscellaneous
  void printList(List *l);
  Comparison intPtrComp(const void *i1, const void *i2);

  void swap(void **a, void **b);
  inline int isEmpty(const List *l);
#endif
