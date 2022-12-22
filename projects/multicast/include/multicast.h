#ifndef __MULTICAST_H__
#define __MULTICAST_H__
#include <winsock.h> 
#include <windows.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <ctime>

#define MCASTADDR "234.5.6.7" 
#define MCASTPORT 25 
#define BUFSIZE 1024
#define BUFSIZE 1024
#define DEFAULT_COUNT 50 

BOOL bSender = FALSE, //Діяти як відправник? 
bLoopBack = FALSE; //Заборонити петлі? 

SOCKET gr_socket = INVALID_SOCKET; //Сокет 
DWORD dwInterface, //Інтерфейс 
dwMulticastGroup, //багатоадресна група 
dwCount; //К-ть повідомлень для відправки/прийому 
short iPort; //Номер порту 
//Розбір аргументів командного рядка і установка 
//деяких глобальних опцій 
void parse_cmd(int argc, char** argv);
//Обробник виходу 
void exit_handler();
//Макрос для перевірки встановлення опцій сокету 
#define CHECK_SET_OPT(opt_ret, err_msg)\ 
{\
if ((opt_ret) < 0)\
{\
printf("setsockopt(%s) failed with: %d\n", \
    err_msg, WSAGetLastError()); \
    return -1; \
}\
}
//Макрос для перевірки введення-виведення 
#define CHECK_IO(io_ret, op_name)\ 
{\
if ((io_ret) < 0)\
{\
printf("%s was failed with: %d\n", \
    op_name, WSAGetLastError()); \
    return -1; \
}\
}
void parse_cmd(int argc, char** argv);
//Обробник виходу 
void exit_handler();

#endif //_MULTICAST_H_
