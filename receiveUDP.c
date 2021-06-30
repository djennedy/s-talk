// Code is adapted (not copied) from Brian Fraser's Workshops and Beej's Guide to Network Programming examples
// IPv4 is exclusively used here

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
#include "queueOperations.h"
#include "writeOutput.h"
#include "receiveUDP.h"

// Max size of the message, theoretical max size of a UDP packet in IPv4.
#define MAXBUFLEN 65508

// Preparing variables we'll use
static int sockfd;
static struct addrinfo *servinfo;
static char* myPort;
static List* list;
static pthread_t receiverThread;

static void* receiverLoop (void* unused)
{
    // Preparing variables we'll use
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int gaiVal;
    int bindVal;
    int numbytes;
    char buf[MAXBUFLEN];
    char* message;
    struct sockaddr_in their_addr;
    socklen_t addr_len;
    char s[INET_ADDRSTRLEN];

    // Setting up the hints addrinfo for the getaddrinfo function
    memset(&hints, 0 ,sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    // Getting address information with getaddrinfo
    // Listening on our machine, on myPort
    gaiVal = getaddrinfo(NULL, myPort, &hints, &servinfo);

    // Error checking for getaddrinfo
    if (gaiVal != 0 )
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(gaiVal));
        exit(-1);
    }

    // Initializing a socket and binding it to any port we find
    // Doing a for loop because getaddrinfo generates a linked list of addrinfos
    for(p = servinfo; p!=NULL; p=p->ai_next)
    {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        // Error checking for socket
        if (sockfd ==-1)
        {
            perror("receiver: socket() error");
            continue;
        }

        bindVal = bind(sockfd, p->ai_addr, p->ai_addrlen);

        // Error checking for bind
        if(bindVal ==-1)
        {
            close(sockfd);
            perror("receiver: bind() error");
            continue;
        }
        break;
    }

     // Error checking if no sockets are binded
    if(p==NULL)
    {
        fprintf(stderr, "receiver: failed to bind socket");
        exit(-1);
    }

    // Freeing our results from getaddrinfo
    freeaddrinfo(servinfo);

    while(1)
    {
        // Receiving
        addr_len = sizeof(their_addr);
        numbytes = recvfrom(sockfd, buf, MAXBUFLEN, 0, (struct sockaddr* ) &their_addr, &addr_len);
        
        // Error checking recvfrom
        if(numbytes ==-1)
        {
            perror("receiver: recvfrom error");
            exit(-1);
        }

        // Copying buf to a smaller string to put to the list
        message = (char*)malloc(sizeof(char)*(numbytes+1));
        strncpy(message, buf, numbytes);
        message[numbytes] = '\0';

        // Adding message to the list
        enqueueMessage(list, message);

        // Signals writer to write message to screen
        writerSignaller();

        if(!strcmp(message, "!"))
        {
            return;
        }
    }
    return NULL;
}

void receiverInit(char* myP, List* l)
{
    myPort = myP;
    list = l;

    int rectVal = pthread_create(&receiverThread, NULL, receiverLoop, NULL);
    if(rectVal != 0)
    {
        perror("sender: thread creation error");
        exit(-1);
    }
}
void receiverShutdown()
{
    // Freeing our results from getaddrinfo
    freeaddrinfo(servinfo);

    // closing connection to the socket
    close(sockfd);

    pthread_join(receiverThread, NULL);
}