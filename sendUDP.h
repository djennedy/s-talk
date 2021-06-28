#ifndef _SEND_UDP_H
#define _SEND_UDP_H

#include "list.h"

void senderInit(char* hostname, char* port, List* list);
void senderShutdown();

#endif