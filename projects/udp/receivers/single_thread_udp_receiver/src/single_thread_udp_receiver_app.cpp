#include "single_thread_udp_receiver.h"

void exit_handler();

SOCKET receiver_socket;

int main(int argc, char *argv[]) {	
    COMMAND_OPTIONS cmd_opts{"", DEFAULT_PORT};

    parse_cmd(argc, argv, &cmd_opts);

    atexit(exit_handler);
    atexit(common_exit_handler);
    common_init_handler();

    CHECK_IO((receiver_socket = create_udp_socket()) > 0, -1, "Can't create socket\n");

    struct sockaddr_in receiver_addr;
    init_inet_address(&receiver_addr, cmd_opts.host, cmd_opts.port);

	//Bind socket to the address on the server
	if (bind(receiver_socket, (sockaddr *)&receiver_addr, sizeof(sockaddr))) {
		char err_msg[128] = "";
		sprintf(err_msg, "Can't bind socket to the port %d", cmd_opts.port);
		error_msg(err_msg);
		return -1;
	}

    printf("Receiver available on the port %d\n", cmd_opts.port);

    while (handle_mouse_position(receiver_socket)) {}

    return 0;
}

void exit_handler() {
    close_socket(receiver_socket);
}