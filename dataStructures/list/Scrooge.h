#ifndef _SCROOGE_H
#define _SCROOGE_H
  #include "LRU.h"

  typedef struct {
    LRU *consumerList;
    unsigned int consumerCount,
		 maxCapacity;
    void (*workGenerate)(void *);
  } Producer;

  Producer *allocProducer();
  Producer *initProducer(Producer *prod, const unsigned int capacity);

  Producer *insertJob(Producer *prod, void *data);
#endif
