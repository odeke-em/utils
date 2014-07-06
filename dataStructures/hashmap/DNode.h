// Author: Emmanuel Odeke <odeke@ualberta.ca>
#ifndef _DNODE_H
#define _DNODE_H
    typedef struct DNode_ {
        void *data;
        struct DNode_ *prev, *next;
    } DNode;

    inline DNode *allocDNode(void);

    DNode *newDNode(void *);
    DNode *destroyDNode(DNode *);
    DNode *destroyLoneDNode(DNode *dn);
    DNode *appendDNode(DNode *cur, void *data);
    DNode *prependDNode(DNode *cur, void *data);

    unsigned int isCircular(DNode *dn);
#endif
