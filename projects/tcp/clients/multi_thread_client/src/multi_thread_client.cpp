#include "multi_thread_client.h"

THREAD_VOID process_connection(void* data) {
    PCLIENT_OPTIONS pclient_options = (PCLIENT_OPTIONS)data;

    SOCKET client_socket = -1;
    CHECK_VOID_IO((client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) > 0, "Can't create client socket\n");

    sockaddr_in server_addr;
    init_inet_address(&server_addr, pclient_options->server_host, pclient_options->server_port);

    //Connect socket to the address on the server
    CHECK_VOID_IO(connect(client_socket, (sockaddr*)&server_addr, sizeof(sockaddr)) == 0, "Can't connect socket to server %s:%d\n", pclient_options->server_host, pclient_options->server_port);

    int sc = send(client_socket, pclient_options->data, sizeof(pclient_options->data), 0);
    if (sc <= 0) {
        char err_msg[128] = "";
        sprintf(err_msg, "Can't send data to the server %s:%d\n", pclient_options->server_host, pclient_options->server_port);
        error_msg(err_msg);
    }

    close_socket(client_socket);
}