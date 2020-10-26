#include "number_prime_tcp_client.h"

void exit_handler();

SOCKET client_socket;

int main(int argc, char* argv[])
{
	atexit(common_exit_handler);
	atexit(exit_handler);
	short port;
	char host[128] = "";
	bool parse_cmd_result = parse_cmd(argc, argv, host, &port);

	if (!parse_cmd_result || !host || !strlen(host))
	{
		printf("Invalid host or port. Usage %s -h host -p port\n", argv[0]);
		return -1;
	}

	common_init_handler();

	client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_socket <= 0) {
		error_msg("Can't create socket");
		return -1;
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(host);

	//Connect to the server
	if (connect(client_socket, (sockaddr*)&server_addr, sizeof(sockaddr))) {
		char err_msg[128] = "";
		sprintf(err_msg, "Can't connect to the server %s:%d", host, port);
		error_msg(err_msg);
		return -1;
	}

	printf("Connection to the server %s:%d success\n", host, port);

	int r;
	printf("%s", "Enter a number to check for its prime:");
	if (scanf("%d", &r) != 1)
	{
		printf("Invalid input\n");
		return -1;
	}

	int sc = send(client_socket, (char*)&r, sizeof(r), 0);
	if (sc <= 0) {
		char err_msg[128] = "";
		sprintf(err_msg, "Can't send data to the server %s:%d", host, port);
		error_msg(err_msg);
		return -1;
	}

	{
		char resp[256] = "";
		sc = recv(client_socket, resp, sizeof(resp), 0);
		if (sc <= 0)
		{
			printf("Server is not able\n");
			return -1;
		}
		printf("%s", resp);
	}

	close_socket(client_socket);

	return 0;
}

void exit_handler()
{
    close_socket(client_socket);
}
