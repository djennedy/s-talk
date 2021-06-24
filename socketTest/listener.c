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

// The port I chose to use (same as Brian's example), according to beej anything above 1024 is fair game;
#define MYPORT "22110"

// Max size of the message we'll send, in the real assignment I think it's not really stated how big the message could get
// But I think the TA said that UDP can handle up to 65,508 bytes (65,535 but 8 byte for UDP header and 20 for IP header)
#define MAXBUFLEN 1024

int main ()
{
    //Declaring variables (literally what beej does)
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int gaiVal;
    int bindVal;
    int numbytes;
    struct sockaddr_in theirAddr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET_ADDRSTRLEN];

    // Setting up the hints addrinfo for the getaddrinfo function
    memset(&hints, 0 ,sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    //Getting address information with getaddrinfo
    //You can replace the IP address with your hostname, this is how we'll do things in the assignment
    gaiVal = getaddrinfo("127.0.0.1", MYPORT, &hints, &servinfo);

    // Error checking for getaddrinfo
    if (gaiVal != 0 )
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(gaiVal));
        return 1;
    }

    // Initializing a socket and binding it to any port we find
    // Doing a for loop because getaddrinfo generates a linked list of addrinfos
    for(p = servinfo; p!=NULL; p=p->ai_next)
    {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        // Error checking for socket
        if (sockfd ==-1)
        {
            perror("listener: socket");
            continue;
        }

        bindVal = bind(sockfd, p->ai_addr, p->ai_addrlen);

        // Error checking for bind
        if(bindVal ==-1)
        {
            close(sockfd);
            perror("listener: bind");
            continue;
        }
        break;
    }

    // Error checking if no sockets are binded
    if(p==NULL)
    {
        fprintf(stderr, "listener: failed to bind socket");
        return 2;
    }

    // Freeing our results from getaddrinfo
    freeaddrinfo(servinfo);

    while(1)
    {
        printf("listener: waiting to recvfrom: ...\n");

        // Receiving
        addr_len = sizeof(theirAddr);
        numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0, (struct sockaddr* ) &theirAddr, &addr_len);
        
        // Error checking recvfrom
        if(numbytes ==-1)
        {
            perror("recvfrom");
            exit(1);
        }

        printf("listener: got packet from %s\n", inet_ntop(theirAddr.sin_family, &theirAddr, s, sizeof(s)));
        buf[numbytes]='\0';
        printf("listener: packet contains: %s\n", buf);
        
        // Sending to the process that sent us our last message
        char reply[]= "Message received \n\0";
        numbytes = sendto(sockfd, reply, strlen(reply),0, (struct sockaddr*)&theirAddr, addr_len);

        // Error checking sendto
        if(numbytes ==-1)
        {
            perror("sendto");
            exit(1);
        }
    }

    // Closing socket
    close(sockfd);

    return 0;

}