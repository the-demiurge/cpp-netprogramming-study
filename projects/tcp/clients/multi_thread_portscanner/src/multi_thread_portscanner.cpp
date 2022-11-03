#include "multi_thread_portscanner.h"

void exit_handler();

SOCKET client_socket;
std::vector<THREAD_HANDLE> connection_pool;

THREAD_RESULT scan_port(void* data) {
	PCLIENT_OPTIONS poptions = (PCLIENT_OPTIONS)data;

	SOCKET client_socket = -1;
	CHECK_IO((client_socket = create_tcp_socket()) > 0, (THREAD_RESULT)-1, "Can't create client socket\n");

	struct sockaddr_in server_addr;
	
	init_inet_address(&server_addr, poptions->server_host, poptions->server_port);
	//Connect to the server
	
	if (connect(client_socket, (sockaddr*)&server_addr, sizeof(sockaddr)) < 0) {
		printf("Port %d is closed\n", poptions->server_port);
	}
	else {
		printf("Port %d is open\n", poptions->server_port);
	}
	
	close_socket(client_socket);
	return 0;
}

int main(int argc, char* argv[])
{
	char host[256];
	int start_port;
	int end_port;
	
	printf("Enter ip: ");
	scanf("%s", host);

	printf("Enter starting port: ");
	scanf("%d", &start_port);

	printf("Enter ending port: ");
	scanf("%d", &end_port);

	atexit(common_exit_handler);
	atexit(exit_handler);
	common_init_handler();

	//client_socket = create_tcp_socket();
	//if (client_socket <= 0) {
	//	error_msg("Can't create socket");
	//	return -1;
	//}

	const int port_num = end_port - start_port + 1;
	CLIENT_OPTIONS options[port_num];
	//HANDLE hThreadArray[port_num];
	for (int i = start_port; i < end_port+1; ++i) {
		int popt_num = 0;
		PCLIENT_OPTIONS poptions = &options[popt_num];
		strcpy(poptions->server_host, host);
		poptions->server_port = i;
		//hThreadArray[popt_num] = create_thread(scan_port, poptions);
		connection_pool.push_back(
			create_thread(scan_port, poptions)
		);
		++popt_num;
		//WaitForMultipleObjects(port_num, hThreadArray, TRUE, INFINITE);
	}

	wait_thread(connection_pool.data(), connection_pool.size());
	

	return 0;
}

void exit_handler()
{
	close_socket(client_socket);
}
