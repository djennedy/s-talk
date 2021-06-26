// server demo

#include <stdio.h>
#include <sys/types.h> // textbook
#include <sys/socket.h> //textbook
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

int main(int argCount,char* args[]){


    #define MESSAGE  "Hello World Testing Socket"   

    // variables//
    int mySock; 
    struct sockaddr_in server; 
    struct hostent *hp;
    char buffer[1024];

    //creating socket//
    mySock = socket(AF_INET, SOCK_DGRAM, 0);
    if(mySock < 0 ){
        perror("socket failed");
        close(mySock);
        exit(1);
    }

    server.sin_family = AF_INET;
    hp = gethostbyname (argv[1]);
    if(hp == 0){
        perror("getting host name failed");
        close(mySock);
        exit(1);
    }

    memcpy(&server.sin_addr, hp->h_addr, hp->h_length); 
    server.sin_port = htons(5000);


    if(connect(mySock, (struct sockaaddr *)&server, sizeof(server))<0){
        perror("connection failed");
        close(mySock);
        exit(1);
    }

    if(send(mySock,MESSAGE, sizeof(MESSAGE),0)<0){
        perror("Sending faild");
        close(mySock);
        exit(1);
    }
    printf("Sent %s\n", MESSAGE);
    close(mySock);



    return 0;
}