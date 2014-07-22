// Author: Emmanuel Odeke <odeke@ualberta.ca>

#include <stdio.h>
#include <string.h>

#include "DMap.h"
#include "errors.h"


int main(int argc, char **argv) {
    char *target = __FILE__;
    if (argc >= 2)
        target = argv[1];

    DMap *dm = fileToDM(target);

    char *fDupd = strdup(__func__);
    ULInt h = pjwCharHash(fDupd);
    char *cast = (char *)dm;

    ULInt ownH = pjwCharHash(cast);
    void *prevEvac = putDMap(dm, fDupd, h, 1);
    char *ft = __FILE__;
    printf("evac: %s fDup: %s h: %ld\n", (char *)prevEvac, fDupd, ownH);
    prevEvac = putDMap(dm, ft, pjwCharHash(ft), 0);
    printf("pE: %p\n", prevEvac);

    void *retr = getDMap(dm, h);
    printf("retr: %p\n", retr);
    assert((retr != NULL));
    printf("dn:data %s\n", (char *)retr);
    printf("dmSize: %ld\n", dm->size);

    void *evac = NULL;
    dm = popDMap(dm, h, (const void **)&evac);
    printf("dmSize: %ld evacuated: %s\n", dm->size, (char *)evac);

    evac = NULL;
    dm = popDMap(dm, h, (const void **)&evac);
    printf("dmSize: %ld evacuated: %s\n", dm->size, (char *)evac);
    dm = destroyDMap(dm);

    return 0;
}
