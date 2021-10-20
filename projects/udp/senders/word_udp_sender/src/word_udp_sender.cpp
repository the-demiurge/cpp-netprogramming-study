#include "word_udp_sender.h"
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

    struct sockaddr_in receiver_address;
    init_inet_address(&receiver_address, cmd_opts.host, cmd_opts.port);

	while(true) {
        printf("%s", "Enter msg (Quit to break):");
        struct MessagePacket msgPacket = {""};
        scanf("%[^\n]s", msgPacket.data);
        getchar();

        if (!strcasecmp(msgPacket.data, "Quit")) {
            break;
        }
        socklen_t len = sizeof(receiver_address);

        int sc = sendto(sender_socket, (char*)&msgPacket, sizeof(msgPacket), 0, (sockaddr*)&receiver_address, len);
        if (sc <= 0) {
            char err_msg[128] = "";
            sprintf(err_msg, "Can't send data to the %s:%d", cmd_opts.host, cmd_opts.port);
            error_msg(err_msg);
            return -1;
        }

        struct WordPacket wordPacket {"", 0};
        sc = recvfrom(sender_socket, (char*)&wordPacket, sizeof(wordPacket), 0, (sockaddr*)&receiver_address, &len);
        if (sc <= 0) {
            char err_msg[128] = "";
            sprintf(err_msg, "Can't receive data");
            error_msg(err_msg);
            return -1;
        }
        printf("Count of words: [%d]\n", wordPacket.words);
	}

	return 0;
}

void exit_handler()
{
	close_socket(sender_socket);
}
