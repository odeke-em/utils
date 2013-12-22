#include <stdio.h>
#include <stdlib.h>
#define DEBUG

#include "Scrooge.h"

#define PRODUCER_CAPACITY 20
#define DEFAULT_PRODUCER_CAPACITY 10

Producer *allocProducer() {
  return (Producer *)malloc(sizeof(Producer));
}

Producer *initProducer(Producer *prod, const unsigned int capacity) {
  if (prod == NULL) {
    prod = allocProducer();
  }

  prod->consumerCount = 0;
  prod->consumerList = NULL;
  prod->workGenerate = NULL;
  prod->maxCapacity = capacity;

  return prod;
}

Producer *destroyProducer(Producer *prod) {
  if (prod != NULL) {
    if (prod->consumerList != NULL) {
      destroyList(prod->consumerList);
      prod->consumerList = NULL;
    }

    free(prod);
    prod = NULL;
  }

  return prod;
}

void *generateData(void) {
  return NULL;
}

Producer *insertJob(Producer *prod, void *data) {
#ifdef DEBUG
  printf("\033[33mIn %s\033[00m\n", __func__);
#endif
  if (prod == NULL) {
    prod = initProducer(prod, DEFAULT_PRODUCER_CAPACITY);
  }

  if (prod->consumerCount < prod->maxCapacity) {
  #ifdef DEBUG
    printf("\033[32mInserting element %d\033[00m\n", prod->consumerCount);
  #endif
    prod->consumerList = prepend(prod->consumerList, data);
    ++prod->consumerCount;
  } else { // Time to purge folks
  #ifdef DEBUG
    printf("Purging now %p\n", prod->consumerList);
  #endif
    prod->consumerList = purgeLRU(prod->consumerList);

    if (prod->consumerList == NULL) {
      prod->consumerCount = 0;
    } else {
      prod->consumerCount = getListSize(prod->consumerList);
    }

    // Try again
    prod = insertJob(prod, data);
  }

  return prod;
}

int main() {
  Producer *prod = NULL;
  prod = initProducer(prod, PRODUCER_CAPACITY);

  int i;
  for (i=0; i < 30; ++i) {
    prod = insertJob(prod, malloc(sizeof(int)));
  }

  prod = destroyProducer(prod);
  return 0;
}
