#include "udp_echo_sender.h"

void exit_handler();

SOCKET client_socket;
int main(int argc, char* argv[])
{
	atexit(common_exit_handler);
	atexit(exit_handler);

	COMMAND_OPTIONS cmd_opts{ "", 0 };

	if (!parse_cmd(argc, argv, &cmd_opts) || parse_cmd(argc, argv, &cmd_opts) && !valid_connection_opts(&cmd_opts))
	{
		common_usage(argv[0]);
		return -1;
	}

    common_init_handler();

	client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (client_socket <= 0) {
		error_msg("Can't create socket");
		return -1;
	}

	struct sockaddr_in server_addr;
	init_inet_address(&server_addr, cmd_opts.host, cmd_opts.port);

	
	char message[256] = "";
	printf("%s", "Enter message:");
;
	scanf("%[^\n]s", message);
	int sc = sendto(client_socket, message, sizeof(message), 0, (sockaddr*)&server_addr, sizeof(server_addr));
	if (sc <= 0) {
		char err_msg[128] = "";
		sprintf(err_msg, "Can't send message to the %s:%d", cmd_opts.host, cmd_opts.port);
		error_msg(err_msg);
		return -1;
	}
	char buffer[256] = "";
	socklen_t len = sizeof(server_addr);
	int rec_cn = recvfrom(client_socket, buffer, sizeof(buffer), 0, (sockaddr*)&server_addr, &len);
	printf("Echo message: [%s]", buffer);
	return 0;
}

void exit_handler()
{
	close_socket(client_socket);
}
