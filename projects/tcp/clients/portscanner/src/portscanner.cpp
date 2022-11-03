#include "portscanner.h"

void exit_handler();

SOCKET client_socket;
int main(int argc, char* argv[])
{
	char host[256];
	int start_port;
	int end_port;
	struct sockaddr_in server_addr;
	
	printf("Enter ip: ");
	scanf("%s", host);

	printf("Enter starting port: ");
	scanf("%d", &start_port);

	printf("Enter ending port: ");
	scanf("%d", &end_port);

	atexit(common_exit_handler);
	atexit(exit_handler);
	common_init_handler();

	client_socket = create_tcp_socket();
	if (client_socket <= 0) {
		error_msg("Can't create socket");
		return -1;
	}

	
	for (int i = start_port; i < (end_port+1); i++)
	{
		
		init_inet_address(&server_addr, host, i);
		//Connect to the server
		if (connect(client_socket, (sockaddr*)&server_addr, sizeof(sockaddr)) < 0) {
			printf("Port %d is closed\n", i);
		}
		else {
			printf("Port %d is open\n", i);
		}
	}

	
	//char msg[256] = "";
	//printf("%s", "Enter msg:");
	////fgets(msg, sizeof(msg), stdin);
	//scanf("%[^\n]s", msg);
	//int sc = send(client_socket, msg, sizeof(msg), 0);
	//if (sc <= 0) {
	//	char err_msg[512] = "";
	//	sprintf(err_msg, "Can't send data to the server %s:%d", cmd_opts.host, cmd_opts.port);
	//	error_msg(err_msg);
	//	return -1;
	//}

	return 0;
}

void exit_handler()
{
	close_socket(client_socket);
}
