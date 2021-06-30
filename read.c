//screen, 
//reads off keyboard and adds to list

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

static  pthread_t readThread;
static pthread_cond_t ready = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t readyMutex = PTHREAD_MUTEX_INITIALIZER;


void readTask(void* useless){
    //read gets called  // done
    //adds message to list // done
    //notify sendUDP that read is ready // done



    char bufStorageOfMessage[MAXBUFLEN]; 
    // bufStorageOfMessage = '\0';
    read(0,bufStorageOfMessage, MAXBUFLEN -1);
    List_perpend(list, bufStorageOfMessage);
}


void readInit(char* hostname, char* port, List* list){

    //first read gets called and added to list
    int readingThread =  pthread_create(&readThread, NULL, readTask, NULL);
    if(readingThread <= 0){//if gave error of -1 or 0 for false
        perror("read thread failed");
    }
    
    //signaling sendUPD
    //wrapping the signal in locks
    pthread_mutex_lock(&readyMutex); // acquire a lock on the mutex
    {
        //signal//
        pthread_cond_signal(&ready);
    }
    pthread_mutex_unlock(&readyMutex); // unlocks the mutex


}



int main(int argCount,char* args[]){



}
