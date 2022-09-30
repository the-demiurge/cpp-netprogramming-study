#include "mt_square_root_client.h"

void exit_handler();

SOCKET client_socket = 0;


int main(int argc, char* argv[])
{
	COMMAND_OPTIONS cmd_opts{ DEFAULT_SERVER_HOST, DEFAULT_SERVER_PORT};

	parse_cmd(argc, argv, &cmd_opts);

	if (!valid_connection_opts(&cmd_opts))
	{
		common_usage(argv[0]);
		return -1;
	}

	atexit(common_exit_handler);
	atexit(exit_handler);

	common_init_handler();

	CHECK_IO((client_socket = create_tcp_socket()) > 0, -1, "Error create client socket");

	struct sockaddr_in server_addr;
	init_inet_address(&server_addr, cmd_opts.host, cmd_opts.port);
	//Connect to the server
	if (connect(client_socket, (sockaddr*)&server_addr, sizeof(sockaddr))) {
		char err_msg[512] = "";
		sprintf(err_msg, "Can't connect to the server %s:%d", cmd_opts.host, cmd_opts.port);
		error_msg(err_msg);
		return -1;
	}

	printf("Connection to the server %s:%d success\n", cmd_opts.host, cmd_opts.port);

	
	SquareRootRequest requests[] = {
		{-1, 2, 1},
		{3, -1, 4},
		{-2, 2, 1}
	};
	
	for (int i = 0; i < sizeof(requests) / sizeof(SquareRootRequest); ++i) {
		ClientHeaderData clientData{client_socket, requests[i] };
		create_thread(send_and_process, (void*)&clientData);
	}

    current_thread_sleep(5000);
	
	return 0;
}

void exit_handler()
{
	if (client_socket > 0) {
		close_socket(client_socket);
	}
	
}
