#ifndef NETWORK_PROGRAMMING_SIMPLE_TCP_CLIENT_H
#define NETWORK_PROGRAMMING_SIMPLE_TCP_CLIENT_H

#include <fstream>
#include <iostream>
#include <filesystem>

#include "common_net.h"
#include "common_protocols.h"
#include "common_thread.h"


#define DEFAULT_SERVER_HOST "127.0.0.1"
#define DEFAULT_SERVER_PORT 5559


struct ClientData {
    SOCKET socket;
    FileTransferRequest request;
};

THREAD_RESULT send_and_process(void*);

void process_response(FileTransferResponse*);


#endif //NETWORK_PROGRAMMING_SIMPLE_TCP_CLIENT_H
