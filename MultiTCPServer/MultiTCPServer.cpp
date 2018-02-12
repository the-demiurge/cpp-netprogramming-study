#include "stdafx.h"

#define DEFAULT_PORT 5557
#define CONNECTION_QUEUE 100

#define CHECK_IO(io, err_msg, err_code, ...) \
if (!(io))\
{\
	printf(err_msg, __VA_ARGS__); \
	return err_code; \
}

#define CHECK_VOID_IO(io, err_msg, ...) \
if (!(io))\
{\
	printf(err_msg, __VA_ARGS__); \
	return; \
}

bool parse_cmd(int argc, char* argv[], char* host, short* port);
void handle_connection(void*);
void error_msg(const char*);
void exit_handler();

SOCKET server_socket;
std::vector<HANDLE> connection_pool;
int main(int argc, char* argv[])
{
	atexit(exit_handler);
	short port = DEFAULT_PORT;
	char host[128] = "";
	bool parse_cmd_result = parse_cmd(argc, argv, host, &port);

	WSADATA ws;
	CHECK_IO(!WSAStartup(MAKEWORD(2, 2), &ws), "Error init of WinSock2", -1);

	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	CHECK_IO((server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) > 0, "Can't create socket", -1);

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	if (parse_cmd && strlen(host) > 0) {
		server_addr.sin_addr.s_addr = inet_addr(host);
	}
	else {
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}

	//Bind socket to the address on the server
	CHECK_IO(!bind(server_socket, (sockaddr*)&server_addr, sizeof(sockaddr)), "Can't bind socket to the port %d", -1, port);
	//Set socket as passive
	CHECK_IO(!listen(server_socket, CONNECTION_QUEUE), "Error listening socket", -1);

	printf("Server running at the port %d\n", port);

	while (true)
	{
		sockaddr_in incom_addr;
		memset(&incom_addr, 0, sizeof(incom_addr));
		int len = sizeof(incom_addr);
		SOCKET socket;
		CHECK_IO((socket = accept(server_socket, (sockaddr*)&incom_addr, &len)) > 0, "Can't accept connection", -1);
		DWORD hThreadId;
		connection_pool.push_back(
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)handle_connection, (LPVOID)socket, 0, &hThreadId)
		);
	}

	closesocket(server_socket);

	return 0;
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


	const int count_vars = 3;
	const int host_buf_sz = 128;
	int tmp_ports[count_vars] = { -1, -1, -1 };
	char tmp_hosts[count_vars][host_buf_sz];
	for (int i = 0; i < count_vars; ++i) {
		memset(tmp_hosts[i], 0, host_buf_sz);
	}
	char* formats[count_vars] = { "-h %s -p %d", "-p %d -h %s", "-p %d" };

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
				*port = (short)tmp_ports[i];
				return true;
			}
		}
	}

	return false;

}

void handle_connection(void* psocket) {
	CHECK_VOID_IO(psocket, "Empty connection thread data");
	SOCKET socket;
	CHECK_VOID_IO((socket = (SOCKET)psocket) > 0, "Invalid connection thread data");
	sockaddr_in addr;
	int addr_len = sizeof(addr);
	CHECK_VOID_IO(!getpeername(socket, (sockaddr*)&addr, &addr_len), "Error retrieving peer info");
	char* str_in_addr = inet_ntoa(addr.sin_addr);
	printf("[%s:%d]>>%s\n", str_in_addr, ntohs(addr.sin_port), "Establish new connection");
	while (true) {
		char buffer[256] = "";
		int rc = recv(socket, buffer, sizeof(buffer), 0);
		if (rc > 0) {
			printf("[%s]:%s\n", str_in_addr, buffer);
		}
		else {
			break;
		}
	}
	closesocket(socket);
	printf("[%s]>>%s", str_in_addr, "Close incomming connection");
}

void exit_handler()
{
	closesocket(server_socket);
	WSACleanup();
}