#ifndef _SEND_UDP_H
#define _SEND_UDP_H

#include "list.h"

// Sends a string through connectionless UDP
// IPv4 is exclusively used
// Starts a new pthread

void senderInit(char* hostnm, char* p, List* l);
void senderShutdown();
void sendSignaller();
static void* senderLoop(void* unused);

#endif