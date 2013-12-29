// Author: Emmanuel Odeke <odeke@ualberta.ca>

#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "hashlist/hashList.h"

void printSL(Element *e) {
  printf("{");
  if (e != NULL) {
    while (e != NULL) {
      if (e->value != NULL) printf(" %d", *(int *)e->value);
      e = e->next;
    }
  }
  printf("}");
}

Graph *allocGraph() {
  return (Graph *)malloc(sizeof(Graph));
}

Graph *initGraph(Graph *g) {
  if (g == NULL) {
    g = allocGraph();
  }

  g->vertexHeap = NULL;
  g->vertexToNeighbours = NULL;

  return g;
}

Graph *createGraph() {
  Graph *g = NULL;
  g = initGraph(g);

  return g;
}

Graph *destroyGraph(Graph *g) {
  if (g != NULL) {
    destroyHashList(g->vertexToNeighbours);
    destroyHeap(g->vertexHeap);
    free(g);
    g = NULL;
  }

  return g;
}

Element *getNeighbours(Graph *g, const int queryNode) {
  if (g == NULL) {
    return NULL;
  } else {
    Element **neighbours = get(g->vertexToNeighbours, queryNode);
    return *neighbours;
  }
}

void addEdge(Graph *g, const int start, const int end) {
  if (g == NULL) {
    g = initGraph(g);
  }

  if (g->vertexToNeighbours == NULL) {
    g->vertexToNeighbours = initHashList(g->vertexToNeighbours);
  }

  Element **srcNeighbours = get(g->vertexToNeighbours, start);
  if (*srcNeighbours == NULL) {
    int *endMem = (int *)malloc(sizeof(int));
    *endMem = end;
    insertElem(g->vertexToNeighbours, endMem, start); 
  } else {
    if (intPtrComp(&start, &end) == Equal) {
    #ifdef ALLOW_SELF_REFERENCES
      fprintf(stderr, "\033[94mAllowing self references\033[00m\n");
    #else
      return;
    #endif
    }

    int goodToAdd = 1;
    Element *trav = *srcNeighbours;
    while (trav != NULL) {
      if (intPtrComp(trav->value, &end) == Equal) {
	goodToAdd = 0;
	break;
      }
      trav = trav->next;
    }

    if (goodToAdd) {
      int *srcEndMem = (int *)malloc(sizeof(int));
      *srcEndMem = end;
      insertElem(g->vertexToNeighbours, srcEndMem, start);
    }
  }
}

Bool popVertex(Graph *g, const int value) {
  Element *popd = pop(g->vertexToNeighbours, value);
  Bool result = False;
  if (popd != NULL) {
    destroySList(popd);
    result = True;
  }

  return result;
}

int *intCopy(const void *d) {
  int *copy = NULL;
  if (d != NULL) {
    int *copy = (int *)malloc(sizeof(int));
    *copy = *(int *)d; 
    return copy;
  }
  return copy;
}

Element *getRoutes(Graph *g, const int src, const int end) {
  if (isEmptyRef((void **)get(g->vertexToNeighbours, src))) {
    // Should also check that the in edge exists
    return NULL;
  } else {
    if (src == end) return NULL;

    Heap *q = NULL;
    q = initHeap(q, intPtrComp, free);

    // Enqueue src itself
    heapInsert(q, intCopy(&src));

    Element *foundRoute = NULL, 
	    *possibleRoute = NULL;

    while (! isEmpty(q)) {
      int *headElem = NULL;
      heapExtract(q, (const void **)&headElem);
      Element *routeList = NULL;
      if (headElem != NULL) {
	if (intPtrComp(headElem, &end) == Equal) {
	  possibleRoute = addToTail(possibleRoute, intCopy(headElem), 0);
	#ifdef DEBUG
	  printf("Going to appendPossibleRoute\n");
	#endif
	  goto appendPossibleRoute;
	}
	Element *neighbours = getNeighbours(g, *headElem);
	if (neighbours != NULL) {
	  while (neighbours != NULL) {
	    heapInsert(q, intCopy(neighbours->value));

	    neighbours = neighbours->next;
	  }

	  possibleRoute = addToTail(possibleRoute, intCopy(headElem), 0);
	} else {
	  void *hSav = NULL;
	  if (possibleRoute != NULL 
	  && intPtrComp(headElem, possibleRoute->value) == Equal) {
	    possibleRoute = elemPop(possibleRoute, &hSav);
	  }

	  if (hSav != NULL) free(hSav);

	  if (possibleRoute == NULL)
	    possibleRoute = addToHead(possibleRoute, intCopy(&src));
	}
      }

      goto headElemCleanUp;

      appendPossibleRoute: {
	if (possibleRoute != NULL)
	  foundRoute = addToHead(foundRoute, possibleRoute);

	possibleRoute = NULL;
	possibleRoute = addToHead(possibleRoute, intCopy(&src));
      }

      headElemCleanUp: {
	if (headElem != NULL) {
	  free(headElem);
	  headElem = NULL;
	}
      }
    }

    cleanUp : {
      destroyHeap(q);
    }

    return foundRoute;
  }
}

Bool isEdge(Graph *g, const int src, const int dest) {
  Element **query = get(g->vertexToNeighbours, src);
  Bool existance = False;
  if (query != NULL) {
    Element *trav = *query;
    while (trav != NULL) {
      if (intPtrComp(trav->value, &dest) == Equal) {
	existance = True;
	break;
      }
      trav = trav->next;
    } 
  }

  return existance;
}

int main() {
  Graph *g = createGraph();
  addEdge(g, 1, 3);
  addEdge(g, 3, 2);
  addEdge(g, 1, 4);
  addEdge(g, 1, 1);
  addEdge(g, 2, 8);
  addEdge(g, 4, 9);
  addEdge(g, 9, 8);
  addEdge(g, 1, 6);
  addEdge(g, 6, 19);
  addEdge(g, 1, 7);
  addEdge(g, 7, 31);
  addEdge(g, 31, 38);
  addEdge(g, 38, 8);
  addEdge(g, 5, 19);
  addEdge(g, 19, 98);
  addEdge(g, 19, 18);
  addEdge(g, 19, 28);
  addEdge(g, 98, 88);
  addEdge(g, 88, 8);
 

  int src = 1, dest = 40;
  int end = 8;
  Element *xp = getRoutes(g, src, end);

  Element *trav = xp;
  printf("Trav %p\n", xp);
  while (trav != NULL) {
    Element *altRoute = (Element *)trav->value;
    printSL(altRoute);
    printf("\n");

    if (altRoute != NULL) {
      destroySList(altRoute);
    }

    Element *nextTmp = trav->next;
    free(trav);
    trav = nextTmp;
  }

  g = destroyGraph(g);
  return 0;
}
