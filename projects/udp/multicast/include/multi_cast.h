#ifndef NETWORK_PROGRAMMING_MULTI_CAST_APP_H
#define NETWORK_PROGRAMMING_MULTI_CAST_APP_H

#include "common_net.h"
#include "common_utils.h"
#ifdef _WIN32
#include "ws2tcpip.h"
#endif

#include <ctype.h>

#define MCASTADDR "234.5.6.7"
#define MCASTPORT 5675
#define BUFSIZE 255
#define DEFAULT_COUNT 50

typedef struct tagGroupOptions{
    char ip_interface[16];
    char group_ip_interface[16];
    short group_port;
    long repeat_count;
    bool is_sender;
    bool is_receiver;
    bool is_loop_back;
    int receive_timeout;
} MULTICAST_GROUP_OPTION, *PMULTICAST_GROUP_OPTION;


void parse_cmd_line(int argc, char **argv, PMULTICAST_GROUP_OPTION opts);

int group_sender(PMULTICAST_GROUP_OPTION, SOCKET, struct sockaddr_in*);
int group_receiver(PMULTICAST_GROUP_OPTION, SOCKET);

#endif //NETWORK_PROGRAMMING_MULTI_CAST_APP_H
