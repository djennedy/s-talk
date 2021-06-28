// using part of Darrn's check + addrino info since its more accurate




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

// Max size of the message, theoretical max size of a UDP packet in IPv4.
#define MAXBUFLEN 65508

// variable set up
static int sockRead;
static struct addrinfo hints, *servinfo, *p;
static int gaiVal;
static int bindVal;
static int readBytes;
char buf[MAXBUFLEN];
static struct sockaddr_in their_addr;
static socklen_t messageLen;
static char s[INET_ADDRSTRLEN];
char* message;

void receiverInit(char* hostname, char* port, List* list)
{

   //set up
    memset(&hints, 0 ,sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    //using Darren's non hard coded values
    gaiVal = getaddrinfo(hostname, port, &hints, &servinfo);

    // Error checking for getaddrinfo
    if (gaiVal != 0 )
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(gaiVal));
        return;
    }

    //crating socket + checks
    for(p = servinfo; p!=NULL; p=p->ai_next)
    {
        sockRead = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        // Error checking for socket
        if (sockRead ==-1)
        {
            perror("receiver: socket() error");
            continue;
        }


        // binding socket
        bindVal = bind(sockRead, p->ai_addr, p->ai_addrlen);
        // Error checking for bind
        if(bindVal ==-1)
        {
            close(sockRead);
            perror("receiver: bind() error in receiver");
            continue;
        }
        break;
    }

     // Error checking if no sockets are binded
    if(p==NULL)
    {
        fprintf(stderr, "receiver: failed to bind socket");
        return;
    }

    // Freeing our results from getaddrinfo
    freeaddrinfo(servinfo);

    while(1)
    {
        // Receiving
        messageLen = sizeof(their_addr);
        readBytes = read(sockRead,buf , MAXBUFLEN ); 
        // first arg -> reads from file describtor, sockRead
        // second arg -> starts from the buff
        // third arg -> end the read at this value, max amount allowed

        buf[readBytes]='\0'; // not sure what this does

        // adding buff to list, 
        message = (char*)malloc(sizeof(char)*(readBytes+1)); // allocates space for new item
        strncpy(message, buf, readBytes);
        message[readBytes] = '\0'; //not sure what this does

        // Adding message to the front of list
        List_prepend(list, message);
    }

}
void receiverShutdown()
{
    close(sockRead);
}