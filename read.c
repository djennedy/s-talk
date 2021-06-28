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

static  pthread_t readThread;



void readTask(void* useless){
    //read gets called  // done
    //adds message to list // done
    //notify sendUDP that read is done // use condition variable, pthreads


    // said below was right idea..//
    char bufStorageOfMessage[MAXBUFLEN]; 
    // bufStorageOfMessage = '\0';
    read(0,bufStorageOfMessage, MAXBUFLEN -1);
    List_perpend(list, bufStorageOfMessage);
}


void readInit(char* hostname, char* port, List* list){

    int readingThread =  pthread_create(&readThread, NULL, readTask, NULL);
    if(readingThread != 0){
        perror("read thread failed");
    }

}



int main(int argCount,char* args[]){



}
