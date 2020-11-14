#include "word_udp_sender.h"
void exit_handler();

SOCKET sender_socket;
int main(int argc, char* argv[])
{
	atexit(common_exit_handler);
	atexit(exit_handler);

	short port;
	char host[128] = "";
	bool parse_cmd_result = parse_cmd(argc, argv, host, &port);

	if (!parse_cmd_result || !host || !strlen(host))
	{
		printf("Invalid host or port. Usage %s -h host -p port\n", argv[0]);
		return -1;
	}

    common_init_handler();

    sender_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sender_socket <= 0) {
		error_msg("Can't create socket");
		return -1;
	}

	struct sockaddr_in receiver_address;
	init_inet_address(&receiver_address, host, port);

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
            sprintf(err_msg, "Can't send data to the %s:%d", host, port);
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
