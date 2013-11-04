// Author: Emmanuel Odeke <odeke@ualberta.ca>
// Just a kata/warm-up

#include <stdio.h>
#include <stdlib.h>

#define BUFSZ 40

void taC(FILE *ifp) {
  // Print lines as read from the in file, in reverse
  if (ifp == NULL) return;

  char c;

  int i=0, bufSize = BUFSZ;
  char *buf = (char *)malloc(sizeof(char)*bufSize);
  
  while ((c = getc(ifp)) != EOF) {
    if (i >= bufSize) {
      bufSize += BUFSZ;
      buf = (char *)realloc(buf, sizeof(char)*bufSize);
    }

    buf[i] = c;
    if (c == '\n') break;
    ++i;
  }

  if (c == EOF) {
    if (buf != NULL) free(buf);
    return;
  }

  taC(ifp);

  printf("%s", buf);
  free(buf);
  buf = NULL;
}

int main() {
  taC(stdin);
  return 0;
}
