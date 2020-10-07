#include "ping.h"

void exit_handler();

char *icmp_data = NULL, *recvbuf = NULL;

SOCKET sock_raw=-1;

int main(int argc, char **argv)
{
	atexit(common_exit_handler);
	atexit(exit_handler);

    common_init_handler();

    char host[256] = "";
    PING_CMD_OPTIONS cmd_opts;
    SET_DEF_CMD_OPTS(cmd_opts);
    if (parse_cmd_opts(argc, argv, host, &cmd_opts))
    {
        usage(argv[0]);
        return -1;
    }

    printf("Ping host: %s\n", host);

    struct sockaddr_in dest, from;
    int ret;

    if (!resolve_addr(host, &(dest.sin_addr)))
    {
        printf("Can't resolve host %s\n", host);
        return -1;
    }

    printf("Resolved IP address: %s\n", inet_ntoa(dest.sin_addr));

    sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (sock_raw == -1)
    {
        printf("socket failed: %d\n", get_last_error());
        return -1;
    }

    IP_OPTION_HEADER ipopt;
    if (cmd_opts.record_route)
    {
        memset(&ipopt, 0, sizeof(ipopt));
        ipopt.code = IP_RECORD_ROUTE; //
        ipopt.ptr = 4;	//
        ipopt.len = 39;	//

        ret = setsockopt(sock_raw, IPPROTO_IP, IP_OPTIONS, (char *)&ipopt, sizeof(ipopt));
        CHECK_SOCK_OPT(ret, "IP_OPTIONS");
    }

    ret = setsockopt(sock_raw, SOL_SOCKET, SO_RCVTIMEO,
                     (char*)&(cmd_opts.timeout), sizeof(cmd_opts.timeout));
    CHECK_SOCK_OPT(ret, "SO_RCVTIMEO");

    ret = setsockopt(sock_raw, SOL_SOCKET, SO_SNDTIMEO,
                     (char*)&(cmd_opts.timeout), sizeof(cmd_opts.timeout));
    CHECK_SOCK_OPT(ret, "SO_SNDTIMEO");


    cmd_opts.packet_size += sizeof(ICMP_HEADER);

    CHECK_ALLOC_MEM((icmp_data = (char*)malloc(MAX_PACKET_SIZE)), "ICMP packet");
    memset(icmp_data, 0, MAX_PACKET_SIZE);
    fill_icmp_data(icmp_data, cmd_opts.packet_size);

    CHECK_ALLOC_MEM((recvbuf = (char*)malloc(MAX_PACKET_SIZE)), "receive buffer");

    int i;
    uint16_t seq_no = 0;

    for (i = 0; i < cmd_opts.ping_count; ++i)
    {
        PICMP_HEADER icmp_hdr = (PICMP_HEADER)icmp_data;
        icmp_hdr->checksum = 0;
        icmp_hdr->timestamp = get_tick_count();
        icmp_hdr->seq = seq_no++;
        icmp_hdr->checksum = checksum((uint16_t*)icmp_data, cmd_opts.packet_size);

        ret = sendto(sock_raw, icmp_data, cmd_opts.packet_size, 0,
                     (struct sockaddr*)&dest, sizeof(dest));
        CHECK_SOCK_TRANSMIT(ret, "sendto()");

        if (ret < cmd_opts.packet_size)
        {
            printf("Wrote %d bytes\n", ret);
        }
        socklen_t from_len = sizeof(sockaddr_in);
        ret = recvfrom(sock_raw, recvbuf, MAX_PACKET_SIZE, 0,
                       (struct sockaddr*)&from, &from_len);
        CHECK_SOCK_TRANSMIT(ret, "recvfrom()");
        decode_icmp_hdr(recvbuf, ret, &from);
#ifdef _WIN32
        Sleep(1000);
#endif // _WIN32
    }
    return 0;
}

void exit_handler()
{
    if (sock_raw > 0)
    {
        close_socket(sock_raw);
    }

    if (icmp_data)
    {
        free(icmp_data);
    }

    if (recvbuf)
    {
        free(recvbuf);
    }
}
