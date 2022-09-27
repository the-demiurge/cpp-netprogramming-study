#include "simple_udp_sender.h"

void exit_handler();

SOCKET sender_socket;
int main(int argc, char* argv[])
{
    COMMAND_OPTIONS cmd_opts{"", 0};

    if (parse_cmd(argc, argv, &cmd_opts) || !valid_connection_opts(&cmd_opts))
    {
        common_usage(argv[0]);
        return -1;
    }

    atexit(exit_handler);
    atexit(common_exit_handler);
    common_init_handler();

    CHECK_IO((sender_socket = create_udp_socket()) > 0, -1, "Can't create socket\n");

    struct sockaddr_in server_addr;
    init_inet_address(&server_addr, cmd_opts.host, cmd_opts.port);

	
	char msg[256] = "";
	printf("%s", "Enter msg:");
	//fgets(msg, sizeof(msg), stdin);
	scanf("%[^\n]s", msg);
	int sc = sendto(sender_socket, msg, sizeof(msg), 0, (sockaddr*)&server_addr, sizeof(server_addr));
	if (sc <= 0) {
		char err_msg[512] = "";
		sprintf(err_msg, "Can't send data to the %s:%d", cmd_opts.host, cmd_opts.port);
		error_msg(err_msg);
		return -1;
	}

	return 0;
}

void exit_handler()
{
	close_socket(sender_socket);
}
