// Author: Emmanuel Odeke <odeke@ualberta.ca>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Node.h"

#define BUF_LEN  10
#define MAX_LINE_LENGTH 150

#define capsZADiff ('Z' - 'A')
#define lowerZADiff ('z' - 'a') + 1
#define _90Diff ('9' - '0')

#define ALPHABET_SIZE (capsZADiff + lowerZADiff + _90Diff + 1) // Extra space for unknown character

int indexResolve(const char c) {
  if (c >= 'A' && c <= 'Z') 
    return c - 'A';
  else if (c >= 'a' && c <= 'z')
    return capsZADiff + 1 + c - 'a';
  else if (c >= '0' && c <= '9')
    return capsZADiff + lowerZADiff + 1 + c - '0';
  else 
    return capsZADiff + lowerZADiff + _90Diff + 1;
}

void *maxByLen(Node *src) {
  void *maxElem = NULL;
  if (src != NULL) {
    int maxLength = 0, tmpLength;
    Node *trav = src;

    while (trav != NULL) {
      if (trav->data != NULL) {//By contract, the data has to be null terminated
	if ((tmpLength = strlen((char *)trav->data)/1) > maxLength) {
	  maxLength = tmpLength;
	  maxElem = trav->data;
	}
      }

      trav = trav->next;
    }
  }

  return maxElem;
}

unsigned int maxMetaTag(Node *src) {
  unsigned int maxLen = 0;
  Node *trav = src;
  while (trav != NULL) {
    if (trav->data != NULL && trav->metaTag > maxLen)
      maxLen = trav->metaTag;

    trav = trav->next;
  }
  return maxLen;
}

Node *bucketSort(Node *src, int (*indexResolve)(const char)) {
  Node **nodeBlock = (Node **)allocMgr(sizeof(Node *) * (ALPHABET_SIZE + 1));
  if (nodeBlock) {
    unsigned int maxLen = maxMetaTag(src);
    int idx;
    for (idx = maxLen; src != NULL && idx >= 0; --idx) {
      Node *trav = src;

      while (trav != NULL) {
	int bucketIndex = 0;
	if (idx <= trav->metaTag) {
	  bucketIndex = indexResolve(*((char *)(trav->data + idx)));
	}

	nodeBlock[bucketIndex] = \
	  addNode(nodeBlock[bucketIndex], trav->data, trav->metaTag);

	Node *tmp = trav->next;

	// Only freeing the node and not the data that will be moved around
	free(trav);
	trav = tmp;
      }

      src = NULL;
      int blockIndex;

      for (blockIndex = 0; blockIndex <= ALPHABET_SIZE; ++blockIndex) {
	while (nodeBlock[blockIndex] != NULL) {
	  int metaTag = (nodeBlock[blockIndex])->metaTag;
	  void *data = (nodeBlock[blockIndex])->data;
	  src = addNode(src, data, metaTag);

	  Node *next = (nodeBlock[blockIndex])->next;
	  free(nodeBlock[blockIndex]);
	  nodeBlock[blockIndex] = next;
	}
      }

    }

    free(nodeBlock);
  }

  return src;
}

int main(int argc, char *argv[]) {
#ifdef VIEW_ALPHABET_RESOLVE
  char *samp = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\0";
  char *trav = samp;

  while (*trav != '\0') {
    printf("%c %d\n", *trav, indexResolve(*trav));
    ++trav;
  }

  printf("alphabetSize: %d\n", ALPHABET_SIZE);
#endif

  FILE *ifp = stdin;
  if (argc >= 2) {
    ifp = fopen(argv[1], "r");
    if (ifp == NULL) {
      fprintf(stderr, "\033[91mCouldn't open %s for reading\033[00m\n", argv[1]);
      ifp = stdin;
    }
  }

  Node *fileContent = NULL;
  while (! feof(ifp)) {
    unsigned int bufLen = BUF_LEN;
    char *wordIn = (char *)malloc(sizeof(char) * bufLen);

    char c;
    int i = 0;
    while ((c = getc(ifp)) != EOF && i < MAX_LINE_LENGTH) {
      if (c == '\n') break;
      else if (i >= bufLen) {
	bufLen += BUF_LEN;
	wordIn = (char *)realloc(wordIn, sizeof(char) * bufLen);
      }

      wordIn[i++] = c;
    }

    if (i) {
      wordIn[i++] = '\0';
      wordIn = (char *)realloc(wordIn, sizeof(char) * i);
      fileContent = addNode(fileContent, wordIn, i);
    } else {
      free(wordIn);
    }
  }

  char *maxElem  = (char *)maxByLen(fileContent);
  fileContent = bucketSort(fileContent, indexResolve);

  Node *trav = fileContent;
  while (trav != NULL) {
    if (trav->data != NULL) {
      printf("%s\n", (char *)trav->data);
    }

    trav = trav->next;
  }

  freeSL(fileContent);
  fclose(ifp);
  return 0;
}
