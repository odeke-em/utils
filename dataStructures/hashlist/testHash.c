#include <stdio.h>
#include "hashList.h"

int main() {
  char *w[] = {"odeke\0", "odeka\0", "oedke\0", "emmanuelodekeiciamigo\0"};
  int i = sizeof(w)/sizeof(w[0]);
  while (--i > -1) {
    hashValue h = pjwCharHash(w[i]);
    printf("%s %d\n", w[i], h);
  }

  return 0;
}
