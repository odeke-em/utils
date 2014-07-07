// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _DMAP_H
#define _DMAP_H

	#include "DNode.h"
	#include "HashMap.h"

	typedef struct {
		ULInt size;
		DNode *dnode;
		HashMap *hmap;
	} DMap;

    DMap *newDMap(void);
    inline DMap *allocDMap(void);

    DMap *pushDMap(DMap *dm, void *data, const ULInt h, const UInt allocStyle);
    DNode *getDMap(DMap *dm, const ULInt h);
    DMap *popDMap(DMap *dm, const ULInt h);
    DMap *destroyDMap(DMap *dm);

    ULInt pjwCharHash(const char *s);
    DMap *fileToDM(const char *path);

#endif // _DMAP_H
