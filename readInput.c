// reads off keyboard and adds to list

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "list.h"
#include "queueOperations.h"
#include "readInput.h"
#include "sendUDP.h"
#include "threadCanceller.h"

// Max size of the message, using theoretical max length for a UDP packet
#define MAXBUFLEN 65506

static List* list;
static pthread_t readerThread;

static void* readLoop(void* useless){
    while(1)
    {
        // Declaring variables
        char* message;
        char bufStorageOfMessage[MAXBUFLEN]; 
        int numbytes;
        int iteration = 0;
        do
        {
            iteration++;

            // Emptying input string buffer
            memset(&bufStorageOfMessage, 0, MAXBUFLEN);
            // Reading user input
            numbytes = read(0,bufStorageOfMessage, MAXBUFLEN);
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
            // Ends the process if exit code was in the first iteration of read
            if (!strcmp(message,"!\n") && iteration==1)
            {
                senderSignaller();
                cancelReceiverWriter();
                return NULL;
            }
        } while (bufStorageOfMessage[numbytes-1]!='\n');

        //send signal for the senderUDP
        senderSignaller();

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

void readerCancel()
{
    pthread_cancel(readerThread);
}

void readerShutdown()
{
    pthread_join(readerThread,NULL);
}
