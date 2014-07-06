// Author: Emmanuel Odeke <odeke@ualberta.ca>
// DataStructure that enables acts as a set ie no duplicates,
// yet at the same time in constant time allows: Get, Push, Pop and Put operations

#include <stdio.h>
#include <stdlib.h>

#include "DMap.h"
#include "DNode.h"
#include "HashMap.h"
#include "errors.h"

static const unsigned int HASH_RADIX = 10;

inline DMap *allocDMap(void) {
	return (DMap *)malloc(sizeof(DMap));
}

DMap *newDMap(void) {
	DMap *dm = allocDMap();
	assert((dm != NULL));	

	dm->hmap = newHashMap(HASH_RADIX);
	dm->dnode = NULL;

	return dm;
}

DMap *pushDMap(DMap *dm, void *data, const ULInt h, const UInt allocStyle) {
	if (dm == NULL)
		dm = newDMap();

	if (dm->hmap == NULL)
		dm->hmap = newHashMap(HASH_RADIX);

	void *retr = get(dm->hmap, h);

	if (retr == NULL) {
		dm->dnode = prependDNode(dm->dnode, data);
		dm->hmap = put(dm->hmap, h, dm->dnode, 0); // Enter it as isStackd
	}

	return dm;
}

DMap *popDMap(DMap *dm, void *data, const ULInt h) {
	if (dm != NULL && dm->hmap != NULL) {
		DNode *popd = NULL;
		dm->hmap = pop(dm->hmap, h, NULL, (const void **)&popd);
		popd = destroyLoneDNode(popd);
	}

	return dm;
}

DNode *getDMap(DMap *dm, const ULInt h) {
	DNode *retr = NULL;
	if (dm != NULL && dm->hmap != NULL)
		retr = (DNode *)get(dm->hmap, h);

	return retr;
}
