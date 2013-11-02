// Author: Emmanuel Odeke <odeke@ualberta.ca>

#include <stdio.h>
#define DEBUG

int wholeMultiplier(const int subject, const int query) {
  // Returns the closest whole number that query can be multiplied
  // to get the subject eg: 5 = 2 * 2 + 1
  int closestWhole = 1;
  int multResult = query;
  if (query > subject) return 0;
  else if (query == subject) return 1;

  while (1) {
    multResult += query;
    if (multResult > subject) break;

    ++closestWhole;
  }

  return closestWhole;
}

void gcd(int b, int a) {
  if (a <= 0 || b <= 0) return;
  if (a > b) return;
  
  int multipFactor = wholeMultiplier(b, a),
      residue = b - (a * multipFactor); 

#ifdef DEBUG
  printf("%d - %d * %d = %d\n", b, a, multipFactor, residue);
#endif

  gcd(a, residue);
}

int main() {
  int a = 60534384, b = 35717371;
  gcd(a, b);

  return 0;
}
