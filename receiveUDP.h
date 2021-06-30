#ifndef _RECEIVE_UDP_H
#define _RECEIVE_UDP_H

#include "list.h"

// Receives a string through connectionless UDP
// IPv4 is exclusively used
// Starts a new pthread

void receiverInit(char* myP, List* l);
void receiverShutdown();

#endif