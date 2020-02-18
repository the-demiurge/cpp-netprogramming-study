#ifndef NETWORK_PROGRAMMING_COMMON_NET_H
#define NETWORK_PROGRAMMING_COMMON_NET_H

#define CHECK_IO(io, err_code, ...) \
if (!(io)) \
{ \
	printf(__VA_ARGS__); \
	return err_code; \
}

#define CHECK_VOID_IO(io, ...) \
if (!(io))\
{\
	printf(__VA_ARGS__); \
	return; \
}

#define CHECK_SET_OPT(opt_ret, err_msg)\
{\
  if ((opt_ret) < 0)\
  {\
    printf("setsockopt(%s) failed with: %d\n",\
           err_msg, WSAGetLastError());\
    return -1;\
  }\
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

#include <tchar.h>
#include <winsock2.h>
#include <windows.h>

#endif

int close_socket(int socket);

bool resolve_addr(char*, in_addr*);
bool parse_cmd(int argc, char* argv[], char* host, short* port);

int common_init_handler();

void common_exit_handler();

void error_msg(const char*);

#endif //NETWORK_PROGRAMMING_COMMON_NET_H
