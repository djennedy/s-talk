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

#include <unistd.h>

#include "list.h"
#define MAXBUFLEN 65508

void writeScreenInit(char* hostname, char* port, List* list)
{


    printf("Testing write!");

    // char bufStorageOfMessage[MAXBUFLEN] = '\0'; 
    char bufWriteToScreen[MAXBUFLEN] = '\0'; // buffer to write to screen

    // read(list,bufStorageOfMessage,MAXBUFLEN);
    // List_search(bufStorageOfMessage);// find message in list


    /*
    List_first(list); // grabs the first message on the list
    for(int i = 0; i< MAXBUFLEN; i++){ // iterates through
        bufWriteToScreen = List_next(list);  // grabbing the message beside it as w
        write(0,bufWriteToScreen, MAXBUFLEN ); // writes the message out to screen
    }
    */


    // bufWriteToScreen = List_first(list); // grabs the first message on the list
    write(1,List_first(list), MAXBUFLEN ); // will put the message from first list onto screen
    




    //write(arg1, arg2, arg3,)//
    //1st arg -> 1 bc unknown file descriptor
    //2nd arg -> buffer you are going to write 
    //3rd arg -> max number it will write til




}



int main(int argCount,char* args[]){

    List* testingWriteList = List_create(); 
   

    printf("testing 1,2,3");
}
