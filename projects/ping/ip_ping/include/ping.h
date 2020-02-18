#ifndef __PING_H__
#define __PING_H__

#include "common_net.h"
#include "icmp.h"
#include "ws2tcpip.h"

typedef struct _ping_cmd_opts {
    uint32_t packet_size;
    int record_route;
    int timeout;
    int ping_count;
} PING_CMD_OPTIONS, *PPING_CMD_OPTIONS;

#define SET_DEF_CMD_OPTS(cmd_opts)\
{\
  memset(&cmd_opts, 0, sizeof(cmd_opts));\
  cmd_opts.packet_size = DEFAULT_PACKET_SIZE;\
  cmd_opts.record_route = 0;\
  cmd_opts.timeout = 1000;\
  cmd_opts.ping_count = 4;\
}

#define CHECK_SOCK_OPT(ret, option)\
if (ret == SOCKET_ERROR)\
{\
	printf("setsockopt(%s) failed: %d\n", option, WSAGetLastError());\
	return -1;\
}

#define CHECK_SOCK_TRANSMIT(ret, transmit)\
if (ret == SOCKET_ERROR)\
{\
	if (WSAGetLastError() == WSAETIMEDOUT)\
	{\
		printf("timed out\n");\
		continue;\
	}\
	printf("%s failed: %d\n", transmit, WSAGetLastError());\
	return -1;\
}

#define CHECK_ALLOC_MEM(buffer, msg)\
if (!buffer)\
{\
  printf("Error allocate memory for %s\n", msg);\
  return -1;\
}

#define EPARSE_CMD_COUNT -2
#define EPARSE_CMD_ANY -1
#define EPARSE_CMD_OK 0

void parse_cmd_opts(const char*, PPING_CMD_OPTIONS);

int parse_cmd_opts(int argc, char* argv[], char*, PPING_CMD_OPTIONS);

void usage(char*);

#endif //_PING_H_
