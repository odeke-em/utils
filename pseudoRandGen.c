/* 
  Author: Emmanuel Odeke <odeke@ualberta.ca>
  Theory for pseudo random generation obtained from:
    http://www.math.utah.edu/~pa/Random/Random.html
    Given p1, p2, N(limit), and x0
    Recurrance formula: xN+1 = (x0*p1) + p2 (mod N)
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct Tree_ {
  int data, p1, p2, mod;
  struct Tree_ *left, *right;
} Tree;

Tree *addLeaf(Tree *motherTree, const int data, int *cycleDetect) {
  if (*cycleDetect) return motherTree; 

  else if (motherTree == NULL) {
    motherTree = (Tree *)malloc(sizeof(Tree));
    motherTree->left = motherTree->right = NULL;
    motherTree->data = data;
  } else {
    if (motherTree->data != data) {
      if (motherTree->data < data)
	motherTree->left = addLeaf(motherTree->left, data, cycleDetect);
      else
	motherTree->right = addLeaf(motherTree->right, data, cycleDetect);
    } else {
      *cycleDetect = 1;
    }
  }

  return motherTree;
}

void printTree(Tree *motherTree) {
  if (motherTree == NULL) {
    return; 
  }
  
  printTree(motherTree->right); 
  printTree(motherTree->left); 
  printf(" %d ", motherTree->data);
}

void freeTree(Tree *motherTree) {
  if (motherTree == NULL) return;

  freeTree(motherTree->left); 
  freeTree(motherTree->right); 

  free(motherTree);
  motherTree = NULL;
}

Tree *pseudoRGen(const int p1, const int p2, const int x0, const int limit) {
  if (limit <= 0) return NULL;

  int cycleDetected = 0;
  int xN_1, xN = x0;
  Tree *storeTree = NULL;

  while (1) {
    xN_1 = ((p1*xN) + p2) % limit;

  #ifdef DEBUG
    printf("xN = %d * %d + %d (mod %d) = %d\n", xN, p1, p2, limit, xN_1);
  #endif

    storeTree = addLeaf(storeTree, xN_1, &cycleDetected);
    if (cycleDetected) break;
    xN = xN_1;
  }

  return storeTree;
}

int main() {
  Tree *pTree = pseudoRGen(263, 71, 79, 100);
  printTree(pTree);
  printf("\n");
  freeTree(pTree);
  return 0;
}
