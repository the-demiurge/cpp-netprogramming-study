#pragma comment(lib, "ws2_32.lib")

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

#define IP_RECORD_ROUTE 0x7

#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0
#define ICMP_MIN 8 // Пакет ICMP не менше 8 байт (заголовок)

#define DEF_PACKET_SIZE 32 // Стандартний розмір пакету
#define MAX_PACKET 1024 // Максимальний розмір ICMP -пакета
#define MAX_IP_HDR_SIZE 60 // Максимальний розмір

// Структура заголовка IP
typedef struct _iphdr
{
	unsigned int h_len : 4; // Довжина заголовка
	unsigned int version : 4; // Версія IP
	unsigned char tos; // Тип служби
	unsigned short total_len; // Повний розмір пакету
	unsigned short ident; // Унікальний ідентифікатор
	unsigned short frag_and_flags; // Прапори
	unsigned char TTL; // Час життя
	unsigned char proto; // Протокол (TCP, UDP і тому подібне)
	unsigned short checksum; // Контрольна сума IP
	unsigned int sourceIP;
	unsigned int destIP;
} IP_HEADER, *PIP_HEADER;

// Структура заголовка ICMP
typedef struct _icmphdr
{
	unsigned char i_type;
	unsigned char i_code; // Тип субкоду
	unsigned short i_cksum;
	unsigned short i_id;
	unsigned short i_seq;
	// Нестандартне поле, резервується для часу
	unsigned short timestamp;
} ICMP_HEADER, *PICMP_HEADER;

// Розширений заголовок IP -
// використовується з параметром IP_OPTIONS
typedef struct _ipoptionhdr
{
	unsigned char code; // Тип параметра
	unsigned char len; // Довжина розширеного заголовка
	unsigned char ptr; // Зміщення першої адреси
	unsigned long addr[9]; // Перелік IP адрес
} IP_OPTION_HEADER, *PIP_OPTION_HEADER;

// виводить інформацію про використання
void usage(char *progname)
{
	printf("usage: ping - r <host> [data size]");
	printf(" - r record route");
	printf(" host remote machine to ping");
	printf(" datasize can be up to 1KB");
	ExitProcess(-1);
}

// Допоміжна функція заповнення полів ICMP-запиту
void fill_icmp_data(char *icmp_data, int datasize)
{
	PICMP_HEADER picmp_hdr = NULL;
	char *datapart = NULL;
	picmp_hdr = (PICMP_HEADER)icmp_data;
	picmp_hdr->i_type = ICMP_ECHO; // Луна-запит ICMP
	picmp_hdr->i_code = 0;
	picmp_hdr->i_id = (unsigned short)GetCurrentProcessId();
	picmp_hdr->i_cksum = 0;
	picmp_hdr->i_seq = 0;

	datapart = icmp_data + sizeof(ICMP_HEADER);
	// Помістимо які-небудь дані у буфер
	memset(datapart, 'E', datasize - sizeof(ICMP_HEADER));
}

// Обчислює 16-бітову суму комплементу
// для вказаного буфера із заголовком
unsigned short checksum(unsigned short *buffer, int size)
{
	unsigned long cksum = 0;

	while (size > 1)
	{
		cksum += *buffer++; size -= sizeof(unsigned short);
	}
	if (size)
	{
		cksum += *(unsigned short*)buffer;
	}
	cksum &= 0xffff;
	cksum += cksum >> 16;
	return ~cksum;
}

// Якщо є розширений заголовок, знаходить в ньому
// параметри IP і виводить значення параметра запису маршруту
void decode_ip_opts(char *buf, int bytes)
{
	PIP_OPTION_HEADER ipopt = (PIP_OPTION_HEADER)(buf + 20);
	in_addr inaddr;
	hostent *host = NULL;

	printf("RR: ");
	int i, n = ipopt->ptr / 4;
	for (i = 0; i < n - 1; ++i)
	{
		char delim = i + 1 < n - 1 ? ' ' : '\n';
		inaddr.S_un.S_addr = ipopt->addr[i];

		host = gethostbyaddr((char *)&inaddr.S_un.S_addr, sizeof(inaddr.S_un.S_addr), AF_INET);
		if (host)
		{
			printf("(%-15s) %s%c", inet_ntoa(inaddr), host->h_name, delim);
		}
		else
		{
			printf("(%-15s)%c", inet_ntoa(inaddr), delim);
		}
	}
	return;
}
// Декодує IP-заголовок пакету у відповідь
// для знаходження даних ICMP.
void decode_icmp_hdr(char *buf, int bytes, struct sockaddr_in *from)
{
	static int icmpcount = 0;
	PIP_HEADER iphdr = (PIP_HEADER)buf;
	unsigned short iphdrlen = iphdr->h_len * 4;
	PICMP_HEADER icmphdr = (PICMP_HEADER)(buf + iphdrlen);


	DWORD tick = GetTickCount();

	if (iphdrlen == MAX_IP_HDR_SIZE && !icmpcount)
	{
		decode_ip_opts(buf, bytes);
	}

	if (bytes < iphdrlen + ICMP_MIN)
	{
		printf("Too few bytes from %s\n", inet_ntoa(from->sin_addr));
	}

	if (icmphdr->i_type != ICMP_ECHOREPLY)
	{
		printf("nonecho type %d recived\n", icmphdr->i_type);
		return;
	}
	// Перевірка, що це ICMP-відповідь на повідомлення
	if (icmphdr->i_id != (unsigned short)GetCurrentProcessId())
	{
		printf("someone else responded packet!\n");
		return;
	}
	printf("%d bytes from %s:", bytes, inet_ntoa(from->sin_addr));
	printf(" ICMP_seq = %d. ", icmphdr->i_seq);
	printf(" time: %d ms", tick - icmphdr->timestamp);
	printf("");

	icmpcount++;
}

int resolve_host(const char* host, sockaddr_in* addr)
{
	memset(addr, 0, sizeof(struct sockaddr_in));
	long laddr = inet_addr(host);
	if (laddr == INADDR_NONE)
	{
		hostent* he = gethostbyname(host);
		if (he == NULL)
		{
			printf("Invalid resolving of host name %s\n", host);
			return -1;
		}
		addr->sin_family = he->h_addrtype;
		memcpy(&(addr->sin_addr), *(he->h_addr_list), he->h_length);
		printf("Resolved IP address: %s\n", inet_ntoa(addr->sin_addr));
		return 0;
	}
	else
	{
		addr->sin_family = AF_INET;
		(addr->sin_addr).s_addr = laddr;
		return 0;
	}
}

int parse_cmd(int argc, char* argv[], sockaddr_in* dest, int* rec_route, int* data_sz)
{
	if (argc < 2) {
		return false;
	}

	char all_args[256] = "";

	for (int i = 1; i < argc; ++i) {
		strcat(all_args, argv[i]);
		if (i + 1 < argc)
		{
			strcat(all_args, " ");
		}
	}

	char host[128] = "\0";
	char rec_rt_ch = '\0';

	char* format = "%s %d -%c";
	switch (sscanf(all_args, format, host, data_sz, &rec_rt_ch))
	{
	case 1:
	case 2:
	case 3:
		*rec_route = tolower(rec_rt_ch) == 'r';
		return resolve_host(host, dest) == 0;
	}

	return false;
}

#define CHECK_SOCK_OPT(ret, option)\
if (ret == SOCKET_ERROR)\
{\
	printf("setsockopt(%s) failed: %d\n", option, WSAGetLastError());\
	return -1;\
}

#define CHECK_SOCK_TRANSMIT(ret, transmit)\
if (ret == SOCKET_ERROR)\
{\
	if (WSAGetLastError() == WSAETIMEDOUT)\
	{\
		printf("timed out\n");\
		continue;\
	}\
	printf("%s failed: %d\n", transmit, WSAGetLastError());\
	return -1;\
}

#define CHECK_ALLOC_MEM(buffer, msg)\
if (!buffer)\
{\
  printf("Error allocate memory for %s\n", msg);\
  return -1;\
}

void exit_handler();

SOCKET sock_raw = INVALID_SOCKET;

char *icmp_data = NULL, *recvbuf = NULL;

// Настроює простий сокет ICMP, створює заголовок ICMP.
// Додає розширений заголовок IP,
// розсилає ICMP луну-запити по кінцевих точках.
// Задає тайм-аут в уникнення
// простою. Прийнятий пакет декодується.
int main(int argc, char **argv)
{
	atexit(exit_handler);

	struct sockaddr_in dest, from;
	int record_route = 0;
	int data_size = DEF_PACKET_SIZE;
	int timeout = 1000, ret;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup() failed: %d", GetLastError());
		return -1;
	}

	if (!parse_cmd(argc, argv, &dest, &record_route, &data_size))
	{
		usage(argv[0]);
		return -1;
	}


	unsigned int addr = 0;
	unsigned short seq_no = 0;
	IP_OPTION_HEADER ipopt;

	sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (sock_raw == INVALID_SOCKET)
	{
		printf("socket() failed: %d\n", WSAGetLastError());
		return -1;
	}

	if (record_route)
	{
		// Налаштування відправки розширеного заголовка IP
		// з кожним ICMP пакетом
		memset(&ipopt, 0, sizeof(ipopt));
		ipopt.code = IP_RECORD_ROUTE; // Запис маршруту
		ipopt.ptr = 4;	// Зміщення першої адреси
		ipopt.len = 39;	// Довжина розширеного заголовка

		ret = setsockopt(sock_raw, IPPROTO_IP, IP_OPTIONS, (char *)&ipopt, sizeof(ipopt));
		CHECK_SOCK_OPT(ret, "IP_OPTIONS");
	}
	// Налаштування тайм-аутів відправки і прийому
	ret = setsockopt(sock_raw, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	CHECK_SOCK_OPT(ret, "SO_RCVTIMEO");

	ret = setsockopt(sock_raw, SOL_SOCKET, SO_SNDTIMEO,
		(char*)&timeout, sizeof(timeout));
	CHECK_SOCK_OPT(ret, "SO_SNDTIMEO");

	//
	// Створення ICMP -пакета
	//
	data_size += sizeof(ICMP_HEADER);

	CHECK_ALLOC_MEM((icmp_data = (char*)malloc(MAX_PACKET)), "ICMP packet");
	CHECK_ALLOC_MEM((recvbuf = (char*)malloc(MAX_PACKET)), "receive buffer");
	memset(icmp_data, 0, MAX_PACKET);
	fill_icmp_data(icmp_data, data_size);

	int count_rcv = 0;
	// Початок відправки/прийому ICMP -пакетов
	while (count_rcv++ < 4)
	{
		PICMP_HEADER icmp_hdr = (PICMP_HEADER)icmp_data;
		icmp_hdr->i_cksum = 0;
		icmp_hdr->timestamp = GetTickCount();
		icmp_hdr->i_seq = seq_no++;
		icmp_hdr->i_cksum = checksum((unsigned short*)icmp_data, data_size);

		ret = sendto(sock_raw, icmp_data, data_size, 0, (struct sockaddr*)&dest, sizeof(dest));
		//CHECK_SOCK_TRANSMIT(ret, "sendto()");

		if (ret < data_size)
		{
			printf("Wrote %d bytes\n", ret);
		}
		int from_len = sizeof(sockaddr_in);
		ret = recvfrom(sock_raw, recvbuf, MAX_PACKET, 0, (struct sockaddr*)&from, &from_len);
		//CHECK_SOCK_TRANSMIT(ret, "recvfrom()");
		decode_icmp_hdr(recvbuf, ret, &from);
		Sleep(1000);
	}
	return 0;
}

void exit_handler()
{
	if (WSACleanup())
	{
		printf("Error WinSock cleanup: %d\n", WSAGetLastError());
	}
	if (sock_raw != INVALID_SOCKET)
	{
		closesocket(sock_raw);
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