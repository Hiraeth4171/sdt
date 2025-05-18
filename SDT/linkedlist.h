#ifndef _SDT_LINKEDLIST_LIB_H
#define _SDT_LINKEDLIST_LIB_H

typedef struct SDTLinkedListNode SDTLinkedListNode;

struct SDTLinkedListNode{
    void* value;
    SDTLinkedListNode* next;
};

#endif
