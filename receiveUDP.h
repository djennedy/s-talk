#ifndef _RECEIVE_UDP_H
#define _RECEIVE_UDP_H

#include "list.h"

void receiverInit(char* hostnm, char* p, List* l);
void receiverShutdown();

#endif