#include "square_root_server.h"

void exit_handler();

SOCKET server_socket;

int main(int argc, char *argv[]) {
    
    COMMAND_OPTIONS cmd_opts{"", DEFAULT_PORT};

    parse_cmd(argc, argv, &cmd_opts);

    atexit(exit_handler);
    atexit(common_exit_handler);
    common_init_handler();

    CHECK_IO((server_socket = create_tcp_socket()) > 0, -1, "Can't create socket\n");

    struct sockaddr_in server_addr;
    init_inet_address(&server_addr, cmd_opts.host, cmd_opts.port);

    //Bind socket to the address on the server
    if (bind(server_socket, (sockaddr *) &server_addr, sizeof(sockaddr))) {
        char err_msg[128] = "";
        sprintf(err_msg, "Can't bind socket to the port %d", cmd_opts.port);
        error_msg(err_msg);
        return -1;
    }

    if (listen(server_socket, CONNECTION_QUEUE)) {
        error_msg("Error listening socket");
        return -1;
    }

    printf("Server running at the port %d\n", cmd_opts.port);

    while (true) {
        sockaddr_in incom_addr;
        memset(&incom_addr, 0, sizeof(incom_addr));
        socklen_t len = sizeof(incom_addr);
        SOCKET socket = accept(server_socket, (sockaddr *) &incom_addr, &len);
        if (socket <= 0) {
            error_msg("Can't accept connection");
            return -1;
        }
        handle_connection(socket, &incom_addr);
    }
    close_socket(server_socket);

    return 0;
}

void exit_handler() {
    close_socket(server_socket);
}