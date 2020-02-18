#include "icmp.h"
#include "common_thread.h"
#include "common_utils.h"

#include <stdio.h>
#include <string.h>

void fill_icmp_data(char *icmp_data, int datasize)
{
	PICMP_HEADER picmp_hdr = (PICMP_HEADER)icmp_data;
	picmp_hdr = (PICMP_HEADER)icmp_data;
	picmp_hdr->type = ICMP_ECHO; //
	picmp_hdr->code = 0;
	picmp_hdr->id = (uint16_t)get_process_id();
	picmp_hdr->checksum = 0;
	picmp_hdr->seq = 0;

	char *datapart = icmp_data + sizeof(ICMP_HEADER);

	memset(datapart, 'E', datasize - sizeof(ICMP_HEADER));
}

uint16_t checksum(uint16_t *buffer, int size)
{
	ulong_t cksum = 0;

	while (size > 1)
	{
		cksum += *buffer++;
		size -= sizeof(uint16_t);
	}
	if (size)
	{
		cksum += *(uint16_t*)buffer;
	}
	cksum = (cksum & 0xffff) + (cksum >> 16);
	cksum += (cksum >> 16);
	return (uint16_t)~cksum;
}

void decode_ip_opts(char *buf, int bytes)
{
	PIP_OPTION_HEADER ipopt = (PIP_OPTION_HEADER)(buf + 20);
	in_addr inaddr;
	hostent *host = NULL;

	printf("RR: ");
	int i, n = ipopt->ptr / 4;
	for (i = 0; i < n - 1; ++i)
	{
		char delim = i + 1 < n - 1 ? '\t' : '\n';
		inaddr.s_addr = ipopt->addr[i];

		host = gethostbyaddr((char *)&inaddr.s_addr, sizeof(inaddr.s_addr), AF_INET);
		if (host)
		{
			printf("(%-15s) %s", inet_ntoa(inaddr), host->h_name);
		}
		else
		{
			printf("(%-15s)", inet_ntoa(inaddr));
		}
		printf("%c", delim);
	}
}

void decode_icmp_hdr(char *buf, int bytes, struct sockaddr_in *from)
{
	static int icmpcount = 0;
	PIP_HEADER iphdr = (PIP_HEADER)buf;
	uint16_t iphdrlen = iphdr->h_len * 4;
	PICMP_HEADER icmphdr = (PICMP_HEADER)(buf + iphdrlen);
	char* str_from = inet_ntoa(from->sin_addr);

	ulong_t tick = get_tick_count();

	if (iphdrlen == MAX_IP_HDR_SIZE && !icmpcount)
	{
		decode_ip_opts(buf, bytes);
	}

	if (bytes < iphdrlen + ICMP_MIN)
	{
		printf("Too few bytes from %s\n", str_from);
	}

	if (icmphdr->type != ICMP_ECHOREPLY)
	{
		printf("non-echo type %d reponse\n", icmphdr->type);
		return;
	}

	uint16_t pid = get_process_id();

	if (icmphdr->id != pid)
	{
		printf("someone else responded packet!\n");
		return;
	}

	printf("\tReceived %d bytes from %s:", bytes, str_from);
	printf("\tICMP_seq = %d", icmphdr->seq);
	printf("\tTime: %ld ms\n", tick - icmphdr->timestamp);

	icmpcount++;
}