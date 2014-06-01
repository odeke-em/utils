// Author: Emmanuel Odeke <odeke@ualberta.ca>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "radTrie.h"

static unsigned int BASE = 10;

inline RTrie *allocRTrie(void) {
    return (RTrie *)malloc(sizeof(RTrie));
}

RTrie *newRTrie(void) {
    RTrie *rt = allocRTrie();
    assert(rt);

    rt->EOS = 0;
    rt->keys = NULL;
    rt->value = NULL;
    rt->availBlock = 0;
    return rt;
}

RTrie *put(RTrie *rt, unsigned long int hash, void *data, Bool heapBool) {
    if (rt == NULL) {
        rt = newRTrie();
    }

    if (rt->keys == NULL) {
        rt->keys = (RTrie **)malloc(sizeof(RTrie *) * BASE);
    }

    unsigned int residue = hash % BASE;
    unsigned int bitPos = 1 << residue;
    hash /= BASE;

    if (! (rt->availBlock & bitPos)) {
        rt->availBlock |= bitPos;
        // printf("bP: %d residue: %d hash: %ld avB: %d\n", bitPos, residue, hash, rt->availBlock);
        *(rt->keys + residue) = NULL;
    }

    if (hash || residue) {
        *(rt->keys + residue) = put(*(rt->keys + residue), hash, data, heapBool);
    }
    else {
        // TODO: Define what to do if the value was already present
        // printf("setting eos for %p\n", data);
        rt->EOS = 1;
        rt->value = data;
        rt->valueIsHeapd = heapBool;
    }

    return rt;
}

void *access(RTrie *rt, unsigned long int hash, Bool isGetOp) {
    if (rt == NULL)
        return NULL;
    else {
        unsigned int residue = hash % BASE, bitPos;
        bitPos = 1 << residue;
        hash /= BASE;
        if (! (rt->availBlock & bitPos))
            return NULL;
        else {
            if (hash || residue)
                return access(*(rt->keys + residue), hash, isGetOp);
            else {
                // printf("EOS: %d data: %p\n", rt->EOS, rt->value);
                if (rt->EOS) {
                    if (isGetOp)
                        return rt->value;
                    else {
                        void *popd = rt->value;
                        rt->value = NULL;
                        rt->EOS = False;
                        return popd;
                    }
                }
                else
                    return NULL;
            }
        }
    }
}

void *get(RTrie *rt, unsigned long int hash) {
    return access(rt, hash, True);
}

void *pop(RTrie *rt, unsigned long int hash) {
    return access(rt, hash, False);
}

RTrie *destroyRTrie(RTrie *rt) {
    if (rt != NULL) {
        if (rt->keys != NULL) {
            unsigned int i=0, bitPos;
            while (i < BASE) {
                bitPos = 1<<i;
                if (rt->availBlock & bitPos) {
                    // printf("bitPos: %d avB: %d\n", bitPos, rt->availBlock);
                    *(rt->keys + i) = destroyRTrie(*(rt->keys + i));
                }
                ++i;
            }

            free(rt->keys);
            rt->keys = NULL;
        }

        if (rt->valueIsHeapd && rt->value != NULL) {
            free(rt->value);
            rt->value = NULL;
        }

        // printf("Freeing rt: %p\n", rt);
        free(rt);
        rt = NULL;
    }

    return rt;
}

int main() {
    RTrie *t = NULL;
    char *duped = strdup("Aloha ola and bonjour\0");
    
    t = put(t, 20, (void *)duped, 1);
    void *gotten = get(t, 20);
    assert(gotten != NULL);

    void *popd = pop(t, 20);
    gotten = get(t, 20);
    assert(gotten == NULL);

    t = destroyRTrie(t);
    return 0;
}
