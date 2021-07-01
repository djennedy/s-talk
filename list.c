#include<stdbool.h>
#include<assert.h>
#include<stddef.h>
#include"list.h"

#include<stdio.h>

static bool initialized = false;

typedef struct HeadMemory_s HeadMemory;
// A stack to represent the statically allocated memory of heads
struct HeadMemory_s
{
    List headArray[LIST_MAX_NUM_HEADS];
    List* top;
    int numAvailable;
};

static HeadMemory hm;

static bool isExhaustedHeadMem()
{
    return hm.numAvailable==0;
}

// Empties and pushes a head into the top of the head memory stack
static void pushHeadMem(List* list)
{
    if(hm.numAvailable==LIST_MAX_NUM_HEADS)
    {
        return;
    }

    list->head = NULL;
    list->tail = NULL;
    list->numItems = 0;
    list->current = NULL;
    list->oob = LIST_OOB_START;
    list->isAvailable=false;

    list->next = hm.top;
    hm.top = list;
    hm.numAvailable++;
    return;
}

//Pops an pointer to an empty head from the top of the head memory stack and returns it
//Returns NULL if all heads are exhausted
static List* popHeadMem()
{
    if(isExhaustedHeadMem())
    {
        return NULL;
    }

    List* temp = hm.top;
    //temp->next = NULL;
    hm.top = hm.top->next;
    hm.numAvailable--;

    temp->head = NULL;
    temp->tail = NULL;
    temp->numItems=0;
    temp->current = NULL;
    temp->oob = LIST_OOB_START;
    temp->isAvailable=true;

    return temp;
}

typedef struct NodeMemory_s NodeMemory;
struct NodeMemory_s
{
    Node nodeArray[LIST_MAX_NUM_NODES];
    Node* top;
    int numAvailable;
};

static NodeMemory nm;

static bool isExhaustedNodeMem()
{
    return nm.numAvailable==0;
}

//Empties and pushes a pointer to a node into the top of the node memory stack
static void pushNodeMem(Node* node)
{
    if(nm.numAvailable==LIST_MAX_NUM_NODES)
    {
        return;
    }

    node->prev=NULL;
    node->item=NULL;
    node->next = nm.top;
    nm.top = node;
    nm.numAvailable++;
}

//Pops a pointer to an empty node from the top of the node memory stack and returns it
//Returns NULL if all nodes are exhausted
static Node* popNodeMem()
{
    if(isExhaustedNodeMem(nm))
    {
        return NULL;
    }

    Node* temp = nm.top;
    nm.top = nm.top->next;
    nm.numAvailable--;

    temp->prev = NULL;
    temp->item = NULL;
    return temp;
}

//Initializes the head memory stack and the node memory stack
//Is called only once, when List_create() is first called
static void initialize()
{
    int i;
    initialized = true;

    //Initializing head memory
    
    for (i=0;i<LIST_MAX_NUM_HEADS-1;i++)
    {
        hm.headArray[i].next=&(hm.headArray[i+1]);
    }

    hm.headArray[LIST_MAX_NUM_HEADS-1].next=NULL;
    hm.numAvailable=LIST_MAX_NUM_HEADS;
    hm.top = &(hm.headArray[0]);

    //Initializing node memory

    for(i = 0;i<LIST_MAX_NUM_NODES-1;i++)
    {
        nm.nodeArray[i].next = &(nm.nodeArray[i+1]);
    }

    nm.nodeArray[LIST_MAX_NUM_NODES-1].next=NULL;
    nm.numAvailable=LIST_MAX_NUM_NODES;
    nm.top = &(nm.nodeArray[0]);
}

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create()
{
    if(initialized==false)
    {
        initialize();
    }

    if(isExhaustedHeadMem())
    {
        return NULL;
    }

    return popHeadMem();
}

// Returns the number of items in pList.
int List_count(List* pList)
{
    assert(pList!=NULL);

    return pList->numItems;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList)
{
    assert(pList!=NULL);

    if(pList->numItems == 0)
    {
        pList->current = NULL;
        return NULL;
    }
    pList->current = pList->head;
    return pList->head->item;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList)
{
    assert(pList!=NULL);

    if(pList->numItems == 0)
    {
        pList->current = NULL;
        return NULL;
    }
    pList->current = pList->tail;
    return pList->tail->item;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList)
{
    assert(pList!=NULL);

    if(pList->numItems==0)
    {
        if (pList->oob==LIST_OOB_START)
        {
            pList->oob=LIST_OOB_END;
        }
        return NULL;
    }

    if(pList->current==pList->tail || (pList->current==NULL && pList->oob==LIST_OOB_END))
    {
        pList->oob = LIST_OOB_END;
        pList->current = NULL;
        return NULL;
    }

    if(pList->current == NULL && pList->oob==LIST_OOB_START)
    {
        pList->current = pList->head;
        return pList->current->item;
    }

    pList->current = pList->current->next;
    return pList->current->item;
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList)
{
    assert(pList!=NULL);

    if(pList->numItems==0)
    {
        return NULL;
    }

    if(pList->current == pList->head || (pList->current==NULL && pList->oob==LIST_OOB_START))
    {
        pList->oob = LIST_OOB_START;
        pList->current = NULL;
        return NULL;
    }

    if(pList->current == NULL && pList->oob==LIST_OOB_END)
    {
        pList->current = pList->tail;
        return pList->current->item;
    } 

    pList->current = pList->current->prev;
    return pList->current->item;
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList)
{
    assert(pList!=NULL);

    if(pList->current == NULL)
    {
        return NULL;
    }
    return pList->current->item;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem)
{
    assert(pList!=NULL);

    if(!pList->isAvailable)
    {
        return LIST_FAIL;
    }

    //1. If there are no more space for new nodes
    if (isExhaustedNodeMem(nm))
    {
        return LIST_FAIL;
    }

    Node* newNode = popNodeMem(nm);
    newNode->item = pItem;

    pList->numItems++;

    //2. If list was empty
    if(pList->numItems-1==0)
    {
        pList->head = newNode;
        pList->tail = newNode;
        newNode->next = NULL;
        newNode->prev = NULL;
        pList->current = newNode;
        return LIST_SUCCESS;
    }

    if(pList->current==NULL)
    {
        //3. If current is before start
        if(pList->oob==LIST_OOB_START)
        {
            newNode->next = pList->head;
            newNode->prev = NULL;
            pList->head->prev = newNode;
            pList->head = newNode;
        }
        //4. if current is after end
        if(pList->oob==LIST_OOB_END)
        {
            newNode->prev = pList->tail;
            newNode->next = NULL;
            pList->tail->next = newNode;
            pList->tail = newNode;
        }
        pList->current = newNode;
        return LIST_SUCCESS;
    }

    //5. If current is the tail node
    if(pList->current == pList->tail)
    {
        pList->tail = newNode;
    }
    
    //6. Regular situation
    newNode->next = pList->current->next;
    newNode->prev = pList->current;
    pList->current->next = newNode;
    if(newNode->next!=NULL)
    {
        newNode->next->prev=newNode;
    }
    pList->current = newNode;

    return LIST_SUCCESS;
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert(List* pList, void* pItem)
{
    assert(pList!=NULL);

    if(isExhaustedNodeMem(nm))
    {
        return LIST_FAIL;
    }
    List_prev(pList);
    return List_add(pList,pItem);
}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem)
{
    assert(pList!=NULL);

    if(isExhaustedNodeMem(nm))
    {
        return LIST_FAIL;
    }
    pList->current = pList->tail;
    return List_add(pList,pItem);
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem)
{
    assert(pList!=NULL);

    if(isExhaustedNodeMem(nm))
    {
        return LIST_FAIL;
    }
    pList->current = pList->head;
    return List_insert(pList,pItem);
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList)
{
    assert(pList!=NULL);

    if(pList->current==NULL)
    {
        return NULL;
    }

    pList->numItems--;
    Node* curNode = pList->current;
    void* curItem = curNode->item;

    if(curNode == pList->tail)
    {
        pList->oob = LIST_OOB_END;
        pList->tail = pList->tail->prev;
    }

    if(curNode == pList->head)
    {
        pList->head = pList->head->next;
    }

    if(curNode->prev!=NULL)
    {
        curNode->prev->next = curNode->next;
    }
    if(curNode->next!=NULL)
    {
        curNode->next->prev = curNode->prev;
    }
    pList->current = curNode->next;
    pushNodeMem(curNode);
    return curItem;
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2)
{
    assert(pList1!=NULL && pList2!=NULL);

    if(pList2->numItems==0)
    {
        pushHeadMem(pList2);
        return;
    }

    if(pList1->numItems==0)
    {
        pList1->head = pList2->head;
        pList1->tail = pList2->tail;
        pList1->numItems = pList1->numItems;

        pushHeadMem(pList2);
        return;
    }

    pList1->tail->next = pList2->head;
    pList2->head->prev = pList1->tail;

    pList1->tail=pList2->tail;

    pList1->numItems+=pList2->numItems;

    pushHeadMem(pList2);
    return;
}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn)
{
    assert(pList!=NULL);

    if(!pList->isAvailable)
    {
        return;
    }

    pList->current = pList->head;
    while(pList->current!=NULL)
    {
        Node* temp = pList->current;
        pList->current = pList->current->next;

        if(pItemFreeFn!=NULL)
        {
            (*pItemFreeFn)(temp->item);
        }
        pushNodeMem(temp);
    }

    pushHeadMem(pList);
    return;
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList)
{
    assert(pList!=NULL);

    if(pList->numItems==0)
    {
        return NULL;
    }

    pList->current = pList->tail;
    void* retItem = List_remove(pList);
    pList->current = pList->tail;
    return retItem;
}

// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// 
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg)
{
    assert(pList!=NULL);
    if(pComparator==NULL)
    {
        return NULL;
    }
    
    if(pList->numItems==0)
    {
        return NULL;
    }

    if(pList->current==NULL && pList->oob==LIST_OOB_END)
    {
        return NULL;
    }

    if(pList->current==NULL && pList->oob==LIST_OOB_START)
    {
        List_next(pList);
    }

    while (pList->current!=NULL)
    {
        if( (*pComparator)(pList->current->item,pComparisonArg))
        {
            return pList->current->item;
        }
        pList->current = pList->current->next;
    }

    pList->oob=LIST_OOB_END;
    return NULL;
} 