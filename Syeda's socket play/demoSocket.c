// demo 
// following youtube demo "C Programming in Linux Tutorial #034 - Socket Programming"
// ^^ cite it : https://www.youtube.com/watch?v=pFLQkmnmD0o&list=PLypxmOPCOkHXbJhUgjRaV2pD9MJkIArhg&index=34

#include <stdio.h>
#include <sys/types.h> // textbook
#include <sys/socket.h> //textbook
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define PORT 22110

int main(int argCount,char* args[]){
    // variables//

    // brian no have//
    int mysocket; // s in tutoial // sock in vid
    char buffer[1024];
    int myAcceptingSocket;
    int rval;

    //brian had//
    struct sockaddr_in server; 

    //creating socket//
    mysocket = socket(AF_INET, SOCK_DGRAM, 0); //AF-> allows communication
    
    // brian had//
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);
    memset(&server,0, sizeof(server)); 

    //binding socket//

    if(bind(mysocket, (struct sockaddr* )&server, sizeof(server)) ){
        perror("this bind faild");
        exit(1);
    }
    

    //listening//

    listen(mysocket,5);


    //accept//
    do{
        myAcceptingSocket = accept(mysocket, (struct sockaddr *)0,0);
        if(myAcceptingSocket == -1){
            perror("could not accept, failed attempt");
        }
        else{
            memset(buffer, 0, sizeof(buffer));
            if(rval == recv(myAcceptingSocket, buffer, sizeof(buffer),0) < 0 ){
                perror("reading the message failed");
            }
            else if(rval==0){
                printf("ending connection");
            }
            else{
                printf("Message is: %s\n", buffer);
             
            }
            printf("We recieved the message: (rval = %d\n)", rval);
            close(myAcceptingSocket);
        }
    }
    while(1);

    return 0;
}