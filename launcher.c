#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "readInput.h"
#include "writeOutput.h"
#include "receiveUDP.h"
#include "sendUDP.h"

// Launches the s-talk program
int main (int argc, char * argv[])
{
    // Checks if arguments are correct
    if (argc!=4)
    {
        printf("Please enter valid arguments \n");
        printf("Valid argument strucure is: \n");
        printf("s-talk [my port number] [remote machine name] [remote port number]");
        return -1;
    }

    // Storing arguments
    char* myPort = argv[2];
    char* theirHostname = argv[3];
    char* theirPort = argv[4];

    // Creates a shared list
    List* list = List_create();

    // Initializes the four modules
    // Modules are running in an infinite loop, they will exit when given the exit code "!"
    readerInit(list);
    senderInit(theirHostname, theirPort, list);
    receiverInit(myPort, list);
    writerInit(list);

    // Cleans up the threads
    readerShutdown();
    senderShutdown();
    receiverShutdown();
    writerShutdown();

    // Frees the list
    // At this point, the list should already be empty
    List_free(list, free);

    return 0;
}