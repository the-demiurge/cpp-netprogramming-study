#include "multi_thread_client.h"

void exit_handler();

std::vector<THREAD_HANDLE> connection_pool;

int main(int argc, char* argv[])
{
	atexit(common_exit_handler);
	atexit(exit_handler);
    short server_port = DEFAULT_PORT;
    char server_host[128] = "";
	bool parse_cmd_result = parse_cmd(argc, argv, server_host, &(server_port));

	common_init_handler();

	const int count_clients = 3;

	char messages[count_clients][50] = {"Message aSFdfgetgr", "Message 3245342", "Message 31534"};
	for (int i = 0; i < count_clients; ++i)
	{
	    CLIENT_OPTIONS client_options;
        strcpy(client_options.server_host, server_host);
        client_options.server_port = server_port;
        strcpy(client_options.data, messages[i]);

		connection_pool.push_back(
			create_thread(process_connection, (PCLIENT_OPTIONS)&client_options)
		);
	}

	return 0;
}

void exit_handler()
{
}