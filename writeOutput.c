// take list message and write to screen

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "list.h"
#include "queueOperations.h"
#include "writeOutput.h"
#define MAXBUFLEN 65508

static pthread_t writerThread;
static List* list;
static char* message;

static void* writeLoop(void* useless){

    while(1)
    {

        // TODO: CREATE A COND VAR SUCH THAT AFTER SEND, IMMEDIATELY WRITE
        // Taking message from list
        message = dequeueMessage(list);

        int writeVar = write(1,List_trim(list), strlen(message)); // will put the message from first list onto screen
        if(writeVar == -1){
            perror("Error in write() : Unable to write to screen");
            exit(-1);
        }

        // Freeing message (message is dynamically allocated from receiver)
        free(message);
        message = NULL;
    }
    return NULL;
    
}


void writerInit(List* l){

    list = l;

    int writingThread =  pthread_create(&writerThread, NULL, writeLoop, NULL);
    if(writingThread != 0){
        perror("write thread failed");
        exit(-1);
    }

}

void writerShutdown()
{
    // De-allocating dynamically allocated message if shutdown is called while message is not yet freed
    // Note: if we HAVE already freed the pointer, then we've set the message pointer to NULL
    // and it is okay to free a NULL pointer (it does nothing)
    free(message);

    pthread_cancel(writerThread);
    pthread_join(writerThread, NULL);
}

