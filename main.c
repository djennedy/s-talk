#include<stdbool.h>
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include"list.h"

// Funcion to free statically allocated items
// Empty because deletion of statically allocated items are handled by the compiler
void freeStatic(void* pItem)
{
    return;
}

// Function to free dynamically allocated items
void freeDynamic (void* pItem)
{
    free(pItem);
}

// Function to compare integers
bool compareInt (void* pItem, void* pComparisonArg)
{
    return *(int*)pItem==*(int*)pComparisonArg;
}

// Testing List_Create(), List_Free() and the head max limit to see if memory management works
void testHeadRelated()
{
    printf("\n\nTesting list create and head limit \n");
    int i;
    List* heads[12];
    printf("Creating an array of size 12 of pointers to heads \n");
    for (i=0;i<12;i++)
    {
        heads[i]=List_create();
    }

    printf("Printing the pointers to the heads in the array \n");
    for(i=0;i<12;i++)
    {
        printf("Value inside heads[%d]: %p \n",i,heads[i]);
    }
    printf("Expected result : heads[10] and heads[11] are NULL pointers \n");
    
    printf("\nTesting List_free \n");
    printf("Deleting all currently allocated heads \n");
    for (i=0;i<12;i++)
    {
        if(heads[i]!=NULL)
        {
            List_free(heads[i],&freeStatic);
        }
    }
    printf("Re-creating an array of size 10 of pointers to heads \n");
    for (i=0;i<10;i++)
    {
        heads[i]=List_create();
        printf("Value inside heads[%d]: %p \n",i,heads[i]);
    }
    printf("Expected: No null pointers\n");
    printf("Deleting all currently allocated heads \n");
    for (i=0;i<12;i++)
    {
        if(heads[i]!=NULL)
        {
            List_free(heads[i],&freeStatic);
        }
    }
}

void testListCount()
{
    printf("\n\nTesting List_count function\n");
    printf("Creating empty head\n");
    List* head = List_create();
    printf("List_count(head) = %d (expected = 0)\n",List_count(head));
    printf("Adding a node to head\n");
    int testItem = 0;
    List_add(head, &testItem);
    printf("List_count(head) = %d (expected = 1)\n",List_count(head));
    printf("Deleting a node\n");
    List_remove(head);
    printf("List_count(head) = %d (expected = 0)\n",List_count(head));
    printf("Deleting all currently allocated heads \n");
    List_free(head,&freeStatic);

}

// Testing traversal functions; List_first, List_last, List_next, List_prev, List_curr
void testListTraversal()
{
    printf("\n\nTesting traversal functions \n");
    printf("Creating empty head\n");
    List* head = List_create();
    printf("List_first(head) = %p (expected NULL)\n", List_first(head));
    printf("List_last(head) = %p (expected NULL)\n", List_last(head));
    printf("List_next(head) = %p (expected NULL)\n", List_next(head));
    printf("List_prev(head) = %p (expected NULL)\n", List_prev(head));
    printf("List_curr(head) = %p (expected NULL)\n", List_curr(head));
    
    printf("\nAdding one node to the head with value integer 0 \n");
    int testItem = 0;
    List_add(head, &testItem);
    printf("*(int*)List_first(head) = %d (expected 0)\n", *(int*)List_first(head));
    printf("*(int*)List_last(head) = %d (expected 0)\n", *(int*)List_last(head));
    printf("*(int*)List_curr(head) = %d (expected 0)\n", *(int*)List_curr(head));

    printf("\n Testing next and prev with 1 node, also testing behaviour when beyond start/end\n");
    printf("List_next(head) = %p (expected NULL)\n",List_next(head));
    printf("head->oob = %d (expected 1 (oob end)) \n",head->oob);
    printf("List_next(head) = %p (expected NULL)\n",List_next(head));
    printf("head->oob = %d (expected 1 (oob end)) \n",head->oob);
    printf("*(int*)List_prev(head) = %d (expected 0)\n", *(int*)List_prev(head));
    printf("List_prev(head) = %p (expected NULL)\n",List_prev(head));
    printf("head->oob = %d (expected 0 (oob start)) \n",head->oob);
    printf("List_prev(head) = %p (expected NULL)\n",List_prev(head));
    printf("head->oob = %d (expected 0 (oob start)) \n",head->oob);
    printf("*(int*)List_next(head) = %d (expected 0)\n", *(int*)List_next(head));

    printf("\nAdding another node to the head with value integer 1 (current item count =2)\n");
    int testItem1=1;
    List_add(head, &testItem1);
    printf("*(int*)List_first(head) = %d (expected 0)\n", *(int*)List_first(head));
    printf("*(int*)List_last(head) = %d (expected 1)\n", *(int*)List_last(head));
    printf("*(int*)List_curr(head) = %d (expected 1)\n", *(int*)List_curr(head));

    printf("\n Testing next and prev with 2 nodes\n");
    printf("*(int*)List_prev(head) = %d (expected 0)\n", *(int*)List_prev(head));
    printf("*(int*)List_next(head) = %d (expected 1)\n", *(int*)List_next(head));
    

    printf("\nDeleting all currently allocated heads \n");
    List_free(head,&freeStatic);
}

// Testing the addition functions: List_add, List_insert, List_append, List_prepend
void testListAddition()
{
    printf("\n\nTesting addition functions\n");
    printf("\nTesting adding into empty lists \n");
    List* heads[4];
    for (int i=0;i<4;i++)
    {
        heads[i] = List_create();
    }
    int testItem0=0;
    List_add(heads[0], &testItem0);
    List_insert(heads[1], &testItem0);
    List_append(heads[2], &testItem0);
    List_prepend(heads[3], &testItem0);
    printf("Current item in heads[0] after adding 0 = %d \n", *(int* ) List_curr(heads[0]));
    printf("Current item in heads[1] after inserting 0 = %d \n", *(int* ) List_curr(heads[1]));
    printf("Current item in heads[2] after appending 0 = %d \n", *(int* ) List_curr(heads[2]));
    printf("Current item in heads[3] after prepending 0 = %d \n", *(int* ) List_curr(heads[3]));

    printf("\nAdding 1 and 2 with each addition method for each element of heads\n");
    int testItem1=1;
    List_add(heads[0], &testItem1); //0 1
    List_insert(heads[1], &testItem1); // 1 0
    List_append(heads[2], &testItem1); // 0 1
    List_prepend(heads[3], &testItem1); // 1 0 

    int testItem2=2;
    List_add(heads[0], &testItem2); //0 1 2
    List_insert(heads[1], &testItem2); // 2 1 0
    List_append(heads[2], &testItem2); // 0 1 2
    List_prepend(heads[3], &testItem2); // 2 1 0

    for(int i=0;i<4;i++)
    {
        printf("\nPrinting nodes in heads[%d] \n",i);
        int j=0;
        List_first(heads[i]);
        while(heads[i]->current!=NULL)
        {
            printf("\t Value in Node #%d: %d",j,*(int*)List_curr(heads[i]));
            List_next(heads[i]);
            j++;
            if(j%5==0)
            {
                printf("\n");
            }
        }
        printf("\nPrinting head and tail nodes in heads[%d] \n",i);
        printf("\tHead of heads[%d] : %d",i,*(int*)heads[i]->head->item);
        printf("\tTail of heads[%d] : %d",i,*(int*)heads[i]->tail->item);
        printf("\n");
    }
    
    printf("\nChanging current node into the middle node, and adding 3 to the list with each addition method \n");
    for(int i=0;i<4;i++)
    {
        List_first(heads[i]);
        List_next(heads[i]);
    }
    int testItem3=3;
    List_add(heads[0], &testItem3); //0 1 3 2
    List_insert(heads[1], &testItem3); // 2 3 1 0
    List_append(heads[2], &testItem3); // 0 1 2 3
    List_prepend(heads[3], &testItem3); // 3 2 1 0
    for(int i=0;i<4;i++)
    {
        printf("\nPrinting nodes in heads[%d] \n",i);
        int j=0;
        List_first(heads[i]);
        while(heads[i]->current!=NULL)
        {
            printf("\t Value in Node #%d: %d",j,*(int*)List_curr(heads[i]));
            List_next(heads[i]);
            j++;
            if(j%5==0)
            {
                printf("\n");
            }
        }
        printf("\n");
    }

    printf("\nTesting additions when current is before start\n");
    for(int i=0;i<4;i++)
    {
        List_first(heads[i]);
        List_prev(heads[i]);
    }
    int testItem4=4;
    List_add(heads[0], &testItem4); //4 0 1 3 2
    List_insert(heads[1], &testItem4); //4 2 3 1 0
    List_append(heads[2], &testItem4); //0 1 2 3 4
    List_prepend(heads[3], &testItem4); //4 3 2 1 0
    for(int i=0;i<4;i++)
    {
        printf("\nPrinting nodes in heads[%d] \n",i);
        int j=0;
        List_first(heads[i]);
        while(heads[i]->current!=NULL)
        {
            printf("\t Value in Node #%d: %d",j,*(int*)List_curr(heads[i]));
            List_next(heads[i]);
            j++;
            if(j%5==0)
            {
                printf("\n");
            }
        }
        printf("\nPrinting head and tail nodes in heads[%d] \n",i);
        printf("\tHead of heads[%d] : %d",i,*(int*)heads[i]->head->item);
        printf("\tTail of heads[%d] : %d",i,*(int*)heads[i]->tail->item);

        printf("\n");
    }

    printf("\nTesting additions when current is before start\n");
    for(int i=0;i<4;i++)
    {
        List_last(heads[i]);
        List_next(heads[i]);
    }
    int testItem5=5;
    List_add(heads[0], &testItem5); //4 0 1 3 2 5
    List_insert(heads[1], &testItem5); //4 2 3 1 0 5
    List_append(heads[2], &testItem5); //0 1 2 3 4 5
    List_prepend(heads[3], &testItem5); //5 4 3 2 1 0
    for(int i=0;i<4;i++)
    {
        printf("\nPrinting nodes in heads[%d] \n",i);
        int j=0;
        List_first(heads[i]);
        while(heads[i]->current!=NULL)
        {
            printf("\t Value in Node #%d: %d",j,*(int*)List_curr(heads[i]));
            List_next(heads[i]);
            j++;
            if(j%6==0)
            {
                printf("\n");
            }
        }
        printf("\nPrinting head and tail nodes in heads[%d] \n",i);
        printf("\tHead of heads[%d] : %d",i,*(int*)heads[i]->head->item);
        printf("\tTail of heads[%d] : %d",i,*(int*)heads[i]->tail->item);
        
        printf("\n");
    }
    for(int i=0;i<4;i++)
    {
        printf("Current size of list (expected 6) : %d\n", List_count(heads[i]));
    }

    printf("\nDeleting all currently allocated heads \n");
    for (int i=0;i<4;i++)
    {
        List_free(heads[i],&freeStatic);
    }

    printf("\nTesting addition on non available heads \n");
    printf("List_add(heads[0], &testItem5) = %d (expected -1) \n",List_add(heads[0], &testItem5)); //empty
    printf("List_insert(heads[1], &testItem5) = %d (expected -1) \n",List_insert(heads[1], &testItem5)); //empty
    printf("List_append(heads[2], &testItem5) = %d (expected -1) \n",List_append(heads[2], &testItem5)); //empty
    printf("List_prepend(heads[3], &testItem5) = %d (expected -1) \n",List_prepend(heads[3], &testItem5)); //empty
}

// Testing the node max limits to see if memory management works
void testNodeLimit()
{
    printf("\n\nTesting node limit \n");
    printf("Testing with 1 head \n");
    List* head = List_create();
    int testItems[102];
    for (int i=0;i<102;i++)
    {
        testItems[i]=i;
    }
    printf("Adding 100 nodes containing ints 0 to 99 to the head \n");
    for (int i=0;i<100;i++)
    {
        List_add(head, &testItems[i]);
    }

    printf("Attempting to add ints 100 and 101 to the head \n");
    List_add(head, &testItems[100]);
    List_add(head, &testItems[101]);

    printf("Printing the items in the nodes in the array \n");
    int i=0;
    List_first(head);
    while(head->current!=NULL)
    {
        printf("Value in Node #%d : %d  ",i,*(int*)List_curr(head));
        List_next(head);
        i++;
        if(i%5==0)
        {
            printf("\n");
        }
    }
    printf("\n");
    printf("Expected result : no node with item 100 and 101 \n");
    
    printf("Deleting the list \n");
    List_free(head,&freeStatic);

    printf("\n\nTesting with multiple heads \n");
    printf("Creating 10 heads \n");
    List* heads[10];
    for (int i=0;i<10;i++)
    {
        heads[i]=List_create();
    }

    printf("Populating each of the 10 heads with 10 nodes\n");
    for(int i=0;i<100;i++)
    {
        List_add(heads[i/10],&testItems[i]);
    }

    printf("Attempting to add one node to each head with item 100\n");
    for(int i=0;i<10;i++)
    {
        List_add(heads[i], &testItems[100]);
    }

    for(int i=0;i<10;i++)
    {
        printf("Printing nodes in heads[%d] \n",i);
        int j=0;
        List_first(heads[i]);
        while(heads[i]->current!=NULL)
        {
            printf("\t Value in Node #%d: %d",j,*(int*)List_curr(heads[i]));
            List_next(heads[i]);
            j++;
            if(j%5==0)
            {
                printf("\n");
            }
        }
        printf("\n");
    }

    printf("Expected: No node with item 100\n");

    printf("\nDeleting all currently allocated heads \n");
    for (i=0;i<10;i++)
    {
        List_free(heads[i],&freeStatic);
    }
}

// Testing removal functions: List_remove, List_trim
void testRemovalFunctions()
{
    printf("\n\nTesting node removal functions\n");
    printf("\nTesting removing empty lists \n");
    List* head;
    head = List_create();
    printf("List_remove(head) : %p (expected NULL) \n",List_remove(head));
    printf("List_trim(head) : %p (expected NULL) \n",List_trim(head));

    printf("\nAdding 0 to 10 to the list\n");
    int testItems[10];
    for (int i=0;i<10;i++)
    {
        testItems[i]=i;
        List_add(head, &testItems[i]);
    }
    int i=0;
    List_first(head);
    while(head->current!=NULL)
    {
        printf("Value in Node #%d : %d  ",i,*(int*)List_curr(head));
        List_next(head);
        i++;
        if(i%5==0)
        {
            printf("\n");
        }
    }
    printf("Current size of list (expected 10) : %d\n", List_count(head));
    printf("\n");
    
    printf("Current node is in beyond end\n");
    printf("List_remove(head) : %p (expected NULL) \n",List_remove(head));
    printf("*(int*)List_trim(head) : %d (expected 9) \n",*(int*)List_trim(head));
    printf("Tail after trimming: %d (expected 8) \n",*(int*)List_last(head));
    printf("Current size of list (expected 9) : %d\n", List_count(head));
    
    printf("\nMoving current node to be before start \n");
    List_first(head);
    List_prev(head);
    printf("List_remove(head) : %p (expected NULL) \n",List_remove(head));
    printf("*(int*)List_trim(head) : %d (expected 8) \n",*(int*)List_trim(head));
    printf("Tail after trimming: %d (expected 7) \n",*(int*)List_last(head));
    printf("Current size of list (expected 8) : %d\n", List_count(head));

    printf("\nMoving current node to start\n");
    printf("Head before removing: %d (expected 0) \n",*(int*)List_first(head));
    printf("List_remove(head) : %d (expected 0) \n",*(int*)List_remove(head));
    printf("Head after removing: %d (expected 1) \n",*(int*)List_first(head));
    printf("Current size of list (expected 7) : %d\n", List_count(head));

    printf("\nMoving current node to end\n");
    printf("Tail before removing: %d (expected 7) \n",*(int*)List_last(head));
    printf("List_remove(head) : %d (expected 7) \n",*(int*)List_remove(head));
    printf("Tail after removing: %d (expected 6) \n",*(int*)List_last(head));
    printf("Current size of list (expected 6) : %d\n", List_count(head));

    printf("\nPrinting current list\n");
    i=0;
    List_first(head);
    while(head->current!=NULL)
    {
        printf("Value in Node #%d : %d  ",i,*(int*)List_curr(head));
        List_next(head);
        i++;
        if(i%5==0)
        {
            printf("\n");
        }
    }
    printf("Current size of list (expected 6) : %d\n", List_count(head));
    printf("\n");

    printf("\nMoving current node to the third node (item stored 3)\n");
    List_first(head);
    List_next(head);
    printf("Third node before removing: %d (expected 3) \n",*(int*)List_next(head));
    printf("List_remove(head) : %d (expected 3) \n",*(int*)List_remove(head));

    printf("\nPrinting current list after third node removal\n");
    i=0;
    List_first(head);
    while(head->current!=NULL)
    {
        printf("Value in Node #%d : %d  ",i,*(int*)List_curr(head));
        List_next(head);
        i++;
        if(i%5==0)
        {
            printf("\n");
        }
    }
    printf("Current size of list (expected 5) : %d\n", List_count(head));
    
    printf("\nDeleting all currently allocated heads \n");
    List_free(head,&freeStatic);
}

// Testing List_concat
void testConcat()
{
    printf("\nTesting List_concat\n");
    printf("\nTesting concat with 2 empty heads\n");
    printf("Creating 2 empty heads\n");
    List* head1=List_create();
    List* head2=List_create();
    printf("Concatenating both empty heads \n");
    List_concat(head1,head2);
    printf("\tTo test whether or not head2 no longer exists, we create 9 more heads\n");
    printf("\tIf all 9 heads are successfully created, then List_concat properly removes head2\n");
    List* heads[10];
    for (int i=0;i<10;i++)
    {
        heads[i]=List_create();
        if(i<9)
        {
            printf("heads[%d] = %p (expected not NULL)\n",i,heads[i]);
        }
        else
        {
            printf("heads[%d] = %p (expected NULL)\n",i,heads[i]);
        }
    }
    printf("\nDeleting the temporary array heads[] \n");
    for (int i=0;i<10;i++)
    {
        if(heads[i]!=NULL)
        {
            List_free(heads[i],&freeStatic);
        }
    }

    printf("\nTesting concat of non-empty head1 and empty head2\n");
    head2=List_create();
    printf("Adding 0 and 1 to head1\n");
    int testItems[10];
    for (int i=0;i<10;i++)
    {
        testItems[i]=i;
    }
    List_add(head1, &testItems[0]);
    List_add(head1, &testItems[1]);

    printf("\nPrinting head1\n");
    int i=0;
    List_first(head1);
    while(head1->current!=NULL)
    {
        printf("Value in Node #%d : %d  ",i,*(int*)List_curr(head1));
        List_next(head1);
        i++;
        if(i%5==0)
        {
            printf("\n");
        }
    }
    printf("\n");
    printf("Current size of list (expected 2) : %d\n", List_count(head1));

    printf("\nConcatenating head1 with empty head2\n");
    List_concat(head1,head2);

    printf("\nPrinting head1 after concatenating with empty head2\n");
    i=0;
    List_first(head1);
    while(head1->current!=NULL)
    {
        printf("Value in Node #%d : %d  ",i,*(int*)List_curr(head1));
        List_next(head1);
        i++;
        if(i%5==0)
        {
            printf("\n");
        }
    }
    printf("\n");
    printf("Current size of list (expected 2) : %d\n", List_count(head1));
    
    printf("\tTo test whether or not head2 no longer exists, we create 9 more heads\n");
    printf("\tIf all 9 heads are successfully created, then List_concat properly removes head2\n");
    for (int i=0;i<10;i++)
    {
        heads[i]=List_create();
        if(i<9)
        {
            printf("heads[%d] = %p (expected not NULL)\n",i,heads[i]);
        }
        else
        {
            printf("heads[%d] = %p (expected NULL)\n",i,heads[i]);
        }
    }
    printf("\nDeleting the temporary array heads[] \n");
    for (int i=0;i<10;i++)
    {
        if(heads[i]!=NULL)
        {
            List_free(heads[i],&freeStatic);
        }
    }

    printf("\nTesting concat of empty head1 and non-empty head2\n");
    List_free(head1,&freeStatic);
    head1=List_create();
    head2=List_create();
    printf("Adding 0 and 1 to head2 \n");
    List_add(head2, &testItems[0]);
    List_add(head2, &testItems[1]);

    printf("\nPrinting head2\n");
    i=0;
    List_first(head2);
    while(head2->current!=NULL)
    {
        printf("Value in Node #%d : %d  ",i,*(int*)List_curr(head2));
        List_next(head2);
        i++;
        if(i%5==0)
        {
            printf("\n");
        }
    }
    printf("\n");
    printf("Current size of list (expected 2) : %d\n", List_count(head2));

    printf("\nConcatenating empty head1 with non-empty head2\n");
    List_concat(head1,head2);

    printf("\nPrinting the previously empty head1 after concatenating with non-empty head2\n");
    i=0;
    List_first(head1);
    while(head1->current!=NULL)
    {
        printf("Value in Node #%d : %d  ",i,*(int*)List_curr(head1));
        List_next(head1);
        i++;
        if(i%5==0)
        {
            printf("\n");
        }
    }
    printf("\n");
    printf("Current size of list (expected 2) : %d\n", List_count(head1));

    printf("head1 = %p\n",head1);
    
    printf("\tTo test whether or not head2 no longer exists, we create 9 more heads\n");
    printf("\tIf all 9 heads are successfully created, then List_concat properly removes head2\n");
    for (int i=0;i<10;i++)
    {
        heads[i]=List_create();
        if(i<9)
        {
            printf("heads[%d] = %p (expected not NULL)\n",i,heads[i]);
        }
        else
        {
            printf("heads[%d] = %p (expected NULL)\n",i,heads[i]);
        }
    }
    printf("\nDeleting the temporary array heads[] \n");
    for (int i=0;i<10;i++)
    {
        if(heads[i]!=NULL)
        {
            List_free(heads[i],&freeStatic);
        }
    }

    printf("\nConcatenating non-empty head1 with non-empty head2\n");
    List_free(head1,&freeStatic);
    List_free(head2,&freeStatic);
    head1=List_create();
    head2=List_create();
    printf("Adding 0 and 1 to head1 and 2 3 to head2");
    List_add(head1,&testItems[0]);
    List_add(head1,&testItems[1]);
    List_add(head2,&testItems[2]);
    List_add(head2,&testItems[3]);

    printf("\nPrinting head1\n");
    i=0;
    List_first(head1);
    while(head1->current!=NULL)
    {
        printf("Value in Node #%d : %d  ",i,*(int*)List_curr(head1));
        List_next(head1);
        i++;
        if(i%5==0)
        {
            printf("\n");
        }
    }
    printf("\n");
    printf("Current size of list (expected 2) : %d\n", List_count(head1));

    printf("\nPrinting head2\n");
    i=0;
    List_first(head2);
    while(head2->current!=NULL)
    {
        printf("Value in Node #%d : %d  ",i,*(int*)List_curr(head2));
        List_next(head2);
        i++;
        if(i%5==0)
        {
            printf("\n");
        }
    }
    printf("\n");
    printf("Current size of list (expected 1) : %d\n", List_count(head2));

    printf("\nMoving current of head1 to tail of head1\n");
    printf("Current node of head1 is : %d\n",*(int*)List_last(head1));
    printf("\nConcatenating head1 and head2\n");
    List_concat(head1,head2);

    printf("Checking current node of head 1: %d (expected 1)\n",*(int*)List_curr(head1));

    printf("\nPrinting head1 after concatenation with head2\n");
    i=0;
    List_first(head1);
    while(head1->current!=NULL)
    {
        printf("Value in Node #%d : %d  ",i,*(int*)List_curr(head1));
        List_next(head1);
        i++;
        if(i%5==0)
        {
            printf("\n");
        }
    }
    printf("\n");
    printf("Current size of list (expected 4) : %d\n", List_count(head1));

    printf("\nDeleting all currently allocated heads \n");
    List_free(head1,&freeStatic);
    List_free(head2,&freeStatic);
}

// Testing List_search
void testSearch()
{
    int i;
    printf("\nTesting List_search\n");
    printf("\nCreating testItems[10] with values 0 to 9\n");
    int testItems[10];
    for(int i=0;i<10;i++)
    {
        testItems[i]=i;
    }
    printf("Testing with empty head\n");
    List* head=List_create();
    printf("List_search(head, &compareInt, &testItems[0]) = %p (expected NULL)\n",List_search(head, &compareInt, &testItems[0]));

    printf("\nPopulating the list with 0-9\n");
    for (int i=0;i<10;i++)
    {
        List_add(head, &testItems[i]);
    }

    printf("\nPrinting head\n");
    i=0;
    List_first(head);
    while(head->current!=NULL)
    {
        printf("Value in Node #%d : %d  ",i,*(int*)List_curr(head));
        List_next(head);
        i++;
        if(i%5==0)
        {
            printf("\n");
        }
    }
    printf("Current size of list (expected 10) : %d\n", List_count(head));

    printf("\nMoving current node to beyond end and testing List_search\n");
    List_last(head);
    List_next(head);
    printf("List_search(head, &compareInt, &testItems[0]) = %p (expected NULL)\n",List_search(head, &compareInt, &testItems[0]));

    printf("\nMoving current node to before start and searching for 5\n");
    int test5=5;
    List_first(head);
    List_prev(head);
    printf("*(int*)List_search(head, &compareInt, &test5) = %d (expected 5)\n",*(int*)List_search(head, &compareInt, &test5));
    printf("*(int*)List_curr(head) = %d (expected 5)\n",*(int*)List_curr(head));

    printf("\nSearching for 8\n");
    int test8=8;
    printf("*(int*)List_search(head, &compareInt, &test5) = %d (expected 8)\n",*(int*)List_search(head, &compareInt, &test8));
    printf("*(int*)List_curr(head) = %d (expected 8)\n",*(int*)List_curr(head));

    printf("\nSearching for 5 which is behind current\n");
    printf("List_search(head, &compareInt, &test5) = %p (expected NULL)\n",List_search(head, &compareInt, &test5));
    printf("List_curr(head) = %p (expected NULL)\n",List_curr(head));
    printf("head->oob = %d (expected 1 (oob end) )\n",head->oob);

    printf("\nDeleting all currently allocated heads \n");
    List_free(head,&freeStatic);
}

int main ()
{
    testHeadRelated();
    printf("\n------------------------------------------------------\n");
    testListCount();
    printf("\n------------------------------------------------------\n");
    testListTraversal();
    printf("\n------------------------------------------------------\n");
    testListAddition();
    printf("\n------------------------------------------------------\n");
    testNodeLimit();
    printf("\n------------------------------------------------------\n");
    testRemovalFunctions();
    printf("\n------------------------------------------------------\n");
    testConcat();
    printf("\n------------------------------------------------------\n");
    testSearch();

    return 0;
}