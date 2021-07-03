#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <pthread.h>

#include"queueOperations.h"
#include"list.h"

static pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;

int enqueueMessage(List* list, char* message)
{
    int prepVal;
    pthread_mutex_lock(&queueMutex);
    {
        prepVal = List_prepend(list, message);
    }
    pthread_mutex_unlock(&queueMutex);
    return prepVal;
}
char* dequeueMessage(List* list)
{
    char* message;
    pthread_mutex_lock(&queueMutex);
    {
        message = List_trim(list);
    }
    pthread_mutex_unlock(&queueMutex);

    return message;
}

int countMessages(List* list)
{
    int count;
    pthread_mutex_lock(&queueMutex);
    {
        count = List_count(list);
    }
    pthread_mutex_unlock(&queueMutex);

    return count;
}