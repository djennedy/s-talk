#ifndef _QUEUE_OPERATIONS_H
#define _QUEUE_OPERATIONS_H

#include "list.h"

// Manages the shared queue operations
// Creates mutexes for both operations

int enqueueMessage(List* list, char* message);
char* dequeueMessage(List* list);
int countMessages(List* list);

#endif