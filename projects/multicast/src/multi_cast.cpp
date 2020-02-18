#include "multi_cast.h"

void parse_cmd_line(int argc, char **argv, PMULTICAST_GROUP_OPTION opts) {
	int i;
    opts->ip_interface = htonl(INADDR_ANY);
    opts->group_ip_interface = inet_addr(MCASTADDR);
	opts->group_port = MCASTPORT;
    opts->is_sender = true;
    opts->is_receiver = false;
    opts->is_loop_back = false;
    opts->repeat_count = DEFAULT_COUNT;

	for (i = 1; i < argc; i++) {
		if ((argv[i][0] == '-') || (argv[i][0] == '/')) {
			switch (tolower(argv[i][1])) {
			case 's': //
				opts->is_sender = true;
				break;
			case 'm': //
				if (strlen(argv[i]) > 3)
                    opts->group_ip_interface = inet_addr(&argv[i][3]);
				break;
			case 'i': //
				if (strlen(argv[i]) > 3)
                    opts->ip_interface = inet_addr(&argv[i][3]);
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
			}
		}
	}
}
