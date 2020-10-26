#include "multi_cast.h"

SOCKET gr_socket = -1;
void exit_handler();

int main(int argc, char **argv) {
    atexit(exit_handler);
	atexit(common_exit_handler);

	struct sockaddr_in local, remote;
	struct ip_mreq mcast;

    common_init_handler();

    MULTICAST_GROUP_OPTION group_option;

	parse_cmd_line(argc, argv, &group_option);

    gr_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (gr_socket <= 0) {
		printf("socket failed with: %d\n", get_last_error());
		return -1;
	}

	CHECK_SET_OPT(set_reuse_address(gr_socket, true), "ERROR REUSE ADDRESS");

	init_inet_address(&local, group_option.ip_interface, group_option.group_port);

	CHECK_IO(bind_socket_to(gr_socket, &local) == 0, -1, "ERROR BIND LOCAL");

    init_inet_address(&remote, group_option.group_ip_interface, group_option.group_port);

    init_group(group_option.ip_interface, group_option.group_ip_interface, &mcast);

    CHECK_SET_OPT(join_group(gr_socket, &mcast), "ERROR JOIN TO GROUP");
	CHECK_SET_OPT(set_group_ttl(gr_socket, 8), "ERROR SET TTL");

	if (group_option.is_loop_back) {
        CHECK_SET_OPT(set_loopback(gr_socket, 0), "ERROR SET LOOP");
	}

    if (group_option.is_receiver)
    {
        set_recv_timeout_ms(gr_socket, group_option.receive_timeout);

        while (true) {
            group_receiver(&group_option, gr_socket);
            if (get_last_error() == ERROR_TIMEOUT) {
                printf("Continue (Y/any key)?\n");
                if (toupper(getchar()) != 'Y') {
                    break;
                }
            }
        }

    } else if (group_option.is_sender)
    {
        group_sender(&group_option, gr_socket, &remote);
    }

	CHECK_SET_OPT(leave_group(gr_socket, &mcast), "LEAVE GROUP");

	return 0;
}

void exit_handler()
{
    if (gr_socket < 0)
    {
        close_socket(gr_socket);
    }
}