#ifndef NETWORK_PROGRAMMING_MULTI_CAST_APP_H
#define NETWORK_PROGRAMMING_MULTI_CAST_APP_H

#include "common_net.h"
#include "ws2tcpip.h"

#define MCASTADDR "234.5.6.7"
#define MCASTPORT 25
#define BUFSIZE 10
#define BUFSIZE 10
#define DEFAULT_COUNT 50

typedef struct tagGroupOptions{
    long ip_interface;
    long group_ip_interface;
    short group_port;
    long repeat_count;
    bool is_sender;
    bool is_receiver;
    bool is_loop_back;
} MULTICAST_GROUP_OPTION, *PMULTICAST_GROUP_OPTION;


void parse_cmd_line(int argc, char **argv, PMULTICAST_GROUP_OPTION opts);

#endif //NETWORK_PROGRAMMING_MULTI_CAST_APP_H
