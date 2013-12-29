#ifndef _UTILS_H_
#define _UTILS_H
  typedef enum Comparison_ {
    Less=-1, Equal=0, Greater=1
  } Comparison;

  typedef enum Tag_ {
    Unknown=-1, Stackd=0, Heapd=1
  } Tag;

  typedef void (*Destructor)(void *data);
  typedef Comparison (*Comparator)(const void *, const void *);
  Comparison intPtrComp(const void *a, const void *b);
  inline void swap(void **a, void **b);
#endif
