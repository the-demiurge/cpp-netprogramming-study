#ifndef _ICMP_
#define _ICMP_
#define IP_RECORD_ROUTE 0x7

#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0
#define ICMP_MIN 8 // Пакет ICMP не менше 8 байт (заголовок)

#define DEFAULT_PACKET_SIZE 32 // Стандартний розмір пакету
#define MAX_PACKET_SIZE 1024 // Максимальний розмір ICMP -пакета
#define MAX_IP_HDR_SIZE 60 // Максимальний розмір

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long ulong_t;

// Структура IP-заголовка
typedef struct _iphdr
{
	unsigned int h_len : 4; // Довжина заголовка
	unsigned int version : 4; // Версія IP
	uint8_t tos; // Тип служби
	uint16_t total_len; // Повний розмір пакету
	uint16_t ident; // Унікальний ідентифікатор
	uint16_t flags; // Прапори
	uint8_t ttl; // Час життя
	uint8_t proto; // Протокол (TCP, UDP і тому подібне)
	uint16_t checksum; // Контрольна сума IP
	ulong_t sourceIP;
	ulong_t destIP;
} IP_HEADER, *PIP_HEADER;

// Структура заголовка ICMP
typedef struct _icmphdr
{
	uint8_t type;
	uint8_t code; // Тип субкоду
	uint16_t checksum;
	uint16_t id;
	uint16_t seq;
	ulong_t timestamp;
} ICMP_HEADER, *PICMP_HEADER;

// Розширений заголовок IP -
// використовується з параметром IP_OPTIONS
typedef struct _ipoptionhdr
{
	uint8_t code; // Тип параметра
	uint8_t len; // Довжина розширеного заголовка
	uint8_t ptr; // Зміщення першої адреси
	ulong_t addr[9]; // Перелік IP адрес
} IP_OPTION_HEADER, *PIP_OPTION_HEADER;

// Допоміжна функція заповнення полів ICMP-запиту
void fill_icmp_data(char*, int);
// Обчислює 16-бітову суму комплементу
// для вказаного буфера із заголовком
uint16_t checksum(uint16_t*, int);
// Якщо є розширений заголовок, знаходить в ньому
// параметри IP і виводить значення параметра запису маршруту
void decode_ip_opts(char*, int);
// Декодує IP - заголовок пакету у відповідь
// для знаходження даних ICMP.
void decode_icmp_hdr(char*, int, struct sockaddr_in*);
#endif //_ICMP_
