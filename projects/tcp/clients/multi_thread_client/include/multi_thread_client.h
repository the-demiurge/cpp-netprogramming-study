#ifndef NETWORK_PROGRAMMING_MULTI_THREAD_CLIENT_H
#define NETWORK_PROGRAMMING_MULTI_THREAD_CLIENT_H

#include "common_net.h"
#include "common_thread.h"

#include <vector>

#define DEFAULT_PORT 5557

typedef struct tagClientOptions {
    char server_host[128];
    short server_port;
    char data[255];
} CLIENT_OPTIONS, *PCLIENT_OPTIONS;

THREAD_RESULT process_connection(void*);
#endif //NETWORK_PROGRAMMING_MULTI_THREAD_CLIENT_H
