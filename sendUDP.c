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
#include "sendUDP.h"
 
//Preparing variables to use
static int sockfd;
static struct addrinfo *servinfo;
static char* theirHostname;
static char* theirPort;
static List* list;
static pthread_t senderThread;
static char* message;
 
 
static pthread_mutex_t sendAvailableCondMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t sendAvailableCond = PTHREAD_COND_INITIALIZER; // initlizing con here
 
 
static void* senderLoop(void* unused)
{
    struct addrinfo hints, *p;
    int gaiVal;
    int numbytes;
    // Setting up the hints addrinfo for the getaddrinfo function
    memset(&hints, 0 ,sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
 
    // Getting address information with getaddrinfo
    // Sending to theirHostname, on theirPort
    gaiVal = getaddrinfo(theirHostname, theirPort, &hints, &servinfo);
    // Error checking for getaddrinfo
    if (gaiVal != 0 )
    {
        fprintf(stderr, "sender: getaddrinfo error: %s\n", gai_strerror(gaiVal));
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
        //Waits until notified by the senderSignaller = pthread_cond_wait
        pthread_mutex_lock(&sendAvailableCondMutex);
        {
            pthread_cond_wait(&sendAvailableCond, &sendAvailableCondMutex);
        }
        pthread_mutex_unlock(&sendAvailableCondMutex);

        int iteration = 0;

        do
        {
            iteration++;

            // Getting message from list
            message = dequeueMessage(list);

            if(message==NULL)
            {
                fprintf(stderr, "sender: dequeue error, queue empty.\n");
                break;
            }
        
            // Sending
            numbytes = sendto(sockfd, message, strlen(message), 0, p->ai_addr, p->ai_addrlen);

            // Check for exit code
            // Ends the thread if exit code is passed in the first iteration of the read
            if(!strcmp(message,"!\n") && iteration==1)
            {
                free(message);
                message = NULL;
                return NULL;
            }
            
            // De-allocating message
            free(message);
            message = NULL;
        
            // Error checking recvfrom
            if(numbytes ==-1)
            {
                perror("sender: sendto error");
                exit(-1);
            }
        } while (countMessages(list)!=0);
    }
    return NULL;
}
 
void senderSignaller(){
 
    //Signals other thread,send thread, waiting on condition variable
    pthread_mutex_lock(&sendAvailableCondMutex);
    {
        pthread_cond_signal(&sendAvailableCond); // signals another thread waiting
    }
    pthread_mutex_unlock(&sendAvailableCondMutex);
  
}

void senderCancel()
{
    pthread_cancel(senderThread);
}
 
 
void senderInit(char* hostnm, char* p, List* l)
{
    theirHostname=hostnm;
    theirPort = p;
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
    // De-allocating dynamically allocated message if shutdown is called while message is not yet freed
    // Note: if we HAVE already freed the pointer, then we've set the message pointer to NULL
    // and it is okay to free a NULL pointer (it does nothing)
    free(message);
    message=NULL;
    
    // Freeing our results from getaddrinfo
    freeaddrinfo(servinfo);
    
    // closing connection to the socket
    close(sockfd);
    
    pthread_join(senderThread, NULL); 
}