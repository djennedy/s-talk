// reads off keyboard and adds to list

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "list.h"
#include "queueOperations.h"
#include "readInput.h"

#define MAXBUFLEN 65508

static List* list;
static pthread_t readerThread;

static void* readTask(void* useless){
    while(1)
    {
        // Declaring variables
        char* message;
        char bufStorageOfMessage[MAXBUFLEN]; 

        // Reading user input
        int numbytes = read(0,bufStorageOfMessage, MAXBUFLEN);
        if(numbytes==-1)
        {
            perror("reader: read() failed");
            exit(-1);
        }

        // Downsizing the size of the message to be more space efficient
        message = (char*)malloc(sizeof(char)*(numbytes+1));
        strncpy(message, bufStorageOfMessage, numbytes);
        message[numbytes] = '\0';

        // Adding the message to the list
        enqueueMessage(list, message);

        // Checking for exit code
        if (!strcmp(message,"!"))
        {
            return NULL;
        }
    }
    return NULL;
}


void readerInit(List* l){
    list = l;

    int readingThread =  pthread_create(&readerThread, NULL, readTask, NULL);
    if(readingThread !=0){//if gave error code of not 0 (0 is success)
        perror("reader: thread creation error");
        exit(-1);
    }
}

void readerShutdown()
{
    pthread_join(readerThread,NULL);
}
