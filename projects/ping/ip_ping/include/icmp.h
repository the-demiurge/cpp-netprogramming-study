#ifndef __ICMP__
#define __ICMP__

#include "common_net.h"

#define IP_RECORD_ROUTE 0x7

#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0
#define ICMP_MIN 8

#define DEFAULT_PACKET_SIZE 32
#define MAX_PACKET_SIZE 1024
#define MAX_IP_HDR_SIZE 60

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long ulong_t;

typedef struct _iphdr
{
	unsigned int h_len : 4; //
	unsigned int version : 4; //
	uint8_t tos; //
	uint16_t total_len; //
	uint16_t ident; //
	uint16_t flags; //
	uint8_t ttl; //
	uint8_t proto; //
	uint16_t checksum; //
	ulong_t sourceIP;
	ulong_t destIP;
} IP_HEADER, *PIP_HEADER;

typedef struct _icmphdr
{
	uint8_t type;
	uint8_t code; //
	uint16_t checksum;
	uint16_t id;
	uint16_t seq;
	ulong_t timestamp;
} ICMP_HEADER, *PICMP_HEADER;

typedef struct _ipoptionhdr
{
	uint8_t code; //
	uint8_t len; //
	uint8_t ptr; //
	ulong_t addr[9]; //
} IP_OPTION_HEADER, *PIP_OPTION_HEADER;


void fill_icmp_data(char*, int);

uint16_t checksum(uint16_t*, int);

void decode_ip_opts(char*, int);

void decode_icmp_hdr(char*, int, struct sockaddr_in*);
#endif //
