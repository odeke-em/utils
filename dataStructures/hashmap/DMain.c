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

    DNode *dn = getDMap(dm, h);
    assert((dn != NULL));
    printf("dn:data %s\n", (char *)dn->data);
    printf("dmSize: %d\n", dm->size);

    dm = popDMap(dm, h); 
    printf("dmSize: %d\n", dm->size);
    dm = destroyDMap(dm);

	return 0;
}
