#ifndef _NET_COMMON_
#define _NET_COMMON_

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#pragma pack(1)

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>

#endif // _WIN32
//Переводить рядкове представлення цільового хосту
//у числовий формат для Socket API
int resolve_host(const char*, sockaddr_in*);
#endif // _NET_COMMON_

