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
#include "sendUDP.h"

//Preparing variables to use
static int sockfd;
static struct addrinfo *servinfo;
static char* hostname;
static char* port;
static List* list;
static pthread_t senderThread;

static void* senderLoop(void* unused)
{
    struct addrinfo hints, *p;
    int gaiVal;
    int bindVal;
    int numbytes;
    char* message;
    // Setting up the hints addrinfo for the getaddrinfo function
    memset(&hints, 0 ,sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    //Getting address information with getaddrinfo
    gaiVal = getaddrinfo(hostname, port, &hints, &servinfo);
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
            perror("sender: socket() error");
            continue;
        }
        break;
    }
    // Error checking if no sockets are binded
    if(p==NULL)
    {
        fprintf(stderr, "sender: failed to create socket");
        exit(-1);
    }

    while(1)
    {
        // Getting message from list
        message = (char *) List_trim(list);
        
        // Sending
        numbytes = sendto(sockfd, message, strlen(message), 0, p->ai_addr, p->ai_addrlen);
        
        // De-allocating 
        free(message);

        // Error checking recvfrom
        if(numbytes ==-1)
        {
            perror("sender: sendto error");
            exit(-1);
        }
    }
    return NULL;
}

void senderInit(char* hostnm, char* p, List* l)
{
    hostname=hostnm;
    port = p;
    list = l;

    int stVal = pthread_create(&senderThread, NULL, senderLoop, NULL);
    if(stVal != 0)
    {
        perror("sender: thread creation error");
        exit(-1);
    }
    
}
void senderShutdown()
{

    // Freeing our results from getaddrinfo
    freeaddrinfo(servinfo);

    // closing connection to the socket
    close(sockfd);

    pthread_cancel(senderThread);
    pthread_join(senderThread, NULL);
}