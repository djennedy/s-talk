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

static void* readLoop(void* useless){
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
        message = (char*)malloc(sizeof(char)*(numbytes));
        strncpy(message, bufStorageOfMessage, numbytes);
        // Note: we don't need null terminator here because we're going to send the result, not print it
        // Our receiver will add the null terminator for us

        // TODO: CREATE A COND VAR SUCH THAT AFTER READ, IMMEDIATELY SEND

        // Adding the message to the list
        enqueueMessage(list, message);

        //send signal for the senderUDP
        sendSignaller(); 



    }
    return NULL;
}


void readerInit(List* l){
    list = l;

    int readingThread =  pthread_create(&readerThread, NULL, readLoop, NULL);
    if(readingThread !=0){//if gave error code of not 0 (0 is success)
        perror("reader: thread creation error");
        exit(-1);
    }
    
}

void readerShutdown()
{
    pthread_cancel(readerThread);
    pthread_join(readerThread,NULL);
}
