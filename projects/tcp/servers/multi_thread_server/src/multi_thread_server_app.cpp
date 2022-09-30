#include "multi_thread_server.h"

void exit_handler();

SOCKET server_socket;
std::vector<THREAD_HANDLE> connection_pool;

volatile bool isRunning;

int main(int argc, char* argv[])
{
    COMMAND_OPTIONS cmd_opts{"", DEFAULT_PORT};

    parse_cmd(argc, argv, &cmd_opts);

    atexit(exit_handler);
    atexit(common_exit_handler);
    common_init_handler();

    CHECK_IO((server_socket = create_tcp_socket()) > 0, -1, "Can't create socket\n");

    sockaddr_in server_addr;
    init_inet_address(&server_addr, cmd_opts.host, cmd_opts.port);

	//Bind socket to the address on the server
	CHECK_IO(!bind(server_socket, (sockaddr*)&server_addr, sizeof(sockaddr)), -1, "Can't bind socket to the port %d\n", cmd_opts.port);
	//Set socket as passive
	CHECK_IO(!listen(server_socket, CONNECTION_QUEUE), -1, "Error listening socket\n");

	printf("Server running at the port %d\n", cmd_opts.port);

    isRunning = true;
	while (isRunning) {
        struct sockaddr_in incom_addr;
        memset(&incom_addr, 0, sizeof(incom_addr));
        socklen_t len = sizeof(incom_addr);
        SOCKET socket;
        CHECK_IO((socket = accept(server_socket, (sockaddr * ) & incom_addr, &len)) > 0, -1, "Can't accept connection\n");
        connection_pool.push_back(
                create_thread(handle_connection, (THREAD_PARAM) socket)
        );
    }

	return 0;
}

void exit_handler()
{
    close_socket(server_socket);
}