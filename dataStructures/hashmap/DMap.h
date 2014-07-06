// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _DMAP_H
#define _DMAP_H
	#include "DNode.h"
	#include "HashMap.h"

	typedef struct {
		DNode *dnode;
		HashMap *hmap;
	} DMap;
#endif // _DMAP_H
