#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DMap.h"
#include "errors.h"

ULInt pjwCharHash(const char *s) {
    ULInt h = 0;
    if (s != NULL) {
        ULInt g;
        while (*s != '\0') {
            h = (h << 4) + *s;
            g = h & 0xf0000000;
            if (g) {
                h ^= (g >> 24);
                h ^= g;
            }
            ++s;
        }
    }

    return h;
}

int main() {
    DMap *dm = newDMap();

    char *fDupd = strdup(__FILE__);
    ULInt h = pjwCharHash(fDupd);
    dm = pushDMap(dm, (void *)fDupd, h, 1);

    DNode *dn = getDMap(dm, h);
    assert((dn != NULL));
    printf("dn:data %s\n", (char *)dn->data);
    printf("dmSize: %d\n", dm->size);

    dm = popDMap(dm, h); 
    printf("dmSize: %d\n", dm->size);
    dm = destroyDMap(dm);

	return 0;
}
