#ifndef MULTI_THREAD_PORTSCANNER_H
#define MULTI_THREAD_PORTSCANNER_H

#include "common_net.h"
#include "common_thread.h"
#include <vector>

typedef struct tagClientOptions {
    char server_host[128];
    short server_port;
} CLIENT_OPTIONS, * PCLIENT_OPTIONS;

THREAD_RESULT scan_port(void* data);

int main(int argc, char* argv[]);

#endif //MULTI_THREAD_PORTSCANNER_H
