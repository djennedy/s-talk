#ifndef _RECEIVE_UDP_H
#define _RECEIVE_UDP_H

#include "list.h"

void receiverInit(char* hostname, char* port, List* list);
void receiverShutdown();

#endif