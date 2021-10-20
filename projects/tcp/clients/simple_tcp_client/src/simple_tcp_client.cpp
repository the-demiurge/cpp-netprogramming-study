#include "simple_tcp_client.h"

void exit_handler();

SOCKET client_socket;
int main(int argc, char* argv[])
{
    COMMAND_OPTIONS cmd_opts{"", 0};

    if (parse_cmd(argc, argv, &cmd_opts) && valid_connection_opts(&cmd_opts))
    {
        common_usage(argv[0]);
        return -1;
    }

    atexit(exit_handler);
    atexit(common_exit_handler);
    common_init_handler();

	client_socket = create_tcp_socket();
	if (client_socket <= 0) {
		error_msg("Can't create socket");
		return -1;
	}

	struct sockaddr_in server_addr;
	init_inet_address(&server_addr, cmd_opts.host, cmd_opts.port);

	//Connect to the server
	if (connect(client_socket, (sockaddr*)&server_addr, sizeof(sockaddr))) {
		char err_msg[128] = "";
		sprintf(err_msg, "Can't connect to the server %s:%d", cmd_opts.host, cmd_opts.port);
		error_msg(err_msg);
		return -1;
	}

	printf("Connection to the server %s:%d success\n", cmd_opts.host, cmd_opts.port);

	char msg[256] = "";
	printf("%s", "Enter msg:");
	//fgets(msg, sizeof(msg), stdin);
	scanf("%[^\n]s", msg);
	int sc = send(client_socket, msg, sizeof(msg), 0);
	if (sc <= 0) {
		char err_msg[128] = "";
		sprintf(err_msg, "Can't send data to the server %s:%d", cmd_opts.host, cmd_opts.port);
		error_msg(err_msg);
		return -1;
	}

	return 0;
}

void exit_handler()
{
	close_socket(client_socket);
}
