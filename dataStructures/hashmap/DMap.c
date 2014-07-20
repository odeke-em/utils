// Author: Emmanuel Odeke <odeke@ualberta.ca>
// DataStructure that enables acts as a set ie no duplicates,
// yet at the same time in constant time allows: Get, Push, Pop and Put operations

#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <pthread.h>

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
	dm->size = 0;

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
		++dm->size;
	}

	return dm;
}

DMap *popDMap(DMap *dm, const ULInt h) {
	if (dm != NULL && dm->hmap != NULL) {
		DNode *popd = NULL;
		dm->hmap = pop(dm->hmap, h, NULL, (const void **)&popd);

		if (popd != NULL)
			--dm->size;

        DNode **ppd = &popd;
		popd = destroyLoneDNode(popd);
        *ppd = NULL;
	}

	return dm;
}

DNode *getDMap(DMap *dm, const ULInt h) {
	DNode *retr = NULL;
	if (dm != NULL && dm->hmap != NULL)
		retr = (DNode *)get(dm->hmap, h);

	return retr;
}

DMap *destroyDMap(DMap *dm) {
	if (dm != NULL) {
		dm->dnode = destroyDNode(dm->dnode);
		dm->hmap = destroyHashMap(dm->hmap);
		free(dm);
		memset(dm, 0, sizeof(*dm));
	}

	return dm;
}

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

DMap *fileToDM(const char *path) {
    printf("path: %s\n", path);
    DMap *dm = NULL;
    if (path != NULL) {
        pthread_mutex_t dmLock = PTHREAD_MUTEX_INITIALIZER;

        int fd = open(path, O_RDONLY);
        if (fd < 0)
            goto doneLoad;
            
        struct stat sb;
        if (stat(path, &sb))
            close(fd);
        else {
            long pageSize = sysconf(_SC_PAGESIZE);
            ULInt mapLen = ((sb.st_size + pageSize - 1)/pageSize) * pageSize;
            printf("mpLen: %ld pgSz: %ld\n", mapLen, pageSize);

            char *fBuf = mmap(NULL, mapLen, PROT_READ, MAP_SHARED, fd, 0);
            close(fd);
            printf("fBuf: %s\n", fBuf);

            if (fBuf == MAP_FAILED) // TODO: Descriptive error handling
                goto doneLoad;
            else {
                register ULInt i=0, j, sLen;
                unsigned int BUF_LEN = 10;
                while (i < sb.st_size) {
                    sLen = BUF_LEN;
                    char c, *s = (char *)malloc(sizeof(char) * sLen);
                    j = 0;
                    while (i < sb.st_size && (c = fBuf[i++]) != EOF && ! isspace(c)) {
                        if (j >= sLen) {
                            sLen += BUF_LEN;
                            s = (char *)realloc(s, sizeof(char) * sLen);
                            assert((s != NULL));
                        }

                        s[j++] = c;
                    }

                    if (! j)
                        free(s);
                    else {
                        s = (char *)realloc(s, sizeof(char) * (j + 1));
                        s[j] = '\0'; 
                        ULInt h = pjwCharHash(s);
                        pthread_mutex_lock(&dmLock);
                        sLen = dm != NULL ? dm->size: 0;
                        dm = pushDMap(dm, (void *)s, h, 1);

                        if (dm->size == sLen) // No insertion was made
                            free(s);

                        pthread_mutex_unlock(&dmLock);
                    }

                    ++i;
                }

                if (munmap(fBuf, mapLen)) {
                    raiseWarning(strerror(errno));
                }
            }
        }
    }

    doneLoad:
        return dm;
}
