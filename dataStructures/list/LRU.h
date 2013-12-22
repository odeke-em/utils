#ifndef _LRU_H
#define _LRU_H
  #include "list.h"

  #define Cache LRU // Achieve some type name aliasing
  typedef List Cache;
  
  Cache *purgeLRU(Cache *c);

  Cache *setTagValue(Cache *c, unsigned int tagValue);

  Cache *accessMember(Cache *c, void *entry, Comparator comp);
#endif