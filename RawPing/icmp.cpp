#include "icmp.h"

#ifdef _WIN32
 #include <windows.h>
#endif // _WIN32

#include <stdio.h>
#include <string.h>

void fill_icmp_data(char *icmp_data, int datasize)
{
	PICMP_HEADER picmp_hdr = (PICMP_HEADER)icmp_data;
	picmp_hdr = (PICMP_HEADER)icmp_data;
	picmp_hdr->type = ICMP_ECHO; // Луна-запит ICMP
	picmp_hdr->code = 0;
	picmp_hdr->id = (unsigned short)GetCurrentProcessId();
	picmp_hdr->checksum = 0;
	picmp_hdr->seq = 0;

	char *datapart = icmp_data + sizeof(ICMP_HEADER);
	// Помістимо які-небудь дані у буфер
	memset(datapart, 'E', datasize - sizeof(ICMP_HEADER));
}

unsigned short checksum(unsigned short *buffer, int size)
{
	unsigned long cksum = 0;

	while (size > 1)
	{
		cksum += *buffer++;
		size -= sizeof(unsigned short);
	}
	if (size)
	{
		cksum += *(unsigned short*)buffer;
	}
	cksum = (cksum & 0xffff) + (cksum >> 16);
	cksum += (cksum >> 16);
	return (unsigned short)~cksum;
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
		inaddr.S_un.S_addr = ipopt->addr[i];

		host = gethostbyaddr((char *)&inaddr.S_un.S_addr, sizeof(inaddr.S_un.S_addr), AF_INET);
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
	unsigned short iphdrlen = iphdr->h_len * 4;
	PICMP_HEADER icmphdr = (PICMP_HEADER)(buf + iphdrlen);

	DWORD tick = GetTickCount();
	printf("Decode: addr len %d\n", iphdrlen);
	if (iphdrlen == MAX_IP_HDR_SIZE && !icmpcount)
	{
		decode_ip_opts(buf, bytes);
	}

	if (bytes < iphdrlen + ICMP_MIN)
	{
		printf("Too few bytes from %s\n", inet_ntoa(from->sin_addr));
	}

	if (icmphdr->type != ICMP_ECHOREPLY)
	{
		printf("nonecho type %d recived\n", icmphdr->type);
		return;
	}
	// Перевірка, що це ICMP-відповідь на повідомлення
	if (icmphdr->id != (unsigned short)GetCurrentProcessId())
	{
		printf("someone else responded packet!\n");
		return;
	}
	printf("%d bytes from %s:", bytes, inet_ntoa(from->sin_addr));
	printf(" ICMP_seq = %d. ", icmphdr->seq);
	printf(" time: %d ms\n", tick - icmphdr->timestamp);

	icmpcount++;
}