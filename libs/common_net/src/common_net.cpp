#include "common_net.h"

int common_init_handler()
{
#ifdef _WIN32
    WSADATA ws;
	CHECK_IO(WSAStartup(MAKEWORD(2, 2), &ws) == 0, -1, "Error init of WinSock2\n");
	return 0;
#elif __linux__
    return 0;
#else
#error "Unsupported platform"
#endif
}

void common_exit_handler() {
#ifdef _WIN32
    CHECK_VOID_IO(WSACleanup() == 0, "Error shutdown of WinSock2\n");
#elif __linux__
    return;
#else
#error "Unsupported platform"
#endif
}

bool resolve_addr(const char* str_addr, in_addr* baddr)
{
    long ip = inet_addr(str_addr);
	if (ip > 0) {
		baddr->s_addr = ip;
        return true;
    }
    else {
        hostent* hp = gethostbyname(str_addr);
        if (hp == NULL)
        {
            fprintf(stderr, "Error host %s", str_addr);
            return false;
        }
		
		memcpy(&(baddr->s_addr), hp->h_addr_list[0], hp->h_length);
        return true;
    }
}

bool parse_cmd(int argc, char* argv[], char* host, short* port)
{
    if (argc < 2) {
        return false;
    }

    char all_args[256];
    memset(all_args, 0, sizeof all_args);

    for (int i = 1; i < argc; ++i) {
        strcat(all_args, argv[i]);
        strcat(all_args, " ");
    }


    struct FormatOptions {
        char format[16];
        int require_cn;
        int order;
    };

    FormatOptions format_opts[] = {
        {"-h %s -p %d", 2, 0},
        {"-p %d -h %s", 2, 1},
        {"-h %s:%d", 2, 0},
        {"-p %d", 1, 0}

    };

    const int opt_counts = sizeof(format_opts)/sizeof(FormatOptions);

    short tmp_port = -1;
    char tmp_host[128];
    int ret = 0;
    int done = -1;
    for (int i = 0; done != 0 && i < opt_counts; ++i) {
        FormatOptions format_opt = format_opts[i];    
        memset(tmp_host, 0, sizeof(tmp_host));
        
        if (format_opt.require_cn == 1) {
            ret = sscanf(all_args, format_opt.format, &tmp_port);
        }
        else {            
            if (format_opt.order == 1) {
                ret = sscanf(all_args, format_opt.format, &tmp_port, tmp_host);
            }
            else if (format_opt.order == 0) {
                ret = sscanf(all_args, format_opt.format, tmp_host, &tmp_port);
            }
        }
        
        done = ret - format_opt.require_cn;             
    }

    if (done == 0) {
        if (tmp_port > 0) {
            *port = tmp_port;
        }

        if (host && strlen(tmp_host) > 0) {
            strcpy(host, tmp_host);
        }
    }

    return done;

}

int close_socket(int socket) {
#ifdef _WIN32
    CHECK_IO(closesocket(socket) == 0, -1, "Error close socket\n");
    return 0;
#elif __linux__
    return close(socket);
#else
 #error "Unsupported platform"
#endif
}

void error_msg(const char* msg) {
    printf("%s\n", msg);
}

struct sockaddr_in *init_inet_address(struct sockaddr_in *address, const char *host, const short port) {
    if (!address || port <= 0)
    {
        return NULL;
    }
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    if (host && strlen(host) > 0) {
        struct in_addr addr;
        resolve_addr(host, &addr);
        address->sin_addr.s_addr = addr.s_addr;
    } else {
        address->sin_addr.s_addr = htonl(INADDR_ANY);
    }

    return address;
}

int set_reuse_address(SOCKET socket, int reuse) {
    return setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(int));
}

int set_group_loopback(SOCKET socket, int loopback) {
    return setsockopt(socket, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&loopback, sizeof(int));
}

int set_group_address(SOCKET socket, const char * address) {
    struct in_addr addr;
    memset(&addr, 0, sizeof(addr));
    resolve_addr((char*)address, &addr);
    return setsockopt(socket, IPPROTO_IP, IP_MULTICAST_IF, (char*)&addr, sizeof(addr));
}

int init_group(const char * local_addr, const char * group_addr, struct ip_mreq * group) {
    struct in_addr addr;

    if (local_addr && strlen(local_addr) > 0) {
        memset(&addr, 0, sizeof(addr));
        if (!resolve_addr((char*)local_addr, &addr)) {
            return -1;
        }
        group->imr_interface.s_addr = addr.s_addr;
    } else {
        group->imr_interface.s_addr = htonl(INADDR_ANY);
    }

    if (group_addr && strlen(group_addr) > 0) {
        memset(&addr, 0, sizeof(addr));
        if (!resolve_addr((char*)group_addr, &addr)) {
            return -1;
        }
        group->imr_multiaddr.s_addr = addr.s_addr;
    } else {
        return -1;
    }

    return 0;
}

int join_group(SOCKET socket, struct ip_mreq * group) {
    if (!group) {
        return -1;
    }
    return setsockopt(socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)group, sizeof(ip_mreq));
}

int leave_group(SOCKET socket, struct ip_mreq * group) {
    if (!group) {
        return -1;
    }
    return setsockopt(socket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)group, sizeof(ip_mreq));
}

int set_group_ttl(SOCKET socket, int ttl) {
    return setsockopt(socket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl, sizeof(int));
}

int set_loopback(SOCKET socket, int turn_on) {
    return setsockopt(socket, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&turn_on, sizeof(int));
}

int bind_socket_to(SOCKET socket, struct sockaddr_in * sockaddr) {
    return bind(socket, (struct sockaddr*)sockaddr, sizeof(sockaddr_in));
}

int set_recv_timeout_ms(SOCKET socket, int millis) {
    if (millis <= 0) {
        return 0;
    }
#ifdef _WIN32
    DWORD timeout = millis * 1000;
#elif __linux__
    struct timeval timeout;
    timeout.tv_sec = millis / 1000;
    timeout.tv_usec = (millis % 1000) * 1000;
#else
  #error "Unsupported platform for setsockopt"
#endif
    return setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
}
