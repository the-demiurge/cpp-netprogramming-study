#ifndef NETWORK_PROGRAMMING_SINGLE_THREAD_TCP_SERVER_H
#define NETWORK_PROGRAMMING_SINGLE_THREAD_TCP_SERVER_H

#include <fstream>
#include <iostream>

#include "common_net.h"
#include "common_thread.h"
#include "common_protocols.h"


#define DEFAULT_PORT 5559
#define CONNECTION_QUEUE 100

FileTransferResult* isOK(FileHeader*, FileTransferResult*);

THREAD_RESULT handle_connection(void*);
#endif //NETWORK_PROGRAMMING_SINGLE_THREAD_TCP_SERVER_H
