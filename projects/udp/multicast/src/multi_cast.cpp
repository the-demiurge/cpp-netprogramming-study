#include "multi_cast.h"
void parse_cmd_line(int argc, char **argv, PMULTICAST_GROUP_OPTION opts) {
	int i;
    strcpy(opts->ip_interface, "");
    strcpy(opts->group_ip_interface, MCASTADDR);
	opts->group_port = MCASTPORT;
    opts->is_sender = false;
    opts->is_receiver = false;
    opts->is_loop_back = false;
    opts->repeat_count = DEFAULT_COUNT;
    opts->receive_timeout = 10000;

	for (i = 1; i < argc; i++) {
		if ((argv[i][0] == '-') || (argv[i][0] == '/')) {
			switch (tolower(argv[i][1])) {
			    case 's': //
			    case 'r': //
			        opts->is_sender = tolower(argv[i][1]) == 's';
				    opts->is_receiver = tolower(argv[i][1]) == 'r';
				    break;
			    case 'm': //
				    if (strlen(argv[i]) > 3) {
				        strcpy(opts->group_ip_interface, &argv[i][3]);
				    }
				    break;
			    case 'i': //
				    if (strlen(argv[i]) > 3) {
                        strcpy(opts->ip_interface, &argv[i][3]);
                    }
				    break;
			    case 'p': //
				    if (strlen(argv[i]) > 3) {
				        opts->group_port = atoi(&argv[i][3]);
				    }
				    break;
			    case 'l': //
				    opts->is_loop_back = true;
				    break;
			    case 'n': //
				    opts->repeat_count = atoi(&argv[i][3]);
				    break;
                case 't': //
                    opts->receive_timeout = atoi(&argv[i][3]);
                    break;
			}
		}
	}
}

int group_sender(PMULTICAST_GROUP_OPTION opts, SOCKET socket, struct sockaddr_in* to) {
    unsigned long int i;
    socklen_t ret;

    for (i = 0; i < opts->repeat_count; ++i)
    {
        char sendbuf[BUFSIZE];
        printf("Try to send...\n");
        sprintf(sendbuf, "server 1: This is a test: %ld", i);
        ret = sendto(socket, (char*)sendbuf, strlen(sendbuf), 0, (struct sockaddr*)to, sizeof(sockaddr_in));
        CHECK_IO(ret, -1, "Error send data in group\n");
        current_thread_sleep(500);
    }
    return 0;
}

int group_receiver(PMULTICAST_GROUP_OPTION opts, SOCKET socket) {
    char recvbuf[BUFSIZE];
    printf("Try to receive...\n");
    struct sockaddr_in from;
    socklen_t len = sizeof(struct sockaddr_in),  ret;
    ret = recvfrom(socket, recvbuf, BUFSIZE, 0, (struct sockaddr *)&from, &len);
    CHECK_IO(ret, -1, "Error receive data in group\n");
    recvbuf[ret] = 0;
    printf("RECV: '%s' from <%s>\n", recvbuf, inet_ntoa(from.sin_addr));
    return 0;
}
