#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <pthread.h>

#include"queueOperations.h"
#include"list.h"

static pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;

void enqueueMessage(List* list, char* message)
{
    pthread_mutex_lock(&queueMutex);
    {
        int prepVal = List_prepend(list, message);
        if(prepVal==-1)
        {
            fprintf(stderr, "Enqueue failed, queue full");
        }
    }
    pthread_mutex_unlock(&queueMutex);
}
char* dequeueMessage(List* list)
{
    char* message;
    pthread_mutex_lock(&queueMutex);
    {
        message = List_trim(list);
        if(message == NULL)
        {
            fprintf(stderr, "Dequeue failed, queue empty");
        }
    }
    pthread_mutex_unlock(&queueMutex);

    return message;
}

int countMessages(List* list)
{
    return List_count(list);
}