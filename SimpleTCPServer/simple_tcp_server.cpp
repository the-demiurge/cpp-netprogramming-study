// SimpleTCPServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

bool parse_cmd(int argc, char* argv[], char* host, unsigned short port);
void error_msg(const char*);
void exit_handler();

int main(int argc, char* argv[])
{
	atexit(exit_handler);
	unsigned short port = 0;
	char host[128] = "";

	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws)) {
		error_msg("Error init of WinSock2");
		return -1;
	}

	SOCKET server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_sock <= 0) {
		error_msg("Can't create socket");
		return -1;
	}

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons()
    return 0;
}

bool parse_cmd(int argc, char* argv[], char* host, unsigned short port)
{
	char all_args[256];
	memset(all_args, 0, sizeof all_args);
	
	for (int i = 1; i < argc; ++i) {
		strcat(all_args, argv[i]);
	}
	printf("Argsss %s\n", all_args);

	
	const int count_vars = 3;
	const int host_buf_sz = 128;
	int tmp_ports[count_vars] = { -1, -1, -1 };
	char tmp_hosts[count_vars][host_buf_sz];
	for (int i = 0; i < count_vars; ++i) {
		memset(tmp_hosts[i], 0, host_buf_sz);
	}
	char* formats[count_vars] = { "-h%s-p%d", "-p%d-h%s", "-p%d" };
	
	int results[] = {
		sscanf(all_args, formats[0], tmp_hosts[0], &tmp_ports[0]) - 2,
		sscanf(all_args, formats[1], &tmp_ports[1], tmp_hosts[1]) - 2,
		sscanf(all_args, formats[2], &tmp_ports[2]) - 1
	};

	for (int i = 0; i < sizeof(results) / sizeof(int); ++i) {
		if (!results[i]) {
			if (strlen(tmp_hosts[i]) > 0) {
				strcpy(host, tmp_hosts[i]);
			}
			if (tmp_ports[i] > 0) {
				port = (unsigned short)tmp_ports[i];
				return true;
			}
		}
	}

	return false;
	
}

void error_msg(const char* msg) {
	printf("%s\n", msg);
}

void exit_handler()
{
	WSACleanup();
}

