#include "multi_thread_client.h"

THREAD_RESULT process_connection(void* data) {
    PCLIENT_OPTIONS poptions = (PCLIENT_OPTIONS)data;

    SOCKET client_socket = -1;
    CHECK_IO((client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) > 0, (THREAD_RESULT)-1, "Can't create client socket\n");

    struct sockaddr_in server_addr;
    init_inet_address(&server_addr, poptions->server_host, poptions->server_port);

    //Connect socket to the address on the server
    CHECK_IO(connect(client_socket, (sockaddr*)&server_addr, sizeof(sockaddr)) == 0, (THREAD_RESULT)-1, "Can't connect socket to server %s:%d\n", poptions->server_host, poptions->server_port);

    int sc = send(client_socket, poptions->data, strlen(poptions->data), 0);
    if (sc <= 0) {
        char err_msg[128] = "";
        sprintf(err_msg, "Can't send data to the server\n");
        error_msg(err_msg);
    }

    printf("Send bytes %d success\n", sc);

    close_socket(client_socket);
    return 0;
}