#ifndef NETWORK_PROGRAMMING_NUMBER_TCP_SERVER_H
#define NETWORK_PROGRAMMING_NUMBER_TCP_SERVER_H

#include "common_net.h"

#define DEFAULT_PORT 5557
#define CONNECTION_QUEUE 100

void handle_connection(SOCKET, sockaddr_in*);
bool is_prime(int);

#endif //NETWORK_PROGRAMMING_NUMBER_TCP_SERVER_H
