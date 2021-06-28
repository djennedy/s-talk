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

#include "list.h"
#define MAXBUFLEN 65508

static pthread_t writeThread;

void writeTask(void* useless){
    if(List_count(list) !=0){// while there are items on the list
            printf("Testing writeScreen!");
            int writeVar = write(1,List_trim(list), MAXBUFLEN ); // will put the message from first list onto screen

            
            if(writeVar == -1){
                perror("Error: Unable to write to screen");

            }
        }
}




void writeInit(char* hostname, char* port, List* list){

  int writingThread =  pthread_create(&writeThread, NULL, writeTask, NULL);
    if(writingThread != 0){
        perror("write thread failed");
    }

}



int main(int argCount,char* args[]){
   
    write(1,"Test", 4); // this works!

}
