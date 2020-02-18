#include "multi_cast.h"

SOCKET gr_socket = INVALID_SOCKET;
void exit_handler();

int main(int argc, char **argv) {
	atexit(common_exit_handler);
	atexit(exit_handler);

	struct sockaddr_in local, remote, from;
	struct ip_mreq mcast;

	char recvbuf[BUFSIZE], sendbuf[BUFSIZE];
	int len = sizeof(struct sockaddr_in), optval, ret;
	DWORD i = 0;

    common_init_handler();

    MULTICAST_GROUP_OPTION group_option;

	parse_cmd_line(argc, argv, &group_option);

    gr_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (gr_socket <= 0) {
		printf("socket failed with: %d\n", WSAGetLastError());
		return -1;
	}

	local.sin_family = AF_INET;
	local.sin_port = htons(group_option.group_port);
	local.sin_addr.s_addr = group_option.ip_interface;
	if (bind(gr_socket, (struct sockaddr*)&local, sizeof(local))) {
		printf("bind failed with: %d\n", WSAGetLastError());
		return -1;
	}

	remote.sin_family = AF_INET;
	remote.sin_port = htons(group_option.group_port);
	remote.sin_addr.s_addr = group_option.group_ip_interface;

	mcast.imr_multiaddr.s_addr = group_option.group_ip_interface;
	mcast.imr_interface.s_addr = group_option.ip_interface;

	int opt_ret = setsockopt(gr_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                             (char *)&mcast, sizeof(mcast));

	CHECK_SET_OPT(opt_ret, "IP_ADD_MEMBERSHIP");
	optval = 8;
	opt_ret = setsockopt(gr_socket, IPPROTO_IP, IP_MULTICAST_TTL,
                         (char*)&optval, sizeof(int));
	CHECK_SET_OPT(opt_ret, "IP_MULTICAST_TTL");

	if (group_option.is_loop_back) {
		optval = 0;
		opt_ret = setsockopt(gr_socket, IPPROTO_IP, IP_MULTICAST_LOOP,
                             (char *)&optval, sizeof(optval));
		CHECK_SET_OPT(opt_ret, "IP_MULTICAST_LOOP");
	}

	for (i = 0; i < group_option.repeat_count; ++i)
	{
		if (!group_option.is_sender) //
		{
		    ret = recvfrom(gr_socket, recvbuf, BUFSIZE, 0,
                           (struct sockaddr *)&from, &len);
			CHECK_IO(ret, -1, "recvfrom");
			recvbuf[ret] = 0;
			printf("RECV: '%s' from <%s>\n", recvbuf,
				inet_ntoa(from.sin_addr));
		}
		else
		{
		    sprintf(sendbuf, "server 1: This is a test: %d", i);
			ret = sendto(gr_socket, (char*)sendbuf, strlen(sendbuf), 0,
                         (struct sockaddr*)&remote, sizeof(remote));
			CHECK_IO(ret, -1, "sendto");
			Sleep(500);
		}
	}

	opt_ret = setsockopt(gr_socket, IPPROTO_IP, IP_DROP_MEMBERSHIP,
                         (char *)&mcast, sizeof(mcast));
	CHECK_SET_OPT(opt_ret, "IP_DROP_MEMBERSHIP");
	return 0;
}

void exit_handler()
{
    if (gr_socket != INVALID_SOCKET)
    {
        close_socket(gr_socket);
    }
}