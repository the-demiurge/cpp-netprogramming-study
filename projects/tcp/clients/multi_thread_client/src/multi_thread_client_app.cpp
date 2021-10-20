#include "multi_thread_client.h"

void exit_handler();

std::vector<THREAD_HANDLE> connection_pool;

int main(int argc, char *argv[]) {
    COMMAND_OPTIONS cmd_opts{"", 0};

    if (parse_cmd(argc, argv, &cmd_opts) && valid_connection_opts(&cmd_opts))
    {
        common_usage(argv[0]);
        return -1;
    }

    atexit(exit_handler);
    atexit(common_exit_handler);
    common_init_handler();

    const int count_clients = 3;

    char messages[count_clients][50] = {"Message aSFdfgetgr", "Message 3245342", "Message 31534"};
    CLIENT_OPTIONS options[count_clients];
    for (int i = 0; i < count_clients; ++i) {
        PCLIENT_OPTIONS poptions = &options[i];
        strcpy(poptions->server_host, cmd_opts.host);
        poptions->server_port = cmd_opts.port;
        strcpy(poptions->data, messages[i]);

        connection_pool.push_back(
                create_thread(process_connection, poptions)
        );
    }

    wait_thread(connection_pool.data(), connection_pool.size());

    return 0;
}

void exit_handler() {
}