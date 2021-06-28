//screen, 
// reads off keyboard and adds to list


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

void readKeyboardInit(char* hostname, char* port, List* list)
{


printf("Testing read!");



// said below was right idea..//


char bufStorageOfMessage[MAXBUFLEN] = '\0'; 
// bufStorageOfMessage = '\0';
// char bufStorageOfMessage[MAXBUFLEN] ="Testing buffer readInput \0";  

read(0,bufStorageOfMessage, MAXBUFLEN -1);
    // first arg -> reads from file descriptor, not known so use 0
    // second arg -> starts from the buff, place to store the input value
    // third arg -> end the read at this value, max amount allowed
// printf("This message was added to buffer ---> : %s\n", bufStorageOfMessage);
List_perpend(list, bufStorageOfMessage);





}



int main(int argCount,char* args[]){

    List* testingReadList = List_create(); 
    readInit("127.0.0.1", "22110",testingReadList);

    printf("testing 1,2,3");
}
