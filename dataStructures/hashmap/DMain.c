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
    dm = pushDMap(dm, (void *)fDupd, h, 1);

    void *retr = getDMap(dm, h);
    assert((retr != NULL));
    printf("dn:data %s\n", (char *)retr);
    printf("dmSize: %ld\n", dm->size);

    void *evac = NULL;
    dm = popDMap(dm, h, (const void **)&evac);
    printf("dmSize: %ld evacuated: %s\n", dm->size, (char *)evac);
    free(evac);

    dm = popDMap(dm, h, (const void **)&evac);
    printf("dmSize: %ld\n", dm->size);
    dm = destroyDMap(dm);

    return 0;
}
