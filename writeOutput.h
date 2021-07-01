#ifndef _WRITE_OUTPUT_H
#define _WRITE_OUTPUT_H

#include "list.h"

// Writer module, will print to the screen
// Starts a new pthread

void writerSignaller();
void writerInit(List* l);
void writerCancel();
void writerShutdown();

#endif