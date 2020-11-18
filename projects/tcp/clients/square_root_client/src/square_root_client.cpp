#include "square_root_client.h"

void exit_handler();

SOCKET client_socket;
void process_response(SquareRootResponse*);

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
	init_inet_address(&server_addr, host, port);

	//Connect to the server
	if (connect(client_socket, (sockaddr*)&server_addr, sizeof(sockaddr))) {
		char err_msg[128] = "";
		sprintf(err_msg, "Can't connect to the server %s:%d", host, port);
		error_msg(err_msg);
		return -1;
	}

	printf("Connection to the server %s:%d success\n", host, port);

	SquareRootRequest request;
	SquareRootResponse response;
	{
		printf("%s", "Enter a, b, c:");
		scanf("%lf %lf %lf", &request.a, &request.b, &request.c);
		int sc = send(client_socket, (char*)&request, sizeof(request), 0);
		if (sc <= 0) {
			char err_msg[128] = "";
			sprintf(err_msg, "Can't send data to the server %s:%d", host, port);
			error_msg(err_msg);
			return -1;
		}
	}

	{
		int sc = recv(client_socket, (char*)&response, sizeof(response), 0);
		if (sc <= 0) {
			char err_msg[128] = "";
			sprintf(err_msg, "Can't received data from the server %s:%d", host, port);
			error_msg(err_msg);
			return -1;
		}

		process_response(&response);
	}	

	return 0;
}

void process_response(SquareRootResponse* res) {
	switch (res->status) {
		case TWO_ROOTS:
			printf("Equation roots: x1=%.3f, x2=%.3f\n", res->x1, res->x2);
			break;
		case ONE_ROOTS:
			printf("Equation roots are identical: x1=x2=%.3f\n", res->x1);
			break;
 		case NO_ROOTS:
			printf("Equation has no roots\n");
			break;		 
	}
}

void exit_handler()
{
	close_socket(client_socket);
}
