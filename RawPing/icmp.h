#ifndef _ICMP_
#define _ICMP_
#define IP_RECORD_ROUTE 0x7

#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0
#define ICMP_MIN 8 // Пакет ICMP не менше 8 байт (заголовок)

#define DEFAULT_PACKET_SIZE 32 // Стандартний розмір пакету
#define MAX_PACKET_SIZE 1024 // Максимальний розмір ICMP -пакета
#define MAX_IP_HDR_SIZE 60 // Максимальний розмір

// Структура IP-заголовка
typedef struct _iphdr
{
	unsigned int h_len : 4; // Довжина заголовка
	unsigned int version : 4; // Версія IP
	unsigned char tos; // Тип служби
	unsigned short total_len; // Повний розмір пакету
	unsigned short ident; // Унікальний ідентифікатор
	unsigned short flags; // Прапори
	unsigned char ttl; // Час життя
	unsigned char proto; // Протокол (TCP, UDP і тому подібне)
	unsigned short checksum; // Контрольна сума IP
	unsigned long sourceIP;
	unsigned long destIP;
} IP_HEADER, *PIP_HEADER;

// Структура заголовка ICMP
typedef struct _icmphdr
{
	unsigned char type;
	unsigned char code; // Тип субкоду
	unsigned short checksum;
	unsigned short id;
	unsigned short seq;
	unsigned long timestamp;
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

// Допоміжна функція заповнення полів ICMP-запиту
void fill_icmp_data(char*, int);
// Обчислює 16-бітову суму комплементу
// для вказаного буфера із заголовком
unsigned short checksum(unsigned short*, int);
// Якщо є розширений заголовок, знаходить в ньому
// параметри IP і виводить значення параметра запису маршруту
void decode_ip_opts(char*, int);
// Декодує IP - заголовок пакету у відповідь
// для знаходження даних ICMP.
void decode_icmp_hdr(char*, int, struct sockaddr_in*);
#endif //_ICMP_
