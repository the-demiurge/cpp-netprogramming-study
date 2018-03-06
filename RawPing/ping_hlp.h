#ifndef _PING_HLP_
#define _PING_HLP_

#include <string.h>
#include "icmp.h"

//Структура для опцій командного рядка
typedef struct _ping_cmd_opts {
	unsigned int packet_size;
	int record_route;
	int timeout;
	int ping_count;
} PING_CMD_OPTIONS, *PPING_CMD_OPTIONS;

#define SET_DEF_CMD_OPTS(cmd_opts)\
{\
  memset(&cmd_opts, 0, sizeof(cmd_opts));\
  cmd_opts.packet_size = DEFAULT_PACKET_SIZE;\
  cmd_opts.record_route = 0;\
  cmd_opts.timeout = 1000;\
  cmd_opts.ping_count = 4;\
}
//Коди помилок парсингу
#define EPARSE_CMD_COUNT -2
#define EPARSE_CMD_ANY -1
#define EPARSE_CMD_OK 0

//Парсинг опцій запуску ping
//Опції представлені через пробіл 
void parse_cmd_opts(const char*, PPING_CMD_OPTIONS);
//Парсинг усіх параметрів командного рядка
int parse_cmd(int argc, char* argv[], char*, PPING_CMD_OPTIONS);
#endif //_PING_HLP_
