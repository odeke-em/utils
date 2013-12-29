// Author: Emmanuel Odeke <odeke@ualberta.ca>

#ifndef _GRAPH_H
#define _GRAPH_H
  #include "list/list.h"
  #include "hashlist/hashList.h"
  #include "heap/heap.h"

  typedef struct Vertex_ {
    int value;
    List *neighbours;
  } Vertex;

  typedef struct Graph_ {
    Heap *vertexHeap;
    HashList *vertexToNeighbours;
  } Graph;

  Bool popVertex(Graph *g, const int value);
  Bool isEdge(Graph *g, const int src, const int dest);
  Element *getNeighbours(Graph *g, const int queryNode);
  void addEdge(Graph *g, const int start, const int end);

  Graph *allocGraph();
  Graph *createGraph();
  Graph *initGraph(Graph *g);
  Graph *destroyGraph(Graph *g);

  Element *getRoutes(Graph *g, const int src, const int end);

  int *intCopy(const void *d);
  void printSL(Element *e);
#endif
