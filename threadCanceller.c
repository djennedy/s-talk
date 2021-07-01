#include "readInput.h"
#include "writeOutput.h"
#include "sendUDP.h"
#include "receiveUDP.h"
#include "threadCanceller.h"

void cancelReceiverWriter()
{
    receiverCancel();
    writerCancel();
}
void cancelReaderSender()
{
    readerCancel();
    senderCancel();
}