#include "udp_echo_receiver.h"

void exit_handler();

SOCKET receiver_socket;

int main(int argc, char *argv[]) {	
    atexit(common_exit_handler);
	atexit(exit_handler);

	COMMAND_OPTIONS cmd_opts{ "", 0 };

	if (!parse_cmd(argc, argv, &cmd_opts) || parse_cmd(argc, argv, &cmd_opts) && !valid_connection_opts(&cmd_opts))
	{
		common_usage(argv[0]);
		return -1;
	}

    common_init_handler();

    receiver_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (receiver_socket <= 0) {
        error_msg("Can't create socket");
        return -1;
    }

    sockaddr_in receiver_addr;
    init_inet_address(&receiver_addr, cmd_opts.host, cmd_opts.port);

	//Bind socket to the address on the server
	if (bind(receiver_socket, (sockaddr *)&receiver_addr, sizeof(sockaddr))) {
		char err_msg[128] = "";
		sprintf(err_msg, "Can't bind socket to the port %d", cmd_opts.port);
		error_msg(err_msg);
		return -1;
	}

    printf("Receiver available on the port %d\n", cmd_opts.port);

    while (process_receiv_data(receiver_socket)) {}

    return 0;
}

void exit_handler() {
    close_socket(receiver_socket);
}