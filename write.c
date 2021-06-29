//keyboard,
// take list message and write to screen

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <pthread.h> 
#include "list.h"
#define MAXBUFLEN 65508

static List* list;

//brians vid on synchronozayion 
static pthread_t writeThread;
static pthread_cond_t readyW = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t readyWMutex = PTHREAD_MUTEX_INITIALIZER;


void writeTask(void* useless){

    int writeVar = write(1,List_trim(list), MAXBUFLEN ); // will put the message from first list onto screen
    if(writeVar == -1){
        perror("Error in write() : Unable to write to screen");

    }
}


void writeInit(char* hostname, char* port, List* list){

    //before write gets called needs to wait//
    //wait until signalled// 
    pthread_mutex_lock(&readyWMutex); // acquire a lock on the mutex
    {
        //signal//
        pthread_cond_wait(&readyW,&readyWMutex);// mutex will release while 
    }
    pthread_mutex_unlock(&readyWMutex); // unlocks the mutex


    //the condition it needs to check for if it was called from reciever
    //if so then proceed to do the write


    int writingThread =  pthread_create(&writeThread, NULL, writeTask, NULL);
    if(writingThread != 0){
        perror("Error: write thread failed");
    }

    //once done//
    pthread_join(writingThread, NULL);

}



int main(int argCount,char* args[]){
   
    write(1,"Test", 4); // this works!

}
